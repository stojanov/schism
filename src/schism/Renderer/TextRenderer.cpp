#include "TextRenderer.h"
#include <freetype/freetype.h>

// TODO: change usages to Schism logging func
#include <iostream>

namespace Schism
{
    FT_Library TextRenderer::m_library = {};

    void TextRenderer::Init()
    {
        if (FT_Init_FreeType(&m_library))
        {
            std::cout << "Failed to initialize Freetype library!" << std::endl;
            std::exit(-1);
        }
    }

    void TextRenderer::Deinit()
    {
        if (FT_Done_FreeType(m_library))
        {
            std::cout << "Failed to destroy Freetype library object!" << std::endl;
            std::exit(-1);
        }
    }

    Font TextRenderer::LoadFontFace(const std::string &fontPath)
    {
        constexpr uint8_t font_horizontal_resolution = 96;
        constexpr uint8_t font_vertical_resolution = 96;

        Font result_font{};
        result_font.pointSize = 10;

        FT_Face font_face;
        if (FT_New_Face(m_library, fontPath.c_str(), -1, &font_face))
        {
            std::cout << "ERROR: The following font family is not recognized by Freetype: " << fontPath << std::endl;
            return result_font;
        }

        std::uint16_t num_faces = font_face->num_faces;
        if (FT_Done_Face(font_face))
        {
            std::cout << "ERROR: Failed to discard font face object!" << std::endl;
            return result_font;
        }

        for (std::uint16_t face_index = 0; face_index < num_faces; face_index++)
        {
            FT_Long face_index_final = 0;
            FT_Face current_face;
            if (FT_New_Face(m_library, fontPath.c_str(), face_index_final, &current_face))
            {
                std::cout << "Failed to load font face #" << face_index_final << " from font " << fontPath << std::endl;
                return result_font;
            }

            if (FT_Set_Char_Size(current_face, 0, result_font.pointSize * 64, font_horizontal_resolution, font_vertical_resolution))
            {
                std::cout << "Failed to set char size for font face!" << std::endl;
            }

            result_font.face = current_face;

            // TODO: Currently we just load the single font face, but we need to
            // keep all font faces that were found, unless we specify one to keep
            break;
        }

        FT_Face face = result_font.face;
        if (face == nullptr)
        {
            std::cout << "ERROR: No font face found! Not proceeding with texture atlas generation." << std::endl;
            return result_font;
        }

        std::wstring character_map = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]\\;',./{}|:\"<>?;";

        result_font.characters.reserve(character_map.size());
        result_font.atlas_width = 0; result_font.atlas_height = 0;

        for (const unsigned char c : character_map)
        {
            if (FT_Load_Char(face, c, FT_LOAD_BITMAP_METRICS_ONLY))
            {
                std::cout << "Failed to load character " << c << std::endl;
                continue;
            }

            FT_GlyphSlot glyph = face->glyph;
            FontCharacter character
            {
                c,
                static_cast<int32_t>(glyph->bitmap.width),
                static_cast<int32_t>(glyph->bitmap.rows),
                0,
                0
            };
            result_font.characters.push_back(character);

            result_font.atlas_width += character.width;
            result_font.atlas_height = std::max(result_font.atlas_height, glyph->bitmap.rows);
        }

        return result_font;
    }

    void TextRenderer::RenderFontToAtlas(Font &font, const Ref<Renderer::Texture>& atlas)
    {
        assert(!font.characters.empty());

        atlas->Bind(0); // TODO: which binding point should we use actually?

        std::uint32_t rolling_x = 0;
        for (auto &character : font.characters)
        {
            if (FT_Load_Glyph(font.face, character.char_index, FT_LOAD_RENDER))
            {
                std::cout << "Failed to load character " << static_cast<char>(character.char_index) << std::endl;
                continue;
            }

            auto &glyph = font.face->glyph;

            character.x = rolling_x;
            atlas->SetSubData(glyph->bitmap.buffer, character.x, character.y, character.width, character.height);

            rolling_x += glyph->bitmap.width;
        }
    }

} // Schism
#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#include "vec2.hpp"
#include "schism/Core/Resources.h"

namespace Schism
{
	class TextRenderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void RegisterShader(Resources::ShaderHandle shader);
		static void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

	private:
		Resources::ShaderHandle m_Shader;
		std::map<char, struct Character> Characters;
	};

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};
}
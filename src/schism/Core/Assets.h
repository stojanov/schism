#pragma once

#include <entt/entt.hpp>

#include "AssetManager.h"
#include "schism/Gl/Shader.h"
#include "schism/Renderer/Font.h"
#include "schism/Renderer/Texture.h"
#include "schism/System/FileIO.h"
#include "schism/Renderer/FontLoader.h"

namespace Schism::Core
{
	struct TextureLoader: entt::resource_loader<TextureLoader, Renderer::Texture>
	{
		Ref<Renderer::Texture> static load(const std::string& path)
		{
			return Renderer::Texture::CreateRef(path);
		}
	};

	struct ShaderLoader: entt::resource_loader<ShaderLoader, Gl::Shader>
	{
		Ref<Gl::Shader> static load(const std::string& vertPath, const std::string& fragPath)
		{
			const std::string vertexData = System::ReadFile(vertPath);
			const std::string fragmentData = System::ReadFile(fragPath);
			 
			return MakeRef<Gl::Shader>(vertexData, fragmentData);
		}
	};

	struct FontLoader: entt::resource_loader<FontLoader, Gl::Shader>
	{
		Ref<Renderer::Font> static load(const std::string& path)
		{
			return Renderer::Font::Create(path);
		}
	};
	
	struct Assets
	{
		AssetManager<Renderer::Texture, TextureLoader> Textures;
		AssetManager<Gl::Shader, ShaderLoader> Shaders;
		AssetManager<Renderer::Font, FontLoader> Fonts;
	};
	
}

#pragma once

#include <entt/entt.hpp>

#include "AssetManager.h"
#include "schism/Gl/Shader.h"
#include "schism/Renderer/Texture.h"
#include "schism/System/FileIO.h"

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
			return Gl::Shader::Create(vertPath, fragPath);
		}
	};

	struct Assets
	{
		AssetManager<Renderer::Texture, TextureLoader> Textures;
		AssetManager<Gl::Shader, ShaderLoader> Shaders;
	};
	
}

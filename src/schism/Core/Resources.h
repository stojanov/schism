#pragma once
#include "entt/resource/handle.hpp"
#include "schism/Gl/Shader.h"
#include "schism/Renderer/Texture.h"

namespace Schism::Resources
{
	using ShaderHandle = entt::resource_handle<Gl::Shader>;
	using TextureHandle = entt::resource_handle<Renderer::Texture>;
}

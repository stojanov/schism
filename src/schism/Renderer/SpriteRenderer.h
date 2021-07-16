#pragma once

#include "schism/Gl/VertexArray.h"
#include "schism/Gl/VertexBuffer.h"
#include "schism/Core/Resources.h"

#include "schism/Components/Transfrom2D.h"
#include "schism/Components/Sprite.h"

namespace Schism::Renderer
{
	class SpriteRenderer
	{
	public:
		SpriteRenderer(Resources::ShaderHandle shader);

		void Draw(Components::Transform2D& transform, Components::Sprite& sprite, glm::mat4& proj);
	private:
		Ref<Gl::VertexBuffer> m_VertBuff;
		Gl::VertexArray m_VertexArray; 
		Resources::ShaderHandle m_Shader;
		
		static constexpr float vertices[] = {
			0.0f, 1.0f,		0.0f, 1.0f,
			1.0f, 0.0f,		1.0f, 0.0f,
			0.0f, 0.0f,		0.0f, 0.0f,

			0.0f, 1.0f,		0.0f, 1.0f,
			1.0f, 1.0f,		1.0f, 1.0f,
			1.0f, 0.0f,		1.0f, 0.0f
		};
	};
}

#include "SpriteRenderer.h"

#include "ext/matrix_transform.hpp"

namespace Schism::Renderer
{
	SpriteRenderer::SpriteRenderer(Resources::ShaderHandle shader)
		:
		m_Shader(shader)
	{
		m_VertBuff = Gl::VertexBuffer::CreateRef({ {
			{ Gl::ShaderDataType::Float2, "position" },
			{ Gl::ShaderDataType::Float2, "texcord" },
		}});

		m_VertBuff->SetData((float*) vertices, sizeof(vertices));

		m_VertexArray.AddVertexBuffer(m_VertBuff);
	}

	void SpriteRenderer::Draw(Components::Transform2D& transformComponent, Components::Sprite& spriteComponent, glm::mat4& proj)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(transformComponent.position, 0.f));
		transform = glm::translate(transform, glm::vec3(0.5f * transformComponent.scale.x, 0.5f * transformComponent.scale.y, 0.0f));
		transform = glm::rotate(transform, glm::radians(transformComponent.rotation), glm::vec3(0.f, 0.f, 1.f));
		transform = glm::translate(transform, glm::vec3(-0.5f * transformComponent.scale.x, -0.5f * transformComponent.scale.y, 0.0f));
		transform = glm::scale(transform, glm::vec3(transformComponent.scale, 1.f));

		spriteComponent.sprite->Bind(0);
		
		m_Shader->Bind();
		m_Shader->SetMat4("projection", proj);
		m_Shader->SetMat4("transform", transform);
		m_Shader->SetInt("sprite", 0);

		m_VertexArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
}

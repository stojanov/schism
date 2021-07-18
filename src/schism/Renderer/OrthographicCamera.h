#pragma once
#include <glm.hpp>

namespace Schism
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bot, float top);

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		
		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
	};
}

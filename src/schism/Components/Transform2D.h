#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Schism::Components
{
	struct Transform2D
	{
		glm::vec3 position;
		glm::vec2 scale;
		float rotation;
	};
}

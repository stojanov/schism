#pragma once
#include "vec3.hpp"

namespace Schism::Components
{
	struct Transform2D
	{
		glm::vec3 position;
		glm::vec3 scale;
		float rotation;
	};
}

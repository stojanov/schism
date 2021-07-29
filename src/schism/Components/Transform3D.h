#pragma once
#include "vec3.hpp"

namespace Schism::Components
{
	struct Transform3D
	{
		glm::vec3 position;
		glm::vec3 scale;
		float rotation;
	};
}

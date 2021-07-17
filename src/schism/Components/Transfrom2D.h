#pragma once
#include "vec2.hpp"

namespace Schism::Components
{
	struct Transform2D
	{
		glm::vec2 position;
		glm::vec2 scale;
		float rotation;
	};
}

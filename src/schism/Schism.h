#pragma once

// This should be included when using the engine

#include "GLFW/glfw3.h"
#include "Schism/Core/Application.h"
#include "Schism/Sandbox/Sandbox.h"
#include "Schism/System/System.h"

namespace Schism
{
	static void Init()
	{
		Log::Init();
		if (!glfwInit())
		{
			SC_CORE_CRITICAL("Coudn't initialize glfw");
		}
	}
}
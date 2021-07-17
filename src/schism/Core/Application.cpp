#include "Application.h"

#include "Window.h"
#include "GLFW/glfw3.h"
#include "schism/System/System.h"

namespace Schism
{
	Application::Application(int w, int h, const char* name)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		Ref<Core::Window> Window = MakeRef<Core::Window>();

		Window->Create(w, h, name);
		Window->SetEventCallback([this](Event& e)
			{
				OnEvent(e);
			});
		m_Ctx = CreateSharedContext(Window);

		SC_CORE_INFO("Schism succesfully initialized");
		SC_CORE_INFO("Gpu - {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
		SC_CORE_INFO("Driver - {0}", glGetString(GL_VERSION));
		SC_CORE_INFO("Shader Version - {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
	}
	
	void Application::Run()
	{
		// Boilerplate code
		auto winPtr = m_Ctx->Window->GetNativeWindow();

		while (!glfwWindowShouldClose(winPtr))
		{
			m_Ctx->Window->ProcessEvents();
			m_Ctx->Window->Swap();
		}

		SC_CORE_INFO("Schism shutting down");
		
		exit(0);
	}

}

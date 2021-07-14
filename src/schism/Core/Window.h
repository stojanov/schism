#pragma once
#include "WindowData.h"
#include "GLFW/glfw3.h"

namespace Schism::Core
{
	class Window
	{
	public:
		Window();
		virtual ~Window();

		int GetWidth() const { return m_Data.Width; }
		int GetHeight() const { return m_Data.Height; }


		void Swap() const;
		void Create(int w, int h, const char* name);
		void ProcessEvents();
		void SetEventCallback(EventCallback callback);

		GLFWwindow* GetNativeWindow() const { return m_WindowPtr; }
		GLFWwindow* GetLoadingContext() const { return m_LoadWinPtr; }
	private:
		void HookGLFWEventFunctions();
		void HookMouseEvents();
		void HookWindowEvents();
		void HookKeyEvents();
		
		GLFWwindow* m_WindowPtr;
		GLFWwindow* m_LoadWinPtr;
		WindowData m_Data;
	};
}

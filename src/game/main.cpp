#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <entt/entt.hpp>
#include <glm.hpp>

#include "schism/Core/Window.h"
#include "schism/Core/Events/KeyEvents.h"
#include "schism/Core/Events/MouseEvents.h"
#include "schism/System/System.h"
#include "schism/Renderer/Texture.h"

struct TextureLoader: entt::resource_loader<TextureLoader, Schism::Renderer::Texture>
{
	Schism::Ref<Schism::Renderer::Texture> static load(const char* path)
	{
		return Schism::Renderer::Texture::CreateRef(path);
	}
};

int main()
{
	Schism::Log::Init();

	Schism::Core::Window win;

	win.Create(640, 640, "Test window");
	static auto EventCallbackFunction = [](Schism::Event& e)
	{
		Schism::EventHandler handler(e);

		handler.Handle<Schism::KeyPressedEvent>([](Schism::KeyPressedEvent& e)
			{
				SC_CORE_INFO("Key pressed {0}", (char) e.GetKey());
			});

		handler.Handle<Schism::MouseButtonPressedEvent>([](Schism::MouseButtonPressedEvent& e)
			{
				auto pos = e.GetPosition();
				SC_CORE_INFO("Mouse Button Pressed {0}:({1}, {2})", e.GetKey(), pos.x, pos.y);
			});
	};
	win.SetEventCallback(EventCallbackFunction);
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(win.GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glm::vec3 backgroundColor{ 1.f, 1.f, 1.f };

	while (!glfwWindowShouldClose(win.GetNativeWindow()))
	{
		glfwPollEvents();
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::ColorEdit3("BG Color", (float*)&backgroundColor);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		win.Swap();
	}
}
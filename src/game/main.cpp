#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <entt/entt.hpp>
#include <glm.hpp>

#include "ext/matrix_clip_space.hpp"
#include "schism/Components/Sprite.h"
#include "schism/Components/Transfrom2D.h"
#include "schism/Core/Window.h"
#include "schism/Core/Events/KeyEvents.h"
#include "schism/Core/Events/MouseEvents.h"
#include "schism/System/System.h"
#include "schism/Renderer/Texture.h"
#include "schism/Core/SharedContext.h"
#include "schism/Renderer/SpriteRenderer.h"


/*
 * All of this is temporary
 */
int main()
{
	Schism::Log::Init();

	Schism::Ref<Schism::Core::Window> win = Schism::MakeRef<Schism::Core::Window>();
	Schism::Core::SharedContextRef ctx = CreateSharedContext(win);

	win->Create(1280, 720, "Test window");
	static auto EventCallbackFunction = [](Schism::Event& e)
	{
		Schism::EventHandler handler(e);

		handler.Handle<Schism::KeyPressedEvent>([](Schism::KeyPressedEvent& e)
			{
				//SC_CORE_INFO("Key pressed {0}", (char) e.GetKey());
			});

		handler.Handle<Schism::MouseButtonPressedEvent>([](Schism::MouseButtonPressedEvent& e)
			{
				auto pos = e.GetPosition();
				//SC_CORE_INFO("Mouse Button Pressed {0}:({1}, {2})", e.GetKey(), pos.x, pos.y);
			});
	};
	
	win->SetEventCallback(EventCallbackFunction);

	ctx->GlobalAssets.Shaders.Load("spriterenderer", "res/shaders/sprite_renderer.vert", "res/shaders/sprite_renderer.frag");
	ctx->GlobalAssets.Textures.Load("sprite1", "res/1.png");
	ctx->GlobalAssets.Textures.Load("sprite2", "res/2.png");

	glm::mat4 projection = glm::ortho(0.0f, (float) win->GetWidth(), (float) win->GetHeight(), 0.0f, -1.0f, 1.0f);

	entt::registry registry;
	Schism::Renderer::SpriteRenderer SRenderer(ctx->GlobalAssets.Shaders.GetHandle("spriterenderer"));

	auto ship1 = registry.create();
	auto ship2 = registry.create();
	 
	auto& ship1Transform = registry.emplace<Schism::Components::Transform2D>(ship1);
	auto& ship2Transform = registry.emplace<Schism::Components::Transform2D>(ship2);

	registry.emplace<Schism::Components::Sprite>(ship1, ctx->GlobalAssets.Textures.GetHandle("sprite1"));
	registry.emplace<Schism::Components::Sprite>(ship2, ctx->GlobalAssets.Textures.GetHandle("sprite2"));

	ship1Transform.position = { 10.f, 10.f };
	ship1Transform.scale = { 100.f, 100.f };
	
	ship2Transform.position = { 200.f, 200.f };
	ship2Transform.scale = { 100.f, 100.f };
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(win->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glm::vec3 backgroundColor{ 1.f, 1.f, 1.f };

	while (!glfwWindowShouldClose(win->GetNativeWindow()))
	{
		glfwPollEvents();
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		auto view = registry.view<Schism::Components::Transform2D, Schism::Components::Sprite>();

		for (auto entity: view)
		{
			auto& transform = view.get<Schism::Components::Transform2D>(entity);
			auto& sprite = view.get<Schism::Components::Sprite>(entity);
		
			SRenderer.Draw(transform, sprite, projection);
		}
		

		ImGui::Begin("Entities");
		int i = 0;
		for (auto entity : registry.view<Schism::Components::Transform2D>())
		{
			auto name = "Entity " + std::to_string(i++);
			auto& transform = view.get<Schism::Components::Transform2D>(entity);
			if (ImGui::CollapsingHeader(name.c_str()))
			{
				ImGui::DragFloat2("Position", (float*)&transform.position);
				ImGui::DragFloat2("Scale", (float*)&transform.scale);
				ImGui::DragFloat("Rotation", &transform.rotation);
			}
		}
		ImGui::End();

		ImGui::Begin("Basic Shit");
		ImGui::ColorEdit3("BG Color", (float*)&backgroundColor);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		win->Swap();
	}
}
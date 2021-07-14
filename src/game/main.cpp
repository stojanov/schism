#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <entt/entt.hpp>
#include <glm.hpp>

#include "schism/System/System.h"
int main()
{
	Schism::Log::Init();

	if (!glfwInit())
	{
		SC_CORE_CRITICAL("Cannot initialize glfw");
	}
	GLFWwindow* window = glfwCreateWindow(600, 600, "Yay", NULL, NULL);
	if (!window)
	{
		SC_CORE_CRITICAL("Canoot create window");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		SC_CORE_CRITICAL("Canoot load glad");
	}

	SC_CORE_INFO("Successfully created window");

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glm::vec3 backgroundColor{ 1.f, 1.f, 1.f };

	while (!glfwWindowShouldClose(window))
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
		glfwSwapBuffers(window);
	}
}
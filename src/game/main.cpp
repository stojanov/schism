#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

int main()
{
	if (!glfwInit())
	{
		std::cerr << "Coudn't initialize glfw";
	}
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "Yay", NULL, NULL);
	if (!window)
	{
		std::cerr << "Error creating window\n";
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Error creating context\n";
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0, 0.f, 0.f, 1.f);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
}
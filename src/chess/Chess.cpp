#include "Chess.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "schism/Components/Sprite.h"
#include "schism/Components/Transform2D.h"
#include "schism/Renderer/SpriteRenderer.h"
#include "schism/Renderer/Sprite.h"

using namespace Schism;

Chess::Chess(Core::SharedContextRef ctx, const std::string& name)
	:
	IScene(ctx, name),
	m_Camera(0, m_Ctx->window->GetWidth(), m_Ctx->window->GetHeight(), 0),
	m_Piece(CreateEntity())
{
}

Chess::~Chess() = default;

void Chess::OnAttach()
{
	entt::entity board = m_Registry.create();

	auto boardtexture = Assets.Textures.Load("board", "res/chess/board_alt.png", true);
	auto pieceTexture = Assets.Textures.Load("piece", "res/chess/white_king.png", true);

	m_Registry.emplace<Components::Sprite>(board, boardtexture);

	m_Piece.AddComponent<Components::Sprite>(pieceTexture);

	auto& transfrom = m_Registry.emplace<Components::Transform2D>(board);

	transfrom.position = { 0.f, 0.f };
	transfrom.scale = {m_Ctx->window->GetWidth(), m_Ctx->window->GetHeight()};

	auto& transform2 = m_Piece.AddComponent<Components::Transform2D>();

	transform2.position = { 0.f, 0.f };
	transform2.scale = { 100.f, 100.f };
}

void Chess::OnDetach()
{

}

void Chess::OnPause()
{

}

void Chess::OnResume()
{

}

void Chess::OnSystemEvent(Event & e)
{

}

void Chess::OnUpdate(Timestep ts)
{
}

void Chess::OnDraw()
{
	auto view = m_Registry.view<Components::Transform2D, Components::Sprite>();

	for (auto e : view)
	{
		const auto& [transform, sprite] = m_Registry.get<Components::Transform2D, Components::Sprite>(e);
		SpriteRenderer::Draw(transform, sprite, m_Camera.GetViewProjectionMatrix());
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	auto& tr = m_Piece.GetComponent<Components::Transform2D>();

	static float size = 50;

	ImGui::Begin("Debug");
	ImGui::SliderFloat("Piece Size", &size, 0, 800);
	ImGui::SliderFloat2("Piece Position", (float*)&tr.position, 0, 800);

	tr.scale.x = size;
	tr.scale.y = size;
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

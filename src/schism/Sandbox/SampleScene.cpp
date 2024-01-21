#include "SampleScene.h"

#include <glm/ext/matrix_transform.hpp>
#include "glm/matrix.hpp"
#include "glm/glm.hpp"
#include "schism/Components/Sprite.h"
#include "schism/Components/Transform2D.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Schism;

SampleScene::SampleScene(Core::SharedContextRef ctx, const std::string& name)
	:
	IScene(ctx, name),
	m_Camera(0, m_Ctx->window->GetWidth(), m_Ctx->window->GetHeight(), 0)
{
	m_Ship1 = m_Registry.create();
}

SampleScene::~SampleScene() = default;

void SampleScene::OnAttach()
{

	m_Renderer.RegisterShader(m_Ctx->GlobalAssets.Shaders.GetHandle("spriterenderer"));
	m_Registry.emplace<Components::Sprite>(m_Ship1, m_Ctx->GlobalAssets.Textures.GetHandle("ship1sprite"));
	auto& transfrom = m_Registry.emplace<Components::Transform2D>(m_Ship1);

	transfrom.position = { 200.f, 200.f, 0.f };
	transfrom.scale = { 200.f, 200.f };
    SC_CORE_INFO("ATTACh");
}

void SampleScene::OnDetach()
{

}

void SampleScene::OnPause()
{

}

void SampleScene::OnResume()
{

}

void SampleScene::OnSystemEvent(Event& e)
{

}

void SampleScene::OnUpdate(Timestep ts)
{
}

void SampleScene::OnDraw()
{
	auto view = m_Registry.view<Components::Transform2D, Components::Sprite>();
    int i = 0;
    SC_CORE_INFO("RENDERING");
	for (auto e : view)
	{
		const auto& [transfrom, sprite] = m_Registry.get<Components::Transform2D, Components::Sprite>(e);
        const glm::mat4 mat = glm::translate(glm::mat4(1.f), transfrom.position);
        if (i++ == 1)
        {
            // break;
        }
        // ImGuizmo::DrawCubes(glm::value_ptr(m_Camera.GetViewMatrix()), glm::value_ptr(m_Camera.GetProjectionMatrix()), glm::value_ptr(mat), 1);
		m_Renderer.Draw(transfrom, sprite, m_Camera.GetViewProjectionMatrix());
	}
}

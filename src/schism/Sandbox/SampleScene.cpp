#include "SampleScene.h"

#include "schism/Components/Sprite.h"
#include "schism/Components/Transform2D.h"

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

	transfrom.position = { 200.f, 200.f };
	transfrom.scale = { 200.f, 200.f };
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

	for (auto e : view)
	{
		const auto& [transfrom, sprite] = m_Registry.get<Components::Transform2D, Components::Sprite>(e);
		m_Renderer.Draw(transfrom, sprite, m_Camera.GetViewProjectionMatrix());
	}
}

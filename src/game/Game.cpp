#include "Game.h"

#include "chess/Chess.h"

Game::Game()
	:
	Application(800, 800, "Chess")
{
	m_Ctx->GlobalAssets.Shaders.Load("spriterenderer", "res/shaders/sprite_renderer.vert", "res/shaders/sprite_renderer.frag");

	SpriteRenderer::RegisterShader(m_Ctx->GlobalAssets.Shaders.GetHandle("spriterenderer"));

	m_SceneManager.Register<Chess>("chess");
}

Game::~Game()
{
	
}

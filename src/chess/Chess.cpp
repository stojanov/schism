#include "Chess.h"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "schism/Components/Sprite.h"
#include "schism/Renderer/SpriteRenderer.h"

#include "Common.h"

using namespace Schism;

namespace Chess
{
	Chess::Chess(Core::SharedContextRef ctx, const std::string& name)
		:
		IScene(ctx, name),
		m_Camera(0, m_Ctx->Window->GetWidth(), m_Ctx->Window->GetHeight(), 0)
	{
		BoardManager::Resources sprites;

		sprites.board.sprite = m_Assets.Textures.Load("board", "res/chess/board_alt.png", true);

		sprites.whiteSprites[PieceType_Pawn].sprite = m_Assets.Textures.Load("WPawn", "res/chess/white_pawn.png", true);
		sprites.whiteSprites[PieceType_LeftKnight].sprite = m_Assets.Textures.Load("WLeftKnight", "res/chess/white_knight.png", true);
		sprites.whiteSprites[PieceType_RightKnight].sprite = m_Assets.Textures.Load("WRightKnight", "res/chess/white_knight.png", true);
		sprites.whiteSprites[PieceType_LeftBishop].sprite = m_Assets.Textures.Load("WLeftBishop", "res/chess/white_bishop.png", true);
		sprites.whiteSprites[PieceType_RightBishop].sprite = m_Assets.Textures.Load("WRightBishop", "res/chess/white_bishop.png", true);
		sprites.whiteSprites[PieceType_Queen].sprite = m_Assets.Textures.Load("WQueen", "res/chess/white_queen.png", true);
		sprites.whiteSprites[PieceType_LeftRook].sprite = m_Assets.Textures.Load("WRook", "res/chess/white_rook.png", true);
		sprites.whiteSprites[PieceType_RightRook].sprite = m_Assets.Textures.Load("WRook", "res/chess/white_rook.png", true);
		sprites.whiteSprites[PieceType_King].sprite = m_Assets.Textures.Load("WKing", "res/chess/white_king.png", true);
		
		sprites.blackSprites[PieceType_Pawn].sprite = m_Assets.Textures.Load("BPawn", "res/chess/black_pawn.png", true);
		sprites.blackSprites[PieceType_LeftKnight].sprite = m_Assets.Textures.Load("BLeftKnight", "res/chess/black_knight.png", true);
		sprites.blackSprites[PieceType_RightKnight].sprite = m_Assets.Textures.Load("BRightKnight", "res/chess/black_knight.png", true);
		sprites.blackSprites[PieceType_LeftBishop].sprite = m_Assets.Textures.Load("BLeftBishop", "res/chess/black_bishop.png", true);
		sprites.blackSprites[PieceType_RightBishop].sprite = m_Assets.Textures.Load("BRightBishop", "res/chess/black_bishop.png", true);
		sprites.blackSprites[PieceType_Queen].sprite = m_Assets.Textures.Load("BQueen", "res/chess/black_queen.png", true);
		sprites.blackSprites[PieceType_LeftRook].sprite = m_Assets.Textures.Load("BRook", "res/chess/black_rook.png", true);
		sprites.blackSprites[PieceType_RightRook].sprite = m_Assets.Textures.Load("BRook", "res/chess/black_rook.png", true);
		sprites.blackSprites[PieceType_King].sprite = m_Assets.Textures.Load("BKing", "res/chess/black_king.png", true);

		int width = m_Ctx->Window->GetWidth();
		int height = m_Ctx->Window->GetHeight();
		float offsetPercentage = 0.32;
		float pieceSize = (width - (width * offsetPercentage)) / 8;

		m_BoardManager.Init(std::move(sprites), width, height, pieceSize, 0.16f);
	}

	Chess::~Chess() = default;

	void Chess::OnAttach()
	{
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

	void Chess::OnSystemEvent(Event& e)
	{
		const auto& board = m_Engine.GetBoardState();

		auto potentialMove = m_BoardManager.ProcessInput(e, board);

		if (potentialMove)
		{
			m_Engine.MakeMove(*potentialMove);
		}
	}

	void Chess::OnUpdate(Timestep ts)
	{
	}

	void Chess::OnDraw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		SpriteRenderer::BeginScene(m_Camera.GetProjectionMatrix());
		m_BoardManager.DrawBoard(m_Engine.GetBoardState());

		static float size = 50;

		ImGui::Begin("Debug");
		ImGui::SliderFloat("Piece Size", &size, 0, 800);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}

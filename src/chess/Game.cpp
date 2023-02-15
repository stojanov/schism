#include "Game.h"

#include "schism/Core/Events/MouseEvents.h"

namespace Chess
{
	Game::Game(BoardRenderer& renderer) // Interface for the boardrenderer would be better
		:
		m_BoardRenderer{ renderer }
	{

	}

	void Game::ProcessInput(Schism::Event& e)
	{
		switch (e.GetEventType())
		{
		case Schism::MouseButtonPressedEvent::GetStaticType():
		{
			const auto& board = m_Engine.GetBoardState();
			Schism::MouseButtonPressedEvent evt = static_cast<Schism::MouseButtonPressedEvent&>(e);
			auto& pos = evt.GetPosition();
			auto boardPosition = FindBoardPositionFromCoord(pos.x, pos.y);

			if (boardPosition.x >= board.size() || boardPosition.y >= board.size() ||
				boardPosition.x < 0 || boardPosition.y < 0)
			{
				return;
			}

			if (!m_State.isWhite)
			{
				boardPosition = std::move(FlipBoardPosition(boardPosition));
			}

			if (m_State.pieceSelected)
			{
				Move m;
				m.piece = board[m_State.selectedPosition.x][m_State.selectedPosition.y];
				m.currentPosition = boardPosition;
				m.prevPosition = m_State.selectedPosition;
				m_State.pieceSelected = false;
				m_ValidMoves.clear();

				m_Engine.MakeMove(m); // Temporary

				return;
			}

			if (board[boardPosition.x][boardPosition.y].type == PieceType_Blank)
			{
				return;
			}

			m_State.pieceSelected = true;
			m_State.selectedPosition = boardPosition;

			auto validMoves = m_Engine.GetValidMoves(boardPosition);

			if ()
			m_ValidMoves = m_Engine.GetValidMoves(boardPosition);
			break;
		}
		default:
			break;
		}
	}

	void Game::DrawBoard()
	{
		m_BoardRenderer.DrawBoard(m_Engine.GetBoardState(), !m_State.isWhite);
	}
}
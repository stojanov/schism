#include "Game.h"

#include "schism/Core/Events/MouseEvents.h"
#include "ChessEvents.h"

namespace Chess
{
	Game::Game(BoardRenderer& renderer, Schism::GameEvent::SyncBus& networkSendBus) // Interface for the boardrenderer would be better
		:
		m_BoardRenderer{ renderer },
        m_NetworkSendBus{ networkSendBus }
	{
        RegisterGameEvent<Move>();
        RegisterGameEvent<ResetGame>();
        RegisterGameEvent<StartGame>();

        ListenGameEvent<Move>([this](Move&& m)
          {
                m_Engine.MakeMove(m);
          });

        ListenGameEvent<ResetGame>([this](ResetGame&& e)
           {
                m_Engine.Reset();
           });

        ListenGameEvent<StartGame>([this](StartGame&& e)
           {
                m_State.isWhite = e.IsWhite;
                m_Engine.Reset();
           });
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
			auto&& boardPosition = FindBoardPositionFromCoord(pos.x, pos.y);

			if (boardPosition.x > board.size() || boardPosition.y > board.size() ||
				boardPosition.x < 0 || boardPosition.y < 0)
			{
				return;
			}

			if (!m_State.isWhite)
			{
				boardPosition = FlipBoardPosition(boardPosition);
			}

			auto selectPiece = [this](const auto& position)
			{
				if (position == m_State.selectedPosition)
				{
					return;
				}

				m_State.pieceSelected = true;
				m_State.selectedPosition = position;

				if (!m_State.isWhite)
				{
					m_ValidMoves = m_Engine.GetValidMoves(FlipBoardPosition(position));
				}
				else
				{
					m_ValidMoves = m_Engine.GetValidMoves(position);
				}
				if (!m_ValidMoves.empty())
				{
					SC_CORE_INFO("Valid move count {0}", m_ValidMoves.size());
				}
			};

			if (m_State.pieceSelected)
			{
				auto& prevPiece = board[m_State.selectedPosition.x][m_State.selectedPosition.y];
				auto& currentPiece = board[boardPosition.x][boardPosition.y];

				if (prevPiece.color == currentPiece.color && IsValidPiece(currentPiece.type))
				{
					selectPiece(boardPosition);
					return;
				}

				Move m;
				m.piece = board[m_State.selectedPosition.x][m_State.selectedPosition.y];
				m.currentPosition = boardPosition;
				m.prevPosition = m_State.selectedPosition;
				m_State.pieceSelected = false;
				m_ValidMoves = {};

				m_Engine.MakeMove(m); // Temporary

				return;
			}

			if (board[boardPosition.x][boardPosition.y].type == PieceType_Blank)
			{
				return;
			}

			selectPiece(boardPosition);
			
			break;
		}
		default:
			break;
		}
	}

    void Game::Update()
    {
        ProcessGameEvent();
    }

	void Game::DrawBoard()
	{
		if (!m_ValidMoves.empty())
		{
			m_BoardRenderer.DrawValidMoves(m_ValidMoves);
		}
		m_BoardRenderer.DrawBoard(m_Engine.GetBoardState(), !m_State.isWhite);
	}


}
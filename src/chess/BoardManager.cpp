#include "BoardManager.h"

#include <iostream>

#include "schism/Core/Events/MouseEvents.h"
#include "schism/Renderer/SpriteRenderer.h"

namespace Chess
{
	BoardManager::BoardManager()
		:
		m_Width{ 0 },
		m_Height{ 0 },
		m_PieceSize{ 0.f },
		m_Offset{ 0.f }
	{
	}

	void BoardManager::Init(Resources&& sprites, uint16_t width, uint16_t height,
		float pieceSize, float offsetPercentage)
	{
		m_Sprites = std::move(sprites);
		m_Width = width;
		m_Height = height;
		m_PieceSize = pieceSize;
		m_Offset = static_cast<float>(m_Width) * offsetPercentage;
	}

	std::optional<Move> BoardManager::ProcessInput(Schism::Event& e, const Board& board)
	{
		Schism::EventHandler handler(e);

		switch (e.GetEventType())
		{
		case Schism::MouseButtonPressedEvent::GetStaticType():
			{
				Schism::MouseButtonPressedEvent evt = static_cast<Schism::MouseButtonPressedEvent&>(e);
				auto& pos = evt.GetPosition();
				auto [boardX, boardY] = FindBoardPositionFromCoord(pos.x, pos.y);

				if (boardX > board.size() || boardY > board.size() ||
					boardX < 0 || boardY < 0)
				{
					return std::nullopt;
				}

				if (m_IsPieceSelected)
				{
					Move m;
					m.piece = board[m_SelectedPosition.first][m_SelectedPosition.second];
					m.positionX = boardX;
					m.positionY = boardY;
					m_IsPieceSelected = false;
					return { m };
				}

				m_IsPieceSelected = true;
				m_SelectedPosition.first = boardX;
				m_SelectedPosition.second = boardY;

				break;
			}
		default:
			break;
		}


		return std::nullopt;
	}

	void BoardManager::DrawBoard(const Board& board)
	{
		static Schism::Components::Transform2D transform;

		for (int x = 0; x < board.size(); x++)
		{
			for (int y = 0; y < board[0].size(); y++)
			{
				if (board[x][y].type == PieceType_Blank)
				{
					continue;
				}

				Schism::Components::Sprite* sprite = nullptr;

				if (board[x][y].color == PieceColor_White)
				{
					sprite = &m_Sprites.whiteSprites[board[x][y].type];
				}
				else
				{
					sprite = &m_Sprites.blackSprites[board[x][y].type];
				}

				transform.position = {
					m_PieceSize * x + m_Offset,
					m_PieceSize * y + m_Offset
				};
				transform.scale = { m_PieceSize, m_PieceSize };

				Schism::SpriteRenderer::Draw(transform, *sprite);
			}
		}

		transform.position = { 0.f, 0.f };
		transform.scale = { m_Width, m_Height };

		Schism::SpriteRenderer::Draw(transform, m_Sprites.board);
	}
}

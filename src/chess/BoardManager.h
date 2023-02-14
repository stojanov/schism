#pragma once

#include <optional>

#include "Common.h"
#include "Engine.h"
#include "schism/Components/Sprite.h"
#include "schism/Core/Events/Event.h"

namespace Chess
{
	class BoardManager
	{
	public:
		struct Resources
		{
			std::array<Schism::Components::Sprite, 9> whiteSprites;
			std::array<Schism::Components::Sprite, 9> blackSprites;
			Schism::Components::Sprite board;
		};

		BoardManager();
		std::optional<Move> ProcessInput(Schism::Event& e, const Board& board);
		void Init(Resources&& sprites, uint16_t width, uint16_t height,
			float pieceSize, float offsetPercentage);
		void DrawBoard(const Board& board);
	private:
		std::pair<uint8_t, uint8_t> FindBoardPositionFromCoord(float x, float y)
		{
			uint8_t gridX = (x - m_Offset) / m_PieceSize;
			uint8_t gridY = (y - m_Offset) / m_PieceSize;

			return std::make_pair(gridX, gridY);
		}

		bool m_IsPieceSelected{ false };
		std::pair<uint8_t, uint8_t> m_SelectedPosition;
		Resources m_Sprites;
		uint16_t m_Width;
		uint16_t m_Height;
		float m_PieceSize;
		float m_Offset;
	};
}

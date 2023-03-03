#pragma once

#include "Common.h"
#include "BoardRenderer.h"
#include "Engine.h"

namespace Chess
{
	// Will handle everything from networking to input processing
	class Game
	{
	public:
		Game(BoardRenderer& renderer);

		void ProcessInput(Schism::Event& e);
		void DrawBoard();
	private:
		struct State
		{
			bool pieceSelected{ false };
			Position selectedPosition{ 0, 0 };
			bool isWhite{ true };
		};

		[[nodiscard]] Position FindBoardPositionFromCoord(float x, float y) const
		{
			const auto offset = m_BoardRenderer.GetOffset();
			const auto pieceSize = m_BoardRenderer.GetPieceSize();

			const uint8_t gridX = (x - offset) / pieceSize;
			const uint8_t gridY = (y - offset) / pieceSize;

			return { gridX, gridY };
		}

		State m_State;
		Engine m_Engine;
		BoardRenderer& m_BoardRenderer;
		std::optional<std::vector<Position>> m_ValidMoves;
		float m_Offset;
		float m_PieceSize;
	};
}
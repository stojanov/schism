#include "Engine.h"

namespace Chess
{
	Engine::Engine()
	{
		Reset();
	}

	const Board& Engine::GetBoardState()
	{
		return m_Board;
	}

	void Engine::Reset()
	{
		for (auto& line : m_Board)
		{
			line.fill({ });
		}

		for (int i = 0; i < 8; i++)
		{
			m_Board[i][1] = { PieceType_Pawn, PieceColor_Black };
			m_Board[i][6] = { PieceType_Pawn, PieceColor_White };
		}

		m_Board[0][0] = { PieceType_LeftRook, PieceColor_Black };
		m_Board[1][0] = { PieceType_LeftKnight, PieceColor_Black };
		m_Board[2][0] = { PieceType_LeftBishop, PieceColor_Black };
		m_Board[3][0] = { PieceType_Queen, PieceColor_Black };
		m_Board[4][0] = { PieceType_King, PieceColor_Black };
		m_Board[5][0] = { PieceType_RightBishop, PieceColor_Black };
		m_Board[6][0] = { PieceType_RightKnight, PieceColor_Black };
		m_Board[7][0] = { PieceType_RightRook, PieceColor_Black };

		m_Board[0][7] = { PieceType_LeftRook, PieceColor_White };
		m_Board[1][7] = { PieceType_LeftKnight, PieceColor_White };
		m_Board[2][7] = { PieceType_LeftBishop, PieceColor_White };
		m_Board[3][7] = { PieceType_Queen, PieceColor_White };
		m_Board[4][7] = { PieceType_King, PieceColor_White };
		m_Board[5][7] = { PieceType_RightBishop, PieceColor_White };
		m_Board[6][7] = { PieceType_RightKnight, PieceColor_White };
		m_Board[7][7] = { PieceType_RightRook, PieceColor_White };
	}

	bool Engine::MakeMove(const Move& move)
	{
		for (int x = 0; x < 8; x++)
		{
			for (int y = 0; y < 8; y++)
			{
				if (m_Board[x][y].type == move.piece.type &&
					m_Board[x][y].color == move.piece.color)
				{
					auto temp = m_Board[x][y];

					m_Board[x][y] = { PieceType_Blank };
					m_Board[move.positionX][move.positionY] = temp;
				}
			}
		}

		return true;
	}


}

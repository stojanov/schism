#include "Engine.h"

namespace Chess
{
	Engine::Engine()
	{
		Reset();
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

		m_Board[0][0] = { PieceType_Rook, PieceColor_Black };
		m_Board[1][0] = { PieceType_Knight, PieceColor_Black };
		m_Board[2][0] = { PieceType_Bishop, PieceColor_Black };
		m_Board[3][0] = { PieceType_Queen, PieceColor_Black };
		m_Board[4][0] = { PieceType_King, PieceColor_Black };
		m_Board[5][0] = { PieceType_Bishop, PieceColor_Black };
		m_Board[6][0] = { PieceType_Knight, PieceColor_Black };
		m_Board[7][0] = { PieceType_Rook, PieceColor_Black };

		m_Board[0][7] = { PieceType_Rook, PieceColor_White };
		m_Board[1][7] = { PieceType_Knight, PieceColor_White };
		m_Board[2][7] = { PieceType_Bishop, PieceColor_White };
		m_Board[3][7] = { PieceType_Queen, PieceColor_White };
		m_Board[4][7] = { PieceType_King, PieceColor_White };
		m_Board[5][7] = { PieceType_Bishop, PieceColor_White };
		m_Board[6][7] = { PieceType_Knight, PieceColor_White };
		m_Board[7][7] = { PieceType_Rook, PieceColor_White };
	}

	bool Engine::MakeMove(const Move& move)
	{
		const auto piece = m_Board[move.prevPosition.x][move.prevPosition.y];
		const auto nextPiece = m_Board[move.currentPosition.x][move.currentPosition.y];

		// Check if we want to move to another piece by the same color
		if (move.piece.color == nextPiece.color && nextPiece.type != PieceType_Blank)
		{
			return false;
		}

		auto validMoves = GetValidMoves(move.prevPosition);
		if (!validMoves)
		{
			return false; // Figure what to do if no valid moves
		}

		for (auto& validMovePosition : *validMoves)
		{
			if (validMovePosition == move.currentPosition)
			{
				m_Board[move.prevPosition.x][move.prevPosition.y] = { PieceType_Blank };
				m_Board[move.currentPosition.x][move.currentPosition.y] = piece;
				return true;
			}
		}

		return true;
	}

	std::optional<std::vector<Position>> Engine::GetValidMoves(const Position& position)
	{
		if (position.x >= m_Board.size() || position.y >= m_Board.size())
		{
			return std::nullopt;
		}

		auto& piece = m_Board[position.x][position.y];

		switch (piece.type)
		{
		case PieceType_Pawn:
		{
			return ValidMovesPawn(position);
		}
		case PieceType_Knight:
		{
			return ValidMovesKnight(position);
		}
		case PieceType_Bishop:
		{
			return ValidMovesBishop(position);
		}
		case PieceType_Queen:
		{
			return ValidMovesQueen(position);
		}
		case PieceType_King:
		{
			return ValidMovesKing(position);
		}
		case PieceType_Rook:
		{
			return ValidMovesRook(position);
		}
		default:
			return std::nullopt;
		}
	}

	void Engine::CheckObstacleHorizontally(std::vector<Position>& validMoves, const Position& position, uint8_t length, bool canTake, bool descending)
	{
		auto& myPiece = m_Board[position.x][position.y];

		auto checkObstacale = [&](const Position& boardPosition) -> bool
		{
			if (boardPosition.y < 0 || boardPosition.y > m_Board.size())
			{
				return false;
			}
			auto& piece = m_Board[boardPosition.x][boardPosition.y];

			if (canTake)
			{
				if (IsValidPiece(piece.type) && piece.color == InvertPieceColor(myPiece.color))
				{
					validMoves.push_back(boardPosition);
					return false;
				}
			}
			else
			{
				if (IsValidPiece(piece.type))
				{
					return false;
				}
				validMoves.push_back(boardPosition);
			}
		};

		for (uint8_t y = position.y + 1; y <= position.y + (length - 1); y++)
		{
			if (!checkObstacale({ position.x, y }))
			{
				break;
			}
		}

		if (descending)
		{
			uint8_t y = position.y - 1;
			while ((y >= position.y - (length - 1)) && y >= 0)
			{
				if (!checkObstacale({ position.x, y }))
				{
					break;
				}
				y--;
			}
		}
	}

	std::vector<Position> Engine::ValidMovesPawn(const Position& position)
	{
		std::vector<Position> validMoves;

		validMoves.reserve(5);

		auto& myPiece = m_Board[position.x][position.y];

		inline auto checkOpposingPiece = [&](const Position& newPosition)
		{
			auto& opposingPiece = m_Board[newPosition.x][newPosition.y];
			if (IsValidPiece(opposingPiece.type) &&
				myPiece.color == InvertPieceColor(opposingPiece.color))
			{
				validMoves.push_back(newPosition);
			}
		};

		inline auto checkCanTake = [&](bool descending)
		{
			bool left = position.x != 0;
			bool right = position.x != 6;

			if (!descending)
			{
				if (position.y <= m_Board.size() - 2)
				{
					if (left)
					{
						Position newPos{ position.x - 1, position.y + 1 };
						checkOpposingPiece(newPos);
					}
					if (right)
					{
						Position newPos{ position.x + 1, position.y + 1 };
						checkOpposingPiece(newPos);
					}
				}
			}
			else
			{
				if (position.y >= 1)
				{
					if (left)
					{
						Position newPos{ position.x - 1, position.y - 1};
						checkOpposingPiece(newPos);
					}
					if (right)
					{
						Position newPos{ position.x + 1, position.y - 1 };
						checkOpposingPiece(newPos);
					}
				}
			}
		};

		if (myPiece.color == PieceColor_White)
		{
			checkCanTake(true);
			if (position.y == 6)
			{
				CheckObstacleHorizontally(validMoves, position, 3, false, true);
			}
			else
			{
				if (position.y <= 1)
				{
					auto& piece = m_Board[position.x][position.y - 1];
					if (!IsValidPiece(piece.type))
					{
						Position newPos{ position.x, position.y - 1 };
						validMoves.push_back(newPos);
					}
				}
			}
		}
		else 
		{
			checkCanTake(false);
			if (position.y == 1)
			{
				CheckObstacleHorizontally(validMoves, position, 3, false, false);
			}
			else
			{
				if (position.y >= 6)
				{
					auto& piece = m_Board[position.x][position.y + 1];
					if (!IsValidPiece(piece.type))
					{
						Position newPos{ position.x, position.y + 1 };
						validMoves.push_back(newPos);
					}
				}
			}
		}

		return validMoves;
	}

	std::vector<Position> Engine::ValidMovesKnight(const Position& position)
	{

	}

	std::vector<Position> Engine::ValidMovesBishop(const Position& position)
	{

	}

	std::vector<Position> Engine::ValidMovesQueen(const Position& position)
	{

	}

	std::vector<Position> Engine::ValidMovesKing(const Position& position)
	{

	}

	std::vector<Position> Engine::ValidMovesRook(const Position& position)
	{

	}

}

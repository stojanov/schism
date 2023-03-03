#include "Engine.h"

#include "schism/System/Log.h"

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

		m_ValidMoves.clear();
		m_ValidMoves.reserve(m_Board.size() * 2);
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

		const auto validMoves = GetValidMoves(move.prevPosition);
		if (!validMoves)
		{
			return false; // Figure what to do if no valid moves
		}

		for (auto& validMovePosition : *validMoves)
		{
			if (validMovePosition == move.currentPosition)
			{
				// Implement takes piece
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

	bool Engine::CheckObstacle(std::vector<Position>& validMoves, const Piece& myPiece, const Position& position, bool canTake) const
	{
		if (position.y >= m_Board.size() ||
			position.x >= m_Board.size())
		{
			return false;
		}
		auto& piece = m_Board[position.x][position.y];

		if (canTake &&
			IsValidPiece(piece.type) &&
			piece.color == InvertPieceColor(myPiece.color))
		{
			validMoves.push_back(position);
			return false; // Stop if we find a valid piece to take
		}

		if (IsValidPiece(piece.type) && piece.color != InvertPieceColor(piece.color))
		{
			return false;
		}

		validMoves.push_back(position);

		return true;
	}


	void Engine::CheckObstacleVertically(std::vector<Position>& validMoves, const Position& position, uint8_t length, bool canTake, bool descending) const
	{
		const auto& myPiece = m_Board[position.x][position.y];
		for (uint8_t y = position.y + 1; y <= static_cast<uint8_t>(position.y + (length - 1)); y++)
		{
			if (!CheckObstacle(validMoves, myPiece, {position.x, y}, canTake))
			{
				break;
			}
		}

		if (descending)
		{
			const uint8_t yLimit = (length - 1) > position.y ? 0 : static_cast<uint8_t>(position.y - (length - 1));
			for (uint8_t y = position.y - 1; y >= yLimit; y--)
			{
				if (!CheckObstacle(validMoves, myPiece, { position.x, y }, canTake))
				{
					break;
				}
			}
		}
	}

	void Engine::CheckObstacleHorizontally(std::vector<Position>& validMoves, const Position& position, uint8_t length, bool canTake, bool descending) const
	{
		const auto& myPiece = m_Board[position.x][position.y];
		for (uint8_t x = position.x + 1; x <= static_cast<uint8_t>(position.x + (length - 1)); x++)
		{
			if (!CheckObstacle(validMoves, myPiece, { x, position.y }, canTake))
			{
				break;
			}
		}

		if (descending)
		{
			const uint8_t xLimit = (length - 1) > position.x ? 0 : static_cast<uint8_t>(position.x - (length - 1));
			for (uint8_t x = position.x - 1; x >= xLimit; x--)
			{
				if (!CheckObstacle(validMoves, myPiece, { x , position.y }, canTake))
				{
					break;
				}
			}
		}
	}

	const std::vector<Position>& Engine::ValidMovesPawn(const Position& position)
	{
		m_ValidMoves.clear();

		const auto& myPiece = m_Board[position.x][position.y];

		auto checkOpposingPiece = [&](const Position& newPosition)
		{
			const auto& opposingPiece = m_Board[newPosition.x][newPosition.y];
			if (IsValidPiece(opposingPiece.type) &&
				myPiece.color == InvertPieceColor(opposingPiece.color))
			{
				m_ValidMoves.push_back(newPosition);
			}
		};

		auto checkCanTake = [&](bool descending)
		{
			const bool left = position.x != 0;
			const bool right = position.x != 7;

			if (!descending)
			{
				if (position.y <= m_Board.size() - 2)
				{
					if (left)
					{
						const Position newPos{
							static_cast<uint8_t>(position.x - 1),
							static_cast<uint8_t>(position.y + 1)
						};
						checkOpposingPiece(newPos);
					}
					if (right)
					{
						const Position newPos{
							static_cast<uint8_t>(position.x + 1),
							static_cast<uint8_t>(position.y + 1)
						};
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
						const Position newPos{
							static_cast<uint8_t>(position.x - 1),
							static_cast<uint8_t>(position.y - 1)
						};
						checkOpposingPiece(newPos);
					}
					if (right)
					{
						const Position newPos{
							static_cast<uint8_t>(position.x + 1),
							static_cast<uint8_t>(position.y - 1)
						};
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
				CheckObstacleVertically(m_ValidMoves, position, 3, false, true);
			}
			else
			{
				// Need to make sure we setup queening
				if (position.y >= 1)
				{
					const Position newPos{ position.x, static_cast<uint8_t>(position.y - 1) };
					const auto& piece = m_Board[newPos.x][newPos.y];
					if (piece.type == PieceType_Blank)
					{
						m_ValidMoves.push_back(newPos);
					}
				}
			}
		}
		else 
		{
			checkCanTake(false);
			if (position.y == 1)
			{
				CheckObstacleVertically(m_ValidMoves, position, 3, false, false);
			}
			else
			{
				// Need to make sure we setup queening
				if (position.y <= 6)
				{
					const Position newPos{ position.x, static_cast<uint8_t>(position.y + 1) };
					const auto& piece = m_Board[newPos.x][newPos.y];
					if (piece.type == PieceType_Blank)
					{
						
						m_ValidMoves.push_back(newPos);
					}
				}
			}
		}

		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesKnight(const Position& position)
	{
		m_ValidMoves.clear();
		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesBishop(const Position& position)
	{
		m_ValidMoves.clear();
		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesQueen(const Position& position)
	{
		m_ValidMoves.clear();
		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesKing(const Position& position)
	{
		m_ValidMoves.clear();
		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesRook(const Position& position)
	{
		m_ValidMoves.clear();

		CheckObstacleHorizontally(m_ValidMoves, position, 8, true, true);
		CheckObstacleVertically(m_ValidMoves, position, 8, true, true);

		return m_ValidMoves;
	}

}

#pragma once

#include <array>
#include <cstdint>

namespace Chess
{
	enum PieceType
	{
		PieceType_Pawn = 0,
		PieceType_Knight,
		PieceType_Bishop,
		PieceType_Queen,
		PieceType_Rook,
		PieceType_King,
		PieceType_Count,
		PieceType_Blank,
	};

	enum PieceColor
	{
		PieceColor_Black = 0,
		PieceColor_White
	};

	struct Piece
	{
		PieceType type{ PieceType_Blank };
		PieceColor color{ PieceColor_White };
	};

	struct Position
	{
		uint8_t x;
		uint8_t y;

		bool operator == (const Position& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
	}; 

	struct Move
	{
		Piece piece{ PieceType_Blank }; // Might be useless
		Position currentPosition;
		Position prevPosition;
	};

	using Board = std::array<std::array<Piece, 8>, 8>;

	inline Position FlipBoardPosition(const Position& pos)
	{
		return { (uint8_t)(7 - pos.x), (uint8_t)(7 - pos.y) };
	}

	inline bool IsValidPiece(PieceType type)
	{
		return (type != PieceType_Count) && (type != PieceType_Blank);
	}

	inline PieceColor InvertPieceColor(PieceColor color)
	{
		return color == PieceColor_White ? PieceColor_Black : PieceColor_White;
	}
}
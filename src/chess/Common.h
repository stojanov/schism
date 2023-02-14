#pragma once

#include <array>

namespace Chess
{
	enum PieceType: uint8_t
	{
		PieceType_Pawn = 0,
		PieceType_LeftKnight,
		PieceType_RightKnight,
		PieceType_LeftBishop,
		PieceType_RightBishop,
		PieceType_Queen,
		PieceType_LeftRook,
		PieceType_RightRook,
		PieceType_King,
		PieceType_Blank,
	};

	enum PieceColor: uint8_t
	{
		PieceColor_Black = 0,
		PieceColor_White
	};

	struct Piece
	{
		PieceType type{ PieceType_Blank };
		PieceColor color{ PieceColor_White };
	};

	struct Move
	{
		Piece piece{ PieceType_Blank };
		uint8_t positionX{ 0 };
		uint8_t positionY{ 0 };
	};

	using Board = std::array<std::array<Piece, 8>, 8>;
}
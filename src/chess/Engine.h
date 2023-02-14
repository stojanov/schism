#pragma once

#include <vector>
#include <array>
#include "Common.h"

namespace Chess
{
	class Engine
	{
	public:
		Engine();
		void Reset();
		bool MakeMove(const Move& move);
		const Board& GetBoardState();
	private:
		Board m_Board;
	};
}
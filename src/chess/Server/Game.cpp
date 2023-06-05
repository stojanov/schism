
#include "Game.h"
#include "Messages.h"

namespace Chess::Net
{
	Game::Game(std::weak_ptr<Client> player1, std::weak_ptr<Client> player2, bool player1Turn)
		:
		m_Player1(player1),
		m_Player2(player2),
		m_Player1Turn(player1Turn)
	{

	}

	void Game::AssignId(uint64_t id)
	{
		m_Id = id;
	}

	void Game::MakeMove(const Move& move)
	{
		if (!m_GameState.MakeMove(move))
		{
			SC_CORE_WARN("(Server/Game) Got invalid move");
			return;
		}
		GameMove gameMove{ m_Id, move };
		auto gameMovePacked = msgpack::pack(gameMove);
		PrependMessageType(gameMovePacked, GAME_MOVE);

		/*
			Find a way to signal the client that they made successful move
		*/
		if (m_Player1Turn)
		{
			auto p = m_Player2.lock();
			if (p) // figure what should happen when invalid pointer
			{
				p->Write(std::move(gameMovePacked));
			}
		}
		else
		{
			auto p = m_Player1.lock();
			if (p)
			{
				p->Write(std::move(gameMovePacked));
			}
		}

		m_Player1Turn != m_Player1Turn;
	}
}
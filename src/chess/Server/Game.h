#pragma once

#include "Client.h"
#include "chess/Engine.h"

namespace Chess::Net
{
    class Game
    {
    public:
        explicit Game(std::weak_ptr<Client> player1, std::weak_ptr<Client> player2, bool player1Turn);

        void AssignId(uint64_t id);

        void DisconnectPlayer(uint64_t player)
        {
            // todo
        }
        void MakeMove(const Move& m);

    private:
        Engine m_GameState;
        bool m_Player1Turn;
        uint64_t m_Id;
        std::weak_ptr<Client> m_Player1;
        std::weak_ptr<Client> m_Player2;
    };
}

#include "Server.h"
#include <random>

namespace Chess::Net
{
    Server::Server(short port) :
        m_Acceptor(m_Context,asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
        Accept();
    }

    Server::~Server()
    {
        Stop();
    }

    void Server::Accept()
    {
        m_Acceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket soc)
        {
            if (!ec)
            {
                auto client = std::make_shared<Client>(std::move(soc));

                auto id = reinterpret_cast<uint64_t>(client.get()); // hacky way to get a unique id
                SC_CORE_INFO("Connected client {}", id);
                client->AssignId(id);

                m_Clients[id] = client;
                std::weak_ptr<Client> clientWeakPtr = client;
                client->AttachReadCallback([this, client = std::move(clientWeakPtr)](std::vector<uint8_t>& buffer, std::size_t length)
                                           {
                                                HandleClientRead(client, buffer, length);
                                           });

                Accept();
            }
            else
            {
                SC_CORE_ERROR("Error accepting client: {}", ec.message());
            }
        });
    }
    void Server::Stop()
    {
        if (!m_Context.stopped())
        {
            m_Context.stop();
        }
    }

    void Server::Start()
    {
        try
        {
            m_Context.run();
        }
        catch (std::exception& e)
        {
            SC_CORE_ERROR("Error running server: {}", e.what());
        }
    }

    void Server::HandleClientRead(std::weak_ptr<Client> client, std::vector<uint8_t>& readBuffer, std::size_t length)
    {
        if (readBuffer.empty())
        {
            return; // log out error
        }

        switch (readBuffer[0])
        {
        case MessageType::REQUEST_GAME:
        {
            auto c = client.lock();

            if (!c)
            {
                // figure out what should happen if pointer invalid
                return;
            }

            CreateGame(c);
            break;
        }
        case MessageType::GAME_MOVE:
        {
            auto gameMove = msgpack::unpack<GameMove>(&readBuffer[1], length - 1);

            if (auto gamePair = m_ActiveGames.find(gameMove.gameId); gamePair != m_ActiveGames.end())
            {
                gamePair->second->MakeMove(gameMove.move);
            }
            else
            {
                SC_CORE_ERROR("Client, requested game with an invalid game id");
            }

            break;
        }
        default:
            break; // log out error
        }
    }

    void Server::CreateGame(const std::shared_ptr<Client>& clientThatStarted)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution dist(0, 100);

        std::shared_ptr<Client> otherPlayer = nullptr;

        auto it = m_Clients.begin();
        do // filter only players that are waiting to join a game
        {
            otherPlayer = it->second;
            it = std::next(it);
        } while (otherPlayer == clientThatStarted);

        
        bool player1IsWhite = dist(gen) > 50;
        
        auto game = std::make_shared<Game>(clientThatStarted, otherPlayer, player1IsWhite);
        auto id = reinterpret_cast<uint64_t>(game.get());

        game->AssignId(id);

        m_ActiveGames[id] = game;

        EnterGame enterGame;
        enterGame.gameId = id;
        enterGame.otherPlayerId = otherPlayer->Id();
        enterGame.isWhite = player1IsWhite;

        {
            auto enterGamePacked = msgpack::pack(enterGame);
            PrependMessageType(enterGamePacked, MessageType::ENTER_GAME);

            clientThatStarted->Write(std::move(enterGamePacked));
        }

        enterGame.otherPlayerId = clientThatStarted->Id();
        enterGame.isWhite = !player1IsWhite;

        {
            auto enterGamePacked = msgpack::pack(enterGame);
            PrependMessageType(enterGamePacked, MessageType::ENTER_GAME);

            clientThatStarted->Write(std::move(enterGamePacked));
        }

        // send player messages

    }
}
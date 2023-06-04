#include "Server.h"


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
                //auto client = std::make_shared<Client>(std::move(s));
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

    void Server::HandleClientRead(std::weak_ptr<Client> client, std::vector<uint8_t>& readBuffer, std::size_t length)
    {
        if (readBuffer.empty())
        {
            return; // log out error
        }

        switch (readBuffer[0])
        {
            case MessageType::MOVE:
            {
                auto move = msgpack::unpack<Move>(&readBuffer[1], length - 1);

                SC_CORE_INFO("Move: currentPosition x:{} y:{}, pieceType: {}, pieceColor: {}",
                             move.currentPosition.x, move.currentPosition.y, move.piece.type, move.piece.color);

                for (auto& i : m_Clients)
                {
                    auto c = i.second;
                    auto myClient = client.lock();
                    if (!myClient)
                    {
                        return;
                    }
                    if (i.second->Id() == myClient->Id())
                    {
                        continue;
                    }

                    auto movePacked = msgpack::pack(move);
                    movePacked.insert(movePacked.begin(), MessageType::MOVE);

                    c->Write(std::move(movePacked));
                }
                break;
            }
            default:
                break; // log out error
        }
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
}
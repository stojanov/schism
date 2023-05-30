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
        asio::ip::tcp::socket soc(m_Context);

        m_Acceptor.async_accept(soc, [&soc, this](std::error_code ec)
        {
            if (!ec)
            {
                //auto client = std::make_shared<Client>(std::move(s));
                auto client = std::make_shared<Client>(std::move(soc));

                auto id = reinterpret_cast<uint64_t>(client.get()); // hacky way to get a unique id

                client->AssignId(id);

                m_Clients[id] = client;
                std::weak_ptr<Client> clientWeakPtr = client;
                client->AttachReadCallback([this, client = std::move(clientWeakPtr)](std::vector<uint8_t>& buffer, std::size_t length)
                                           {
                                                HandleClientRead(client, buffer, length);
                                           });

                Accept();
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
                    auto c = client.lock();
                    if (!c)
                    {
                        return;
                    }
                    if (i.second->Id() == c->Id())
                    {
                        continue;
                    }


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
        m_Context.run();
    }
}
#include "NetServer.h"


namespace Chess::Net
{
    NetServer::NetServer(short port) :
        soc(m_Context),
        m_Acceptor(m_Context,asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
        Accept();
    }

    NetServer::~NetServer()
    {
        Stop();
    }

    void NetServer::Accept()
    {
        m_Acceptor.async_accept(soc, [this](std::error_code ec)
        {
            if (!ec)
            {
                auto client = std::make_shared<Client>(std::move(soc));
                m_Clients[m_ClientIds++] = client; // client id very naive
                std::weak_ptr<Client> clientWeakPtr = client;
                client->AttachReadCallback([this, client = std::move(clientWeakPtr)](std::vector<uint8_t>& buffer, std::size_t length)
                                           {
                                                HandleClientRead(client, buffer, length);
                                           });

                Accept();
            }
        });
    }

    void NetServer::HandleClientRead(std::weak_ptr<Client> client, std::vector<uint8_t>& readBuffer, std::size_t length)
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
                break;
            }
            default:
                break; // log out error
        }
    }

    void NetServer::Stop()
    {
        if (!m_Context.stopped())
        {
            m_Context.stop();
        }
    }

    void NetServer::Start()
    {
        m_Context.run();
    }
}
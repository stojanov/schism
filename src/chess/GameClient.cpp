#include "GameClient.h"

#include "Common.h"
#include "Server/Messages.h"

namespace Chess
{
    GameClient::GameClient(const std::string& host, const std::string& port, Schism::GameEvent::CallbackBus& gameEventBus)
        :
        m_GameEventBus{ gameEventBus },
        m_Socket(m_IoContext)
    {
        m_ReadBuffer.reserve(Net::MAX_MESSAGE_BUFFER_LENGTH);

        asio::ip::tcp::resolver resolver(m_IoContext);

        auto endpoints = resolver.resolve(host, port);

        asio::async_connect(m_Socket, endpoints,
                            [this](const asio::error_code& error, const asio::ip::tcp::endpoint& endpoint)
                            {
                                if (error)
                                {
                                    // log out error
                                }
                                ReadWork();
                            });

        ListenGameEvent<Move>([this](Move&& m)
                              {
                                    auto packed = msgpack::pack(m);
                                    packed.insert(packed.begin(), Net::MessageType::MOVE);
                                    m_Socket.write_some(asio::buffer(packed));
                              });
    }

    void GameClient::PollEvents()
    {
        ProcessGameEvents();
    }

    void GameClient::Stop()
    {
        m_IoContext.stop();
    }

    void GameClient::Start()
    {
        m_IoContext.run();
    }

    void GameClient::ReadWork()
    {
        m_Socket.async_read_some(
                asio::buffer(m_ReadBuffer, Net::MAX_MESSAGE_BUFFER_LENGTH),
                [this](std::error_code ec, size_t length)
                {
                    if (ec)
                    {
                        // log out error
                    }
                    if (length < 2)
                    {
                        SC_CORE_ERROR("Message not big enough");
                    }

                    switch (m_ReadBuffer[0])
                    {
                        case Net::MessageType::MOVE:
                        {
                            Move m = msgpack::unpack<Move>(&m_ReadBuffer[1], length - 1);
                            m_GameEventBus.PostEvent(m);
                            break;
                        }
                        default:
                            SC_CORE_ERROR("Unknown message Type");
                    }

                    ReadWork();
                });
    }
}
#include "NetClient.h"

#include <iostream>
#include <msgpack/msgpack.hpp>
#include "chess/Common.h"
namespace Chess
{
    NetClient::NetClient(asio::ip::tcp::socket soc):
        m_Soc(std::move(soc)),
        m_IsStopped(false)
    {
        m_ReadBuffer.resize(1024);
        ReadWork();
    }

    NetClient::~NetClient()
    {
        Stop();
    }

    void NetClient::Stop()
    {
        m_IsStopped = true;
        m_Soc.cancel();
    }

    void NetClient::ReadWork()
    {
        m_Soc.async_read_some(asio::buffer(m_ReadBuffer, 1024),
                              [this](std::error_code ec, size_t length)
                              {
                                    if (!ec) // handle multiple errors
                                    {
                                        HandleRead(length);

                                        ReadWork();
                                    }
                              });
    }

    void NetClient::HandleRead(size_t length)
    {
        std::cout << "GOT MESSAGE " << length << " ";
        for (auto c : m_ReadBuffer)
        {
            std::cout << c;
        }
        std::cout << "\n";

        auto m2 = msgpack::unpack<Chess::Move>(m_ReadBuffer);

        SC_CORE_INFO("prev: {} {}, current: {} {}, piece color {}, piecetype {}",
                     m2.prevPosition.x, m2.prevPosition.y,
                     m2.currentPosition.x, m2.currentPosition.y,
                     m2.piece.color, m2.piece.type);

    }

    void NetClient::HandleWrite()
    {

    }
}
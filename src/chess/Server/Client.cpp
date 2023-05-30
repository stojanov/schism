#include "NetClient.h"

#include <iostream>
#include <msgpack/msgpack.hpp>
#include <utility>
#include "chess/Common.h"

namespace Chess::Net
{
    NetClient::NetClient(asio::ip::tcp::socket soc):
        m_Soc(std::move(soc))
    {
        m_ReadBuffer.resize(MAX_BUFFER_LENGTH);
        ReadWork();
    }

    NetClient::~NetClient()
    {
        Stop();
    }

    void NetClient::Stop()
    {
        m_Soc.cancel();
    }

    void NetClient::AttachReadCallback(ReadCallback&& readCallback)
    {
        m_ReadCallback = std::move(readCallback);
    }

    void NetClient::ReadWork()
    {
        m_Soc.async_read_some(asio::buffer(m_ReadBuffer, MAX_BUFFER_LENGTH),
                              [this](std::error_code ec, size_t length)
                              {
                                    if (!ec) // TODO: handle multiple errors
                                    {
                                        if (m_ReadCallback)
                                        {
                                            m_ReadCallback(m_ReadBuffer, length);
                                        }

                                        ReadWork();
                                    }
                                    else
                                    {
                                        SC_CORE_WARN("Error on read connection!");
                                    }
                              });
    }
}
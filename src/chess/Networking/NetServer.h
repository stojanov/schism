#pragma once

#include "NetClient.h"
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <phmap.h>
#include <thread>
#include <vector>
#include <memory>

namespace Chess
{
    class NetServer
    {
    public:
        NetServer(short port);
        ~NetServer();

        void Start(); // Will spawn a new network thread
        void Stop();
    private:
        void Accept();
        asio::io_context m_Context;
        asio::ip::tcp::socket soc;
        asio::ip::tcp::acceptor m_Acceptor;
        bool m_IsStarted;
        std::vector<std::shared_ptr<NetClient>> m_Clients;
    };
}
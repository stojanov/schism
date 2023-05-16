#pragma once


#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <phmap.h>
#include <thread>
#include <vector>
#include <memory>
#include "NetClient.h"
#include "NetGame.h"
#include "NetMessageType.h"

namespace Chess
{
    class NetServer
    {
    public:
        explicit NetServer(short port);
        ~NetServer();

        void Start(); // Will spawn a new network thread
        void Stop();
    private:
        void Accept();

        void HandleClientDisconnect();
        void HandleClientRead(std::weak_ptr<NetClient> client, std::vector<uint8_t>& readBuffer, std::size_t length);

        std::uint32_t m_ClientIds; // good enough for now, use a database in the future or guid
        std::uint32_t m_GameIds;
        asio::io_context m_Context;
        asio::ip::tcp::socket soc;
        asio::ip::tcp::acceptor m_Acceptor;
        phmap::parallel_flat_hash_map<std::uint32_t, std::shared_ptr<NetClient>> m_Clients;
        phmap::parallel_flat_hash_map<std::uint32_t, std::shared_ptr<NetGame>> m_ActiveGames;
    };
}
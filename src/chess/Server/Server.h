#pragma once


#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <phmap.h>
#include <thread>
#include <vector>
#include <memory>
#include "Client.h"
#include "NetGame.h"
#include "Messages.h"

namespace Chess::Net
{
    class Server
    {
    public:
        explicit Server(short port);
        ~Server();

        void Start(); // Will spawn a new network thread
        void Stop();
    private:
        void Accept();

        void HandleClientDisconnect();
        void HandleClientRead(std::weak_ptr<Client> client, std::vector<uint8_t>& readBuffer, std::size_t length);

        asio::io_context m_Context;
        asio::ip::tcp::acceptor m_Acceptor;
        phmap::parallel_flat_hash_map<std::uint32_t, std::shared_ptr<Client>> m_Clients;
        phmap::parallel_flat_hash_map<std::uint32_t, std::shared_ptr<NetGame>> m_ActiveGames;
    };
}
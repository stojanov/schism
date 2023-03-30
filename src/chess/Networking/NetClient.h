#pragma once

#include <vector>
#include <asio/ts/internet.hpp>
#include <asio/buffer.hpp>
#include <schism/Game/GameEvent/CallbackListener.h>

namespace Chess
{
    class NetClient
    {
    public:
        NetClient(asio::ip::tcp::socket soc);
        ~NetClient();

        void Stop();
        void Write(const std::vector<char>& message);
        void Write(std::vector<char>&& message);
    private:
        void HandleRead(size_t length);
        void ReadWork();
        void HandleWrite();

        asio::ip::tcp::socket m_Soc;
        std::vector<uint8_t> m_ReadBuffer;
        bool m_IsStopped;
    };
}
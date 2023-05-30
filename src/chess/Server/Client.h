#pragma once

#include <vector>
#include <asio/ts/internet.hpp>
#include <asio/buffer.hpp>
#include <schism/Game/GameEvent/CallbackListener.h>

namespace Chess::Net
{
    class NetGame;

    class NetClient
    {
        using ReadCallback = std::function<void(std::vector<uint8_t>&, std::size_t lenght)>;
    public:
        explicit NetClient(asio::ip::tcp::socket soc);
        ~NetClient();

        void Stop();


        void AttachReadCallback(ReadCallback&& readCallback);

        void Write(const std::vector<char>& message);
        void Write(std::vector<char>&& message);
    private:
        static constexpr size_t MAX_BUFFER_LENGTH = 1024;
        void HandleRead(size_t length);
        void ReadWork();

        asio::ip::tcp::socket m_Soc;
        std::vector<uint8_t> m_ReadBuffer;
        ReadCallback m_ReadCallback;
    };
}
#pragma once

#include <cstdint>
#include "chess/Common.h"
#include "msgpack/msgpack.hpp"
#include <type_traits>
namespace Chess::Net
{

    enum MessageType: std::uint8_t
    {
        LOGIN = 0,
        LOGOUT,
        ENTER_GAME,
        REQUEST_GAME,
        GAME_MOVE,
        COUNT
    };

    template<typename T, typename U>
    concept GameMoveConcept = requires(T && obj, U arg) {
        //{ std::declval<T>().pack(std::declval<typename std::remove_reference<T>::type>()) };
        { obj.pack(msgpack::Packer&) };
        { obj.Type() } -> std::same_as<MessageType>;
    };

    inline void PrependMessageType(std::vector<uint8_t>& vec, MessageType type) noexcept
    {
        vec.insert(vec.begin(), type);
    }

    template<typename T>
    inline T UnpackMessage(const std::vector<uint8_t>& buffer, size_t length)
    {
        return msgpack::unpack<T>(&buffer[1], length - 1);
    }

    template<typename T>
    inline std::vector<uint8_t> PackMessage(T& data, MessageType type)
    {
        auto packed = msgpack::pack(data);
        PrependMessageType(packed, type);
        return std::move(packed);
    }

    struct RequestGame
    {};

    struct EnterGame
    {
        uint64_t gameId;
        uint64_t otherPlayerId; // might be needed
        bool isWhite;

        static constexpr MessageType Type()
        {
            return ENTER_GAME;
        }

        template<typename T>
        void pack(T& p)
        {
            p(gameId, otherPlayerId, isWhite);
        }
    };

    struct GameMove
    {
        uint64_t gameId;
        Move move;

        static constexpr MessageType Type()
        {
            return GAME_MOVE;
        }

        void pack(msgpack::Packer& p)
        {
            p(gameId, move);
        }
    };
}

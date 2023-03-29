#pragma once
#include "msgpack/msgpack.hpp"
namespace Chess
{
    struct ResetGame{};

    struct StartGame
    {
        bool IsWhite{ true };

        template<typename T>
        void pack(T& p)
        {
            p(IsWhite);
        }
    };
}
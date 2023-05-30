//
// Created by angels on 16.5.23.
//

#pragma once

#include <cstdint>

namespace Chess::Net
{
    enum MessageType: std::uint8_t
    {
        LOGIN = 0,
        LOGOUT,
        MOVE,
        COUNT
    };
}

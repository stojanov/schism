#pragma once

#include "asio.hpp"
#include "Common.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"

namespace Chess
{
class GameClient: public Schism::GameEvent::SyncListener
    {
    public:
        GameClient(Schism::GameEvent::CallbackBus& gameEventBus);
    private:
        Schism::GameEvent::CallbackBus& m_GameEventBus;
    };
}
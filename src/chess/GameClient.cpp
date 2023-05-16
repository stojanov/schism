#include "GameClient.h"


namespace Chess
{
    GameClient::GameClient(Schism::GameEvent::CallbackBus &gameEventBus)
        :
        m_GameEventBus{ gameEventBus }
    {

    }
}
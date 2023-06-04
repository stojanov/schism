#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"
#include "msgpack/msgpack.hpp"
#include "chess/Common.h"
#include "chess/ChessEvents.h"
#include "chess/Server/Server.h"
#include "asio.hpp"
#include <thread>


int main(int argc, char* argv[])
{
    Schism::Init();


    bool isServer = false;
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "server")
        {
            Chess::Net::Server server(6666);
            server.Start();
            isServer = true;
        }
    }

    if (isServer)
    {
        return 0;
    }
    Game g;

    g.Run();

    return 0;

}
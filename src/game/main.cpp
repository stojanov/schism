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

struct AddNumber
{
    int a;
    int b;
};

struct DecNumber
{
    int a;
    int b;
};

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

    std::shared_ptr<Schism::GameEvent::CallbackListener> listener = std::make_shared<Schism::GameEvent::CallbackListener>();

    listener->ListenGameEvent<AddNumber>([](AddNumber&& num)
                                        {
                                            SC_CORE_INFO("ADD NUMBER {} {}", num.a, num.b);
                                        });

    Schism::GameEvent::CallbackBus bus;

    bus.AttachListener(listener);

    std::thread t([&]()
                  {
                        int i = 0;
                        while (1)
                        {
                            bus.PostEvent<AddNumber>(AddNumber{ i, i });
                            i++;
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                        }

                  });

    while (1)
    {
        listener->ProcessGameEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds (50));
    }

    return 0;

}
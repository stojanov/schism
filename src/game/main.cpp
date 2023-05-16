#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"
#include "msgpack/msgpack.hpp"
#include "chess/Common.h"
#include "chess/ChessEvents.h"
#include "chess/Server/NetServer.h"
#include "asio.hpp"
#include <thread>

struct AddNumbers
{
    int a;
    int b;
};

struct DecNumbers
{
    int a;
    int b;
};

int main(int argc, char* argv[])
{
    Schism::Init();

    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "server")
        {
            Chess::NetServer server(6666);
            server.Start();
        }
    }

    asio::io_context ctx;
    asio::ip::tcp::socket soc(ctx);
    asio::ip::tcp::resolver resolver(ctx);
    auto endpoints = resolver.resolve("localhost", "6666");
    asio::connect(soc, endpoints);

    auto m = Chess::Move{};
    m.prevPosition = { 1, 2 };
    m.currentPosition = { 2, 3 };
    m.piece = Chess::Piece{};
    m.piece.color = Chess::PieceColor_White;
    m.piece.type = Chess::PieceType_Bishop;

    auto d = msgpack::pack(m);
    std::cout << "SENT " << d.size() << "\n";
    asio::write(soc, asio::buffer(d));

    return 0;

    Game app;

    //app.Run();

    Schism::GameEvent::CallbackBus eventBus;

    std::shared_ptr<Schism::GameEvent::CallbackListener> listener = std::make_shared<Schism::GameEvent::CallbackListener>();
    eventBus.AttachListener(listener);

    listener->RegisterGameEvent<AddNumbers>();
    listener->ListenGameEvent<AddNumbers>([](AddNumbers&& e)
                                          {
                                              std::cout << e.a << " " << e.b << "\n";
                                          });
    std::thread th([&eventBus]()
                   {
                       int a = 0;
                       int b = 0;
                       while (true)
                       {
                           //         eventBus.PostEvent(AddNumbers{ a++, b++ });
                           std::this_thread::sleep_for(std::chrono::milliseconds(500));
                       }
                   });

    /*
	while (true)
	{
        listener->ProcessGameEvent();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
     */
}
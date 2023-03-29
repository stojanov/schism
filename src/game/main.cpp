#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"
#include "msgpack/msgpack.hpp"
#include "chess/Common.h"
#include "chess/ChessEvents.h"

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

    auto m = Chess::Move{};
    m.prevPosition = { 1, 2 };
    m.currentPosition = { 2, 3 };
    m.piece = Chess::Piece{};
    m.piece.color = Chess::PieceColor_White;
    m.piece.type = Chess::PieceType_Bishop;

    auto d = msgpack::pack(m);
    //auto m2 = msgpack::unpack<Chess::Move>(d);
    auto m2 = Chess::Move{};
    SC_CORE_INFO("prev: {} {}, current: {} {}, piece color {}, piecetype {}",
                 m2.prevPosition.x, m2.prevPosition.y,
                 m2.currentPosition.x, m2.currentPosition.y,
                 m2.piece.color, m2.piece.type);
    return 0;
	Schism::Init();
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
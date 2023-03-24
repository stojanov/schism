#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/SyncBus.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"

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
                eventBus.PostEvent(AddNumbers{ a++, b++ });
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
		});
    
	while (true)
	{
        listener->Process<AddNumbers>();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}
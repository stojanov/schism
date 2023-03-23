#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/SyncListener.h"
#include "schism/Game/GameEvent/SyncBus.h"
#include "schism/Game/GameEvent/CallbackListener.h"
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

class MyListener:
    public Schism::GameEvent::SyncListener,
	public std::enable_shared_from_this<MyListener>
{
public:
	void Init()
	{
		//RegisterGameEvent<AddNumbers>();
		RegisterGameEvent<DecNumbers>();
	}

    void OnEvent(AddNumbers&& e)
    {

    }


	std::shared_ptr<MyListener> PTR()
	{
		return shared_from_this();
	}
};

int main(int argc, char* argv[])
{
	Schism::Init();
	Game app;
	
	//app.Run();

	Schism::GameEvent::SyncBus eventBus;

	std::shared_ptr<MyListener> listener = std::make_shared<MyListener>();
    Schism::GameEvent::CallbackListener cbListener;
	eventBus.AttachListener(listener->PTR());

	std::thread th([&listener]()
		{
			listener->Init();

			while (true)
			{
			}
		});

	int a = 0;
	int b = 0;
	while (true)
	{

		eventBus.PostEvent(AddNumbers{ a++, b++ });
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
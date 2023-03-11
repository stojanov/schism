#include "schism/Schism.h"
#include "Game.h"
#include "schism/Game/GameEvent/Bus.h"


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
	public Schism::GameEvent::Listener,
	public std::enable_shared_from_this<MyListener>
{
public:
	void Init()
	{
		RegisterGameEvent<AddNumbers>();
		RegisterGameEvent<DecNumbers>();
	}

	void Run()
	{
		auto e = ListenGameEvent<AddNumbers>();

		std::cout << "Got event a: " << e.a << " " << e.b << "\n";
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

	Schism::GameEvent::Bus eventBus;

	std::shared_ptr<MyListener> listener = std::make_shared<MyListener>();

	eventBus.AttachListener(listener->PTR());

	std::thread th([&listener]()
		{
			listener->Init();

			while (true)
			{
				listener->Run();
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
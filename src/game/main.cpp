#include "Schism.h"

class Sandbox : public Schism::Application
{
public:
	Sandbox()
	{
		
	}
	~Sandbox()
	{
	}
};

int main()
{ 
	auto app = Schism::CreateApplication();
	app->Run();
	delete app;
}

Schism::Application* Schism::CreateApplication()
{
	return new Sandbox();
}
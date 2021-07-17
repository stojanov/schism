#include "Schism.h"
#include "schism/System/Ptr.h"

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
}

std::unique_ptr<Schism::Application> Schism::CreateApplication()
{
	return MakePtr<Sandbox>();
}
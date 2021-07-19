#include "schism/Schism.h"

int main(int argc, char* argv[])
{
	Schism::Init();
	Schism::Sandbox App;
	
	App.Run();
}
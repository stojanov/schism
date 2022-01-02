#include "schism/Schism.h"
#include "Game.h"

int main(int argc, char* argv[])
{
	Schism::Init();
	Game app;
	
	app.Run();
}
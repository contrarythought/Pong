#include<iostream>
#include"game.h"

int main(int argc, char* argv[])
{
	Game game;

	bool success = game.initialize();

	if (success)
	{
		game.RunLoop();
	}

	game.Shutdown();


	return 0;
}
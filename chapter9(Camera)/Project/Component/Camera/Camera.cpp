#include "PreCompiled.hpp"

int main(int argc, char** argv)
{
	Game game;
	bool success = game.Initialize();

	while (success)
	{
		game.RunLoop();
	}

	game.ShutDown();
	return 0;
}
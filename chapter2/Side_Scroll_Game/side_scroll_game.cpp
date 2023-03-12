#include "Game.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	// Game Ŭ���� �ν��Ͻ� ����
	Game game;
	bool success = game.Initialize();

	// �ʱ�ȭ �����ϸ� ���� ����
	if (success) {
		game.RunLoop();
	}

	game.ShutDown();
	return 0;
}
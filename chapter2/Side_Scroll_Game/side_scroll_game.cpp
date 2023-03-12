#include "Game.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	// Game 클래스 인스턴스 생성
	Game game;
	bool success = game.Initialize();

	// 초기화 성공하면 게임 실행
	if (success) {
		game.RunLoop();
	}

	game.ShutDown();
	return 0;
}
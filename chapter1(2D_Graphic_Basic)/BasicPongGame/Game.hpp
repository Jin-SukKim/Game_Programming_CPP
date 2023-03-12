#pragma once
#include <SDL.h> // SDL_Windows 포인터 참조

struct Vector2 {
	float x;
	float y;
};

class Game {
private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput(); // 유저의 input
	void UpdateGame();	// input을 바탕으로 로직등을 업데이트
	void GenerateOutput(); // 바뀐 씬을 랜더링

	// SDL이 생성한 윈도우
	SDL_Window* mWindow;
	// 게임이 계쏙 실행되야 하는지를 판단
	bool mIsRunning;
	// 그래픽을 그리기 위한 렌더러
	SDL_Renderer* mRenderer;

	// 패들
	Vector2 mPaddlePos;
	// 공
	Vector2 mBallPos;

	// 델타 타임
	Uint32 mTicksCount;
	// direction of paddle
	int mPaddleDir;
	// 공의 속도
	Vector2 mBallVel;
public:
	// constructor
	Game();
	// 게임 초기화
	bool Initialize();
	// 게임이 끝나기 전까지 게임 루프를 실행
	void RunLoop();
	// 게임 종료
	void ShutDown();
};
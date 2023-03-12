#include "Game.hpp"

const int thickness = 15;
const float paddleH = 100.f;
// constructor
Game::Game() : mWindow(nullptr), mIsRunning(true), mTicksCount(0), mPaddleDir(0) {};

// 초기화를 성공하면 true 아니면 false return
bool Game::Initialize() {
	/*	SDL_Init 함수를 사용해 SDL 라이브러리를 초기화
		이 함수는 초기화를 하려는 모든 서브시스템의
		비트 or 연산값을 파라미터로 받는다.
		(Audio, Video, Haptic(force feedback), Gamecontrollor etc)

		정수값을 return, 0이 아니면 초기화가 실패했다는 것이다.
	*/ 
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); // 비디오 서브시스템만 초기화

	// 0이 아니면 초기화 실패로 false return
	if (sdlResult != 0) {
		// console에 에러 메시지 출력
		// SDL_Log는 C printf 함수와 같은 문법 사용
		SDL_Log("Unable to initialize SLD : %s", SDL_GetError());
		return false;
	}

	/*	윈도우 생성
		파라미터 종류	 :
			제목, 배치될 x/y 좌표(왼쪽 상단 구석 기준),
			너비와 높이, 생성 관련 플래그) 

		플래그 종류 : 전체화면, 현재 모니터 해상도로 전체화면, opengl 라이브러리 지원추가, 크기 조절
	*/
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1) - pong game",	// 윈도우 제목
		100,	// 윈도우 좌측 상단 x좌표
		100,	// 윈도우 좌측 상단 y좌표
		1024,	// 윈도우 너비
		768,	// 윈도우 높이
		0		// 플래그(0은 플래그 설정 X
	);

	// 윈도우 생성을 실패하면 nullptr return
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	/*	파라미터: 
	*	윈도우의 포인터, 
	*	어떤 그래픽 카드를 사용할지(-1은 default로 SDL이 선택)
	*	초기화 플래그(가속화된 렌더러(그래픽 하드웨어 활용), 수직 동기화 활성화 여부)
	*/
	mRenderer = SDL_CreateRenderer(
		mWindow,	// 렌더링을 위해 생성한 윈도우
		-1,			// 일반적으로 -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// 패들 첫 위치
	mPaddlePos.x = 15.f;
	mPaddlePos.y = 768.0f / 2.0f;

	// 공 첫 위치
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;

	// 공의 속도 지정
	mBallVel.x = -200.f;
	mBallVel.y = 235.f;

	// SDL 초기화와 윈도우 생성
	return true;
}

// 게임 종료를 위해 SDL을 닫는다.
void Game::ShutDown() {
	// 렌더러 해제
	SDL_DestroyRenderer(mRenderer);
	// SDL_Window 객체를 해제한다.
	SDL_DestroyWindow(mWindow);
	// SDL을 닫는다.
	SDL_Quit();
}

void Game::ProcessInput() {
	
	SDL_Event event; 

	// 이벤트 큐는 트겆ㅇ 프레임에서 여러 이벤트를 포함할 수 있어
	// 모든 이벤트를 조회하며 다뤄야한다.
	// 이벤트 큐에서 이벤트를 발견하면 true return
	while (SDL_PollEvent(&event)) {
		// 여러 유형의 이벤트 다루기
		switch (event.type) {
		case SDL_QUIT: // 윈도우 X 클릭 or 단축키 사용해 윈도우 닫기
			mIsRunning = false;
			break;
		}
	}

	// 키보드의 상태 얻기
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// 이스케이프 키를 눌렀다면 루프 종료
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// 패들의 위치 갱신, 동시 입력 시 0
	mPaddleDir = 0;

	// w 입력시 위로 움직인다(-1, 음수y)
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}

	// s 입력시 밑으로 움직인다(+1, 양수y)
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
	}
}

void Game::UpdateGame() {
	// 목표 프레임인 60FPS를 충족하기 위해
	// 프레임 당 delta time 16.6ms를 충족해야 된다.
	// 적어도 프레임 간 16.ms가 경과함을 보장한다.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// delta time은 현재 프레임 틱값과 이전 프레임 틱값의 차다.
	// (차(ms)를 초 단위로 변환)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// 최대 델타 시간값으로 고정
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// 다음 프레임을 위해 틱값을 갱신
	// SDL_GetTicks() = SDL_Init 함수 호축 이후 경과된 시간(ms) 반환
	mTicksCount = SDL_GetTicks();

	// 델타 시간 함수로 게임 세계의 오브젝트 갱신
	if (mPaddleDir != 0) {
		// 패들의 방향과 초당 300.f 픽셀의 속도, 델타 시간으로 y위치 갱신
		mPaddlePos.y += mPaddleDir * 300.f * deltaTime;

		// 패들이 화면 영역을 벗어나는지 검증
		if (mPaddlePos.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos.y = paddleH / 2.f + thickness;
		}

		if (mPaddlePos.y > (768.f - paddleH / 2.f - thickness)) {
			mPaddlePos.y = 768.f - paddleH / 2.f - thickness;
		}
	}

	// 볼의 속도로 볼의 위치 갱신
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// 공이 상단 벽과 충돌하면 y값 반전
	// 프레임이 바뀔 때 공이 여전히 벽과 충돌해
	// 무한히 공을 반전 시킬수 있으므로 y 속도를 측정해준다.
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1.f;
	}

	// 공과 하단 벽 충돌
	if (mBallPos.y >= (768.f - thickness) && mBallVel.y > 0.0f) {
		mBallVel.y *= -1.f;
	}

	// 공과 우측 벽 충돌
	if (mBallPos.x >= (1024 - thickness) && mBallVel.x > 0.0f) {
		mBallVel.x *= -1.f;
	}

	// 패들과 공의 y값의 차이
	float diff = mPaddlePos.y - mBallPos.y;
	// 절대값
	diff = diff > 0.f ? diff : -diff;

	// 공과 패들의 충돌
	if (
		// 패들과 공의 y차가 충분히 작고
		diff <= (paddleH / 2.f) &&
		// 공이 올바른 x 값을 갖고 있고
		mBallPos.x <= 25.f && mBallPos.x >= 20.f &&
		// 공이 왼쪽으로 이동 중이라면
		mBallVel.x < 0.f) 
	{
		mBallVel.x *= -1.f;
	}

	// 공이 화면 밖으로 나가면 게임이 끝난다.
	if (mBallPos.x <= 0.f) {
		mIsRunning = false;
	}
}

void Game::GenerateOutput() {
	// 렌더러를 단색으로 지정
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A
	);

	// back buffer를 단색으로 클리어
	SDL_RenderClear(mRenderer);

	// 전체 게임 장면 그리기

	// 색상을 흰색으로 변경
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// 사각형 설정
	SDL_Rect wall{
		// 사각형의 스크린 좌표계 위치
		0,			// 왼쪽 상단 x
		0,			// 왼쪽 상단 y
		1024,		// 너비
		thickness	// 높이
	};

	// SDL_Rect 포인터를 받는 함수로
	// 사각형 그리기
	SDL_RenderFillRect(mRenderer, &wall);

	// bottom wall 그리기
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// right wall 그리기
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	// paddle 그리기
	SDL_Rect paddle{
		// float에서 SDL_Rect가 사용하는 int로 변환
		static_cast<int>(mPaddlePos.x - thickness/2.f),
		static_cast<int>(mPaddlePos.y - paddleH/2.f),
		thickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// ball 그리기
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2.f),
		static_cast<int>(mBallPos.y - thickness / 2.f),
		thickness,
		thickness

	};
	SDL_RenderFillRect(mRenderer, &ball);

	// front buffer와 back buffer 교환
	SDL_RenderPresent(mRenderer);
}

// 게임 루프를 반복해서 실행
void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}


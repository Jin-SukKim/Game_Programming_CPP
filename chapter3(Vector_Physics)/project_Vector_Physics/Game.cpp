#include "PreCompiled.hpp"

const int thickness = 15;
const float paddleH = 100.f;
// constructor
Game::Game() : mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false) {};

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

	sdlResult = IMG_Init(IMG_INIT_PNG); // 이미지 초기화
	if (sdlResult == 0) {
		SDL_Log("Unable to initialize IMG : %s", SDL_GetError());
		return false;
	}

	/*	윈도우 생성
		파라미터 종류	 :
			제목, 배치될 x/y 좌표(왼쪽 상단 구석 기준),
			너비와 높이, 생성 관련 플래그)

		플래그 종류 : 전체화면, 현재 모니터 해상도로 전체화면, opengl 라이브러리 지원추가, 크기 조절
	*/
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2) - Side Scroll Game",	// 윈도우 제목
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

	LoadData();
	mTicksCount = SDL_GetTicks();
	// SDL 초기화와 윈도우 생성
	return true;
}

// 게임 종료를 위해 SDL을 닫는다.
void Game::ShutDown() {
	UnLoadData();
	IMG_Quit();
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

	// Ship 입력
	mShip->ProcessKeyboard(state);

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

	// 모든 Actor를 갱신
	mUpdatingActors = true;
	for (auto actor : mActors) {
		// actor의 component와 actor를 갱신
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 대기 중인 Actor를 mActors로 이동
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	// vector의 값들을 지우지만 메모리는 할당되어있다.
	mPendingActors.clear();

	// 죽은 Actor를 임시 vector에 추가
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// 죽은 엑터 제거(mActors에서 추려낸 Actor들)
	for (auto actor : deadActors) {
		delete actor;
	}


}

void Game::GenerateOutput() {
	// 렌더러를 단색으로 지정
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		0,		// B
		255		// A
	);

	// back buffer를 단색으로 클리어
	SDL_RenderClear(mRenderer);

	// 전체 게임 장면 그리기

	// 스프라이트 컴포넌트 전부 그려주기
	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}

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


// Actor를 반복하는 동안 새 Actor를 생성하는 경우를 다루려면 대기 Actor들을 위한 벡터가 필요하다
// mActor가 반복되고 있으므로 mActor에 요소를 추가하면 안되고 mPendingActors에 요소롤 추가해
// mActor의 반복이 끝나면 mActor로 대기중인 Actor를 이동시킨다.
void Game::AddActor(Actor* actor) {
	// Actor를 갱신 중이라면 대기 벡터에 Actor 추가
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

// 두 vector에서 Actor를 제거한다.
void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// vector의 마지막 값하고 바꿔준다.
		std::iter_swap(iter, mPendingActors.end() - 1);
		// 마지막 element를 제거하면 erase copy를 방지할 수 있다.
		mPendingActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

SDL_Texture* Game::LoadTexture(const char* fileName) {
	// 파일로부터 이미지 로딩
	// 성공하면 포인터를 return하고 실패하면 nullptr return
	SDL_Surface* surf = IMG_Load(fileName);
	if (!surf) {
		SDL_Log("Failed to load texture file %s", fileName);
		return nullptr;
	}

	// 텍스쳐 생성
	// SDL_Surface를 SDL이 화면에 그리는데 필요한 포맷인 SDL_Texture로 변환
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf); // 사용할 렌더러, 변환된 SDL_Surface
	SDL_FreeSurface(surf); // SDL_Surface 초기화
	if (!tex) {
		SDL_Log("Failed to convert surface to texture for %s", fileName);
		return nullptr;
	}

	return tex;
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	// 불러올 Texture가 이미 맵에 존재하느지 확인
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		// 맵에 존재한다면 SDL_Texture에 바로 return
		tex = iter->second;
	}
	// 맵에 없다면 텍스쳐를 로드한다.
	else {
		tex = LoadTexture(fileName.c_str());
		// Load한 Texture를 맵에 넣어준다.
		mTextures.emplace(fileName, tex);
	}
	return tex;
}

// Game의 모든 Actor를 생성한다.
void Game::LoadData() {
	// Player's ship 만들기
	mShip = new Ship(this);
	mShip->SetPosition(Vector2d(100.f, 384.f));
	mShip->SetScale(1.5f);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}


void Game::UnLoadData() {
	// ~Actor 함수가 RemoveActor를 호출하므로 다른 스타일의 루프를 사용
	// actor 포인터 메모리 삭제
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// Texture 삭제
	for (auto i : mTextures) {
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();

}

void Game::AddSprite(SpriteComponent* sprite) {
	// 정렬된 벡터에서 삽입해야 할 위치를 찾는다.
	// 자신보다 그리기 순서값이 큰 최초 요소
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// 반복자 위치 앞에 요소를 삽입한다.
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	// swap은 순서를 망가트리므로 하지 못한다.
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
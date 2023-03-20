#include "PreCompiled.hpp"

// constructor
Game::Game() : mWindow(nullptr), mSpriteShader(nullptr),
mIsRunning(true), mUpdatingActors(false) {};

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

	/*
		OpenGL 윈도우 생성하기 전 OpenGl의 버전이나 색상 깊이,
		파라미터들에 대한 속성 설정이 가능하다.
		OpenGL 윈도우 속성 설정(윈도우를 생성하기 전에 사용할 것)
		함수 호출이 성공하면 0을 반환하고 실패하면 음수를 return한다.
	
	SDL_GL_SetAttribute(
		SDL_GLattr attr,	// 설정할 속성
		int value			// 해당 속성의 값
	);

	*/
	// 코어 OpenGL 프로파일 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// OpenGL 3.3 버전으로 지정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA 채널마다 8비트 크기인 색상 버터 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// 더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL이 하드웨어 가속을 사용하도록 강제
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	/*	윈도우 생성
		파라미터 종류	 :
			제목, 배치될 x/y 좌표(왼쪽 상단 구석 기준),
			너비와 높이, 생성 관련 플래그)

		플래그 종류 : 전체화면, 현재 모니터 해상도로 전체화면, opengl 라이브러리 지원추가, 크기 조절
	*/
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 5) - OpenGL",	// 윈도우 제목
		100,	// 윈도우 좌측 상단 x좌표
		100,	// 윈도우 좌측 상단 y좌표
		1024,	// 윈도우 너비
		768,	// 윈도우 높이
		SDL_WINDOW_OPENGL	// OpenGL을 사용
	);

	// 윈도우 생성을 실패하면 nullptr return
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// 생성한 윈도우를 OpenGL의 Context에 지정
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW 초기화
	glewExperimental = GL_TRUE;	// 일부 플랫폼에서 코어 콘텍스트 사용시 발생할 수도 있는 초기화 에러를 막는다.
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	// 일부 플랫폼에서 GLEW는 에러 코드를 내보낸다.
	// 그러므로 에러값을 제거
	glGetError();
	
	// 셰이더
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// 정점 배열 개체 생성
	InitSpriteVerts();

	LoadData();

	mTicksCount = SDL_GetTicks();
	// SDL 초기화와 윈도우 생성
	return true;
}

// 게임 종료를 위해 SDL을 닫는다.
void Game::ShutDown() {
	UnLoadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	// SDL Context 해제
	SDL_GL_DeleteContext(mContext);
	// SDL_Window 객체를 해제한다.
	SDL_DestroyWindow(mWindow);
	// SDL을 닫는다.
	SDL_Quit();
}

// ProcessInput 내부에서 Actor가 또다른 Actor를 만들거나
// Component를 다루는 loop 앞에서는 mUpdatingActors bool 값이 True로 설정돼야 한다.
// 새로운 Actor를 추가할 때는 mActors 대신 mPendingActors에 추가해야 한다.
// 벡터가 반복되는 동안 mActors가 수정되지 않도록해야한다.
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

	// 모든 Actor를 반복하면서 각 Actor의 ProcessInput을 호출
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;

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
	for (auto pending : mPendingActors)
	{
		// 대기중인 Actor가 올바른 세계 변환 행렬을 가지도록 한다.
		pending->ComputeWorldTransform();
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
	// OpenGL의 색상을 지정
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f); // 회색
	// 색상 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT);

	// 장면을 그린다

	glEnable(GL_BLEND);
	// 색상 버퍼에 alpha blending을 허용해준다.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 스프라이트 셰이더와 vertex 배열 개체를 활성화
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// 모든 스프라이트를 그린다.
	for (auto sprite : mSprites) {
		sprite->Draw(mSpriteShader);
	}

	// 버퍼를 스왑해서 장면을 출력
	SDL_GL_SwapWindow(mWindow);
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


Texture* Game::GetTexture(const std::string& fileName) {
	Texture* tex = nullptr;
	// 불러올 Texture가 이미 맵에 존재하느지 확인
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		// 맵에 존재한다면 Texture에 바로 return
		tex = iter->second;
	}
	// 맵에 없다면 텍스쳐를 로드한다.
	else {
		tex = new Texture();
		if (tex->Load(fileName)) {
			// Load한 Texture를 맵에 넣어준다.
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

// Game의 모든 Actor를 생성한다.
void Game::LoadData() {
	// 플레이어
	mShip = new Ship(this);
	mShip->SetRotation(MathUtils::PIOver2);

	// Asteroids 생성
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++) {
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
		i.second->Unload();
		delete i.second;
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


void Game::InitSpriteVerts() {

	float vertices[] = {
		-0.5f,  0.5f, 0.f, // top left, vertex 0
		 0.5f,  0.5f, 0.f, // top right, vertex 1
		 0.5f, -0.5f, 0.f, // bottom right, vertex 2
		-0.5f, -0.5f, 0.f,  // bottom left, vertex 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// 정점과 인덱스 버퍼 변수는 스프라이트 사각형의 배열이다.
	// 정점 버퍼에 4개의 정점이, 인덱스 버퍼에 6개의 인덱스가 있다.
	// (사각형에는 2개의 삼각형 존재)
	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

// 셰이더 파일을 로드하고 셰이더를 활성화
bool Game::LoadShaders() {
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Transform.vert", "Shaders/Basic.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	// 1024x768로 가정한 뷰-투영 행렬을 생성
	Matrix4x4 viewProj = Matrix4x4::CreateSimpleViewProj(1024.f, 768.f);
	// Vertex shader에 전달
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	
	return true;
}

// 게임
void Game::AddAsteroid(Asteroid* ast) {
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast) {
	auto iter = std::find(mAsteroids.begin(), mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}
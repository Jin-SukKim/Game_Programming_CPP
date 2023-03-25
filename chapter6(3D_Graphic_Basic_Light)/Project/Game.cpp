#include "PreCompiled.hpp"

// constructor
Game::Game() : mRenderer(nullptr),
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

	// 렌더러 생성
	mRenderer = new Renderer(this);
	// 렌더러 초기화
	if (!mRenderer->Initialize(1024.f, 768.f))
	{
		SDL_Log("Failed to Initialize renderer.");
		delete mRenderer;
		mRenderer = nullptr;
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
	if (mRenderer)
	{
		mRenderer->ShutDown();
	}
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
	for (auto actor : mActors) {
		actor->ProcessInput(state);
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
	mRenderer->Draw();
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


// Game의 모든 Actor를 생성한다.
void Game::LoadData() {
	// 액터 생성

	// 사각
	Actor* a = new Actor(this);
	a->SetPosition(Vector3d(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3d::UnitY, -MathUtils::PIOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3d::UnitZ, 
		MathUtils::PI + MathUtils::PI / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	// 원
	a = new Actor(this);
	a->SetPosition(Vector3d(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	// 바닥 생성
	const float start = -1250.f;
	const float size = 250.f;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			a = new PlaneActor(this);
			a->SetPosition(Vector3d(start + i * size,
				start + j * size, -100.f));
		}
	}

	// 왼쪽/오른쪽 벽
	q = Quaternion(Vector3d::UnitX, MathUtils::PIOver2);
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3d(start + i * size, start - size, 0.f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3d(start + i * size, -start + size, 0.f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3d::UnitZ, MathUtils::PIOver2));
	// 앞/뒤 벽
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3d(start - size, start + i * size, 0.f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3d(-start + size, start + i * size, 0.f));
		a->SetRotation(q);
	}

	// 조명 효과 세팅

	// 주변광을 어두운 회색으로 설정
	mRenderer->SetAmbientLight(Vector3d(0.2f, 0.2f, 0.2f));
	// 방향광 설정
	
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	// 방향광 방향 - 아래, 왼쪽(0, -0.7, -0.7)
	dir.mDirection = Vector3d(0.0f, -0.707f, -0.707f);
	// 방향광 난반사 색상
	dir.mDiffuseColor = Vector3d(0.78f, 0.88f, 1.0f);
	// 방향광 정반사 색상
	dir.mSpecColor = Vector3d(0.8f, 0.8f, 0.8f);
	
	// 카메라 생성
	mCameraActor = new CameraActor(this);

	// UI 생성
	a = new Actor(this);
	a->SetPosition(Vector3d(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor(this);
	a->SetPosition(Vector3d(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
}


void Game::UnLoadData() {
	// ~Actor 함수가 RemoveActor를 호출하므로 다른 스타일의 루프를 사용
	// actor 포인터 메모리 삭제
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// 랜더러 삭제
	if (mRenderer)
	{
		mRenderer->UnLoadData();
	}

}

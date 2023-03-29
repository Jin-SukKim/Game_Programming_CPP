#include "PreCompiled.hpp"

// constructor
Game::Game() : mRenderer(nullptr),
mIsRunning(true), mUpdatingActors(false), mAudioSystem(nullptr) {};

// 초기화를 성공하면 true 아니면 false return
bool Game::Initialize() {
	/*	SDL_Init 함수를 사용해 SDL 라이브러리를 초기화
		이 함수는 초기화를 하려는 모든 서브시스템의
		비트 or 연산값을 파라미터로 받는다.
		(Audio, Video, Haptic(force feedback), Gamecontrollor etc)

		정수값을 return, 0이 아니면 초기화가 실패했다는 것이다.
	*/
	int sdlResult = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER); // 비디오/오디오 서브시스템만 초기화


	// 0이 아니면 초기화 실패로 false return
	if (sdlResult != 0) {
		// console에 에러 메시지 출력
		// SDL_Log는 C printf 함수와 같은 문법 사용
		SDL_Log("Unable to initialize SLD : %s", SDL_GetError());
		return false;
	}

	// 입력 시스템 초기화
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
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

	// 오디오 시스템 생성
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
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

	mInputSystem->Shutdown();
	delete mInputSystem;

	if (mRenderer)
	{
		mRenderer->ShutDown();
	}
	// 오디오 시스템 종료
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	// SDL을 닫는다.
	SDL_Quit();
}

// ProcessInput 내부에서 Actor가 또다른 Actor를 만들거나
// Component를 다루는 loop 앞에서는 mUpdatingActors bool 값이 True로 설정돼야 한다.
// 새로운 Actor를 추가할 때는 mActors 대신 mPendingActors에 추가해야 한다.
// 벡터가 반복되는 동안 mActors가 수정되지 않도록해야한다.
void Game::ProcessInput() {

	// 이전 프레임 입력 상태
	mInputSystem->PrepareForUpdate();

	SDL_Event event;

	// SDL_PollEvent는 현재 입력 버퍼를 갱신한다.
	while (SDL_PollEvent(&event)) {
		// 여러 유형의 이벤트 다루기
		switch (event.type) {
		case SDL_QUIT: // 윈도우 X 클릭 or 단축키 사용해 윈도우 닫기
			mIsRunning = false;
			break;
		// 처음 눌리는 키
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyPress(event.key.keysym.sym);
			}
			break;
		case SDL_MOUSEWHEEL:
			mInputSystem->ProcessEvent(event);
			break;
		default:
			break;
		}
	}

	
	// 현재 입력 상태 갱신
	mInputSystem->Update();

	// 입력 상태 얻기
	const InputState& state = mInputSystem->GetState();

	// 이스케이프 키를 눌렀다면 루프 종료
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EReleased) {
		mIsRunning = false;
	}

	mUpdatingActors = true;
	// 모든 Actor를 반복하면서 각 Actor의 ProcessInput을 호출
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

	// 오디오 시스템 갱신
	mAudioSystem->Update(deltaTime);

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

	// 2D
	// Create player's ship
	mShip = new Ship(this);
	mShip->SetRotation(MathUtils::PIOver2);

	// Create asteroids
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}

	// 3D
	/*
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

	// 음악 시작
	
	// 구를 오디오 시스템과 함께 생성해 여러 사운드 재생
	a = new Actor(this);
	a->SetPosition(Vector3d(500.0f, -75.0f, 0.0f));
	a->SetScale(1.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = new AudioComponent(a);
	ac->PlayEvent("event:/FireLoop");
	*/
	// 음악 재생
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
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

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
		case '-':
		{
			// Reduce master volume
			float volume = mAudioSystem->GetBusVolume("bus:/");
			volume = MathUtils::Max(0.0f, volume - 0.1f);
			mAudioSystem->SetBusVolume("bus:/", volume);
			break;
		}
		case '=':
		{
			// Increase master volume
			float volume = mAudioSystem->GetBusVolume("bus:/");
			volume = MathUtils::Min(1.0f, volume + 0.1f);
			mAudioSystem->SetBusVolume("bus:/", volume);
			break;
		}
		case 'e':
			// Play explosion
			mAudioSystem->PlayEvent("event:/Explosion2D");
			break;
		case 'm':
			// Toggle music pause state
			mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
			break;
		case 'r':
			// Stop or start reverb snapshot
			if (!mReverbSnap.IsValid())
			{
				mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
			}
			else
			{
				mReverbSnap.Stop();
			}
			break;
		case '1':
			// Set default footstep surface
			mCameraActor->SetFootstepSurface(0.0f);
			break;
		case '2':
			// Set grass footstep surface
			mCameraActor->SetFootstepSurface(0.5f);
			break;
		default:
			break;
	}
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}
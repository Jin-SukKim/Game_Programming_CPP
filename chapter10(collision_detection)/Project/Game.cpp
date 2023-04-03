#include "PreCompiled.hpp"

// constructor
Game::Game() : mRenderer(nullptr),
mIsRunning(true), mUpdatingActors(false), mAudioSystem(nullptr) {};

// �ʱ�ȭ�� �����ϸ� true �ƴϸ� false return
bool Game::Initialize() {
	/*	SDL_Init �Լ��� ����� SDL ���̺귯���� �ʱ�ȭ
		�� �Լ��� �ʱ�ȭ�� �Ϸ��� ��� ����ý�����
		��Ʈ or ���갪�� �Ķ���ͷ� �޴´�.
		(Audio, Video, Haptic(force feedback), Gamecontrollor etc)

		�������� return, 0�� �ƴϸ� �ʱ�ȭ�� �����ߴٴ� ���̴�.
	*/
	int sdlResult = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_GAMECONTROLLER); // ����/����� ����ý��۸� �ʱ�ȭ


	// 0�� �ƴϸ� �ʱ�ȭ ���з� false return
	if (sdlResult != 0) {
		// console�� ���� �޽��� ���
		// SDL_Log�� C printf �Լ��� ���� ���� ���
		SDL_Log("Unable to initialize SLD : %s", SDL_GetError());
		return false;
	}

	// �Է� �ý��� �ʱ�ȭ
	mInputSystem = new InputSystem();
	if (!mInputSystem->Initialize())
	{
		SDL_Log("Failed to initialize input system");
		return false;
	}

	// ������ ����
	mRenderer = new Renderer(this);
	// ������ �ʱ�ȭ
	if (!mRenderer->Initialize(1024.f, 768.f))
	{
		SDL_Log("Failed to Initialize renderer.");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// ����� �ý��� ����
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	// ���� Ȱ��ȭ
	mPhysWorld = new PhysWorld(this);

	LoadData();

	mTicksCount = SDL_GetTicks();
	// SDL �ʱ�ȭ�� ������ ����
	return true;
}

// ���� ���Ḧ ���� SDL�� �ݴ´�.
void Game::ShutDown() {
	UnLoadData();

	mInputSystem->Shutdown();
	delete mInputSystem;

	if (mRenderer)
	{
		mRenderer->ShutDown();
	}
	// ����� �ý��� ����
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	// SDL�� �ݴ´�.
	SDL_Quit();
}

// ProcessInput ���ο��� Actor�� �Ǵٸ� Actor�� ����ų�
// Component�� �ٷ�� loop �տ����� mUpdatingActors bool ���� True�� �����ž� �Ѵ�.
// ���ο� Actor�� �߰��� ���� mActors ��� mPendingActors�� �߰��ؾ� �Ѵ�.
// ���Ͱ� �ݺ��Ǵ� ���� mActors�� �������� �ʵ����ؾ��Ѵ�.
void Game::ProcessInput() {

	// ���� ������ �Է� ����
	mInputSystem->PrepareForUpdate();

	SDL_Event event;

	// SDL_PollEvent�� ���� �Է� ���۸� �����Ѵ�.
	while (SDL_PollEvent(&event)) {
		// ���� ������ �̺�Ʈ �ٷ��
		switch (event.type) {
		case SDL_QUIT: // ������ X Ŭ�� or ����Ű ����� ������ �ݱ�
			mIsRunning = false;
			break;
		// ó�� ������ Ű
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyPress(event.key.keysym.sym);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			HandleKeyPress(event.button.button);
			break;
		case SDL_MOUSEWHEEL:
			mInputSystem->ProcessEvent(event);
			break;
		default:
			break;
		}
	}

	
	// ���� �Է� ���� ����
	mInputSystem->Update();

	// �Է� ���� ���
	const InputState& state = mInputSystem->GetState();

	// �̽������� Ű�� �����ٸ� ���� ����
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EReleased) {
		mIsRunning = false;
	}

	mUpdatingActors = true;
	// ��� Actor�� �ݺ��ϸ鼭 �� Actor�� ProcessInput�� ȣ��
	for (auto actor : mActors) {
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;

}

void Game::UpdateGame() {
	// ��ǥ �������� 60FPS�� �����ϱ� ����
	// ������ �� delta time 16.6ms�� �����ؾ� �ȴ�.
	// ��� ������ �� 16.ms�� ������� �����Ѵ�.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// delta time�� ���� ������ ƽ���� ���� ������ ƽ���� ����.
	// (��(ms)�� �� ������ ��ȯ)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// �ִ� ��Ÿ �ð������� ����
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// ���� �������� ���� ƽ���� ����
	// SDL_GetTicks() = SDL_Init �Լ� ȣ�� ���� ����� �ð�(ms) ��ȯ
	mTicksCount = SDL_GetTicks();

	// ��� Actor�� ����
	mUpdatingActors = true;
	for (auto actor : mActors) {
		// actor�� component�� actor�� ����
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// ��� ���� Actor�� mActors�� �̵�
	for (auto pending : mPendingActors)
	{
		// ������� Actor�� �ùٸ� ���� ��ȯ ����� �������� �Ѵ�.
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	// vector�� ������ �������� �޸𸮴� �Ҵ�Ǿ��ִ�.
	mPendingActors.clear();

	// ���� Actor�� �ӽ� vector�� �߰�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	// ���� ���� ����(mActors���� �߷��� Actor��)
	for (auto actor : deadActors) {
		delete actor;
	}

	// ����� �ý��� ����
	mAudioSystem->Update(deltaTime);

}

void Game::GenerateOutput() {
	mRenderer->Draw();
}

// ���� ������ �ݺ��ؼ� ����
void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}


// Actor�� �ݺ��ϴ� ���� �� Actor�� �����ϴ� ��츦 �ٷ���� ��� Actor���� ���� ���Ͱ� �ʿ��ϴ�
// mActor�� �ݺ��ǰ� �����Ƿ� mActor�� ��Ҹ� �߰��ϸ� �ȵǰ� mPendingActors�� ��ҷ� �߰���
// mActor�� �ݺ��� ������ mActor�� ������� Actor�� �̵���Ų��.
void Game::AddActor(Actor* actor) {
	// Actor�� ���� ���̶�� ��� ���Ϳ� Actor �߰�
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

// �� vector���� Actor�� �����Ѵ�.
void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// vector�� ������ ���ϰ� �ٲ��ش�.
		std::iter_swap(iter, mPendingActors.end() - 1);
		// ������ element�� �����ϸ� erase copy�� ������ �� �ִ�.
		mPendingActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}


// Game�� ��� Actor�� �����Ѵ�.
void Game::LoadData() {
	// ���� ����
	Actor* a = nullptr;
	Quaternion q;
	// 3D
	
	/*
	// �簢
	a = new Actor(this);
	a->SetPosition(Vector3d(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	q(Vector3d::UnitY, -MathUtils::PIOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3d::UnitZ, 
		MathUtils::PI + MathUtils::PI / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	// ��
	a = new Actor(this);
	a->SetPosition(Vector3d(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	*/

	// �ٴ� ����
	const float start = -1250.f;
	const float size = 250.f;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			a = new PlaneActor(this);
			a->SetPosition(Vector3d(start + i * size,
				start + j * size, -100.f));
		}
	}

	// ����/������ ��
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
	// ��/�� ��
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3d(start - size, start + i * size, 0.f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3d(-start + size, start + i * size, 0.f));
		a->SetRotation(q);
	}

	// ���� ȿ�� ����

	// �ֺ����� ��ο� ȸ������ ����
	mRenderer->SetAmbientLight(Vector3d(0.2f, 0.2f, 0.2f));
	// ���Ɽ ����
	
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	// ���Ɽ ���� - �Ʒ�, ����(0, -0.7, -0.7)
	dir.mDirection = Vector3d(0.0f, -0.707f, -0.707f);
	// ���Ɽ ���ݻ� ����
	dir.mDiffuseColor = Vector3d(0.78f, 0.88f, 1.0f);
	// ���Ɽ ���ݻ� ����
	dir.mSpecColor = Vector3d(0.8f, 0.8f, 0.8f);

	// UI ����
	a = new Actor(this);
	a->SetPosition(Vector3d(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));

	a = new Actor(this);
	a->SetPosition(Vector3d(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));

	a = new Actor(this);
	a->SetScale(2.f);
	mCrosshair = new SpriteComponent(a);
	mCrosshair->SetTexture(mRenderer->GetTexture("Assets/Crosshair.png"));

	// ��� ���콺 Ȱ��ȭ
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// �����ǥ Ŭ����
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// ī�޶� ����
	mFPSActor = new FPSActor(this);
	/*
	mFollowActor = new FollowActor(this);
	mOrbitActor = new OrbitActor(this);
	mSplineActor = new SplineActor(this);
	*/

	// ChangeCamera('1');

	// Ÿ�� ����
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, 0.0f, 100.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, 0.0f, 400.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, -500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, 500.0f, 200.0f));

	// ���� ����
	
	// ���� ����� �ý��۰� �Բ� ������ ���� ���� ���
	a = new Actor(this);
	a->SetPosition(Vector3d(500.0f, -75.0f, 0.0f));
	a->SetScale(1.0f);
	// mc = new MeshComponent(a);
	// mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = new AudioComponent(a);
	ac->PlayEvent("event:/FireLoop");
	
	// ���� ���
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
}


void Game::UnLoadData() {
	// ~Actor �Լ��� RemoveActor�� ȣ���ϹǷ� �ٸ� ��Ÿ���� ������ ���
	// actor ������ �޸� ����
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// ������ ����
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
		case '2':
		case '3':
		case '4':
			ChangeCamera(key);
			break;
		case SDL_BUTTON_LEFT:
		{
			mFPSActor->Shoot();
			break;
		}
		default:
			break;
	}
}

void Game::ChangeCamera(int mode)
{ 
	/*
	// Disable everything
	mFPSActor->SetState(Actor::EPaused);
	mFPSActor->SetVisible(false);
	mCrosshair->SetVisible(false);
	mFollowActor->SetState(Actor::EPaused);
	mFollowActor->SetVisible(false);
	mOrbitActor->SetState(Actor::EPaused);
	mOrbitActor->SetVisible(false);
	mSplineActor->SetState(Actor::EPaused);

	// Enable the camera specified by the mode
	switch (mode)
	{
	case '1':
	default:
		mFPSActor->SetState(Actor::EActive);
		mFPSActor->SetVisible(true);
		mCrosshair->SetVisible(true);
		break;
	case '2':
		mFollowActor->SetState(Actor::EActive);
		mFollowActor->SetVisible(true);
		break;
	case '3':
		mOrbitActor->SetState(Actor::EActive);
		mOrbitActor->SetVisible(true);
		break;
	case '4':
		mSplineActor->SetState(Actor::EActive);
		mSplineActor->RestartSpline();
		break;
	}
	*/
}

void Game::AddPlane(PlaneActor* plane)
{
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane)
{
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}
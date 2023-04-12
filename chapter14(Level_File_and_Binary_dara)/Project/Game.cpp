#include "PreCompiled.hpp"

// constructor
Game::Game() : mRenderer(nullptr),
mIsRunning(true), mUpdatingActors(false), mGameState(EGameplay),
mAudioSystem(nullptr) {};

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

	// 물리 활성화
	mPhysWorld = new PhysWorld(this);

	// SDL_ttf 초기화
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize SDL_ttf");
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
	TTF_Quit();
	delete mPhysWorld;

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
			mGameState = EQuit;
			break;
		// 처음 눌리는 키
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				if (mGameState == EGameplay) {
					HandleKeyPress(event.key.keysym.sym);
				}
				else if (!mUIStack.empty())
				{
					mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (mGameState == EGameplay)
			{
				HandleKeyPress(event.button.button);
			}
			else if (!mUIStack.empty())
			{
				mUIStack.back()->HandleKeyPress(event.button.button);
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

	if (mGameState == EGameplay)
	{
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EActive)
			{
				actor->ProcessInput(state);
			}
		}
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->ProcessInput(state);
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

	if (mGameState == EGameplay)
	{
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

	// 오디오 시스템 갱신
	mAudioSystem->Update(deltaTime);

	// UI screen 갱신
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}

	// UIScreens 갱신
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete* iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::GenerateOutput() {
	mRenderer->Draw();
}

// 게임 루프를 반복해서 실행
void Game::RunLoop() {
	while (mGameState != EQuit) {
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
		// 복사 삭제를 방지하기 위해 마지막을 바꿔서 삭제
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}


// Game의 모든 Actor를 생성한다.
void Game::LoadData() {
	// 영어 텍스트 로드
	LoadText("Assets/English.gptext");

	/*
	// 액터 생성
	Actor* a = nullptr;
	Quaternion q;
	// 3D
	
	// 사각
	a = new Actor(this);
	a->SetPosition(Vector3d(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	q = Quaternion(Vector3d::UnitY, -MathUtils::PIOver2);
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
			Vector3d pos = Vector3d(start + i * size,
				start + j * size, -100.f);
			a->SetPosition(pos);

			// 점광 생성
			a = new Actor(this);
			pos.z += 100.f;
			a->SetPosition(pos);
			PointLightComponent* p = new PointLightComponent(a);
			Vector3d color;
			switch ((i + j) % 5)
			{
			case 0:
				color = Vector3d::Green;
				break;
			case 1:
				color = Vector3d::Blue;
				break;
			case 2:
				color = Vector3d::Red;
				break;
			case 3:
				color = Vector3d::Yellow;
				break;
			case 4:
				color = Vector3d::LightPink;
				break;
			default:
				color = Vector3d::Black;
				break;
			}

			p->mDiffuseColor = color;
			p->mInnerRadius = 100.f;
			p->mOuterRadius = 200.f;
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
	*/

	// 조명 효과 세팅
	/*
	// 주변광을 어두운 회색으로 설정
	mRenderer->SetAmbientLight(Vector3d(0.4f, 0.4f, 0.4f));
	// 방향광 설정
	
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	// 방향광 방향 - 아래, 왼쪽(0, -0.7, -0.7)
	dir.mDirection = Vector3d(0.0f, -0.707f, -0.707f);
	// 방향광 난반사 색상
	dir.mDiffuseColor = Vector3d(0.78f, 0.88f, 1.0f);
	// 방향광 정반사 색상
	dir.mSpecColor = Vector3d(0.8f, 0.8f, 0.8f);
	*/

	// 레벨 파일에서 광원 속성을 로딩한다.
	LevelLoader::LoadLevel(this, "Assets/Level3.gplevel");

	// UI 생성
	mHUD = new HUD(this);
	// 음악 재생
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

	// 상대 마우스 활성화
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// 상대좌표 클리어
	SDL_GetRelativeMouseState(nullptr, nullptr);

	/*
	// 카메라 액터
	mFollowActor = new FollowActor(this);
	
	mFPSActor = new FPSActor(this);
	mOrbitActor = new OrbitActor(this);
	mSplineActor = new SplineActor(this);
	*/

	// ChangeCamera('1');

	/*
	// 타겟 생성
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, 0.0f, 100.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, 0.0f, 400.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, -500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(1450.0f, 500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(0.0f, -1450.0f, 200.0f));
	a->SetRotation(Quaternion(Vector3d::UnitZ, MathUtils::PIOver2));
	a = new TargetActor(this);
	a->SetPosition(Vector3d(0.0f, 1450.0f, 200.0f));
	a->SetRotation(Quaternion(Vector3d::UnitZ, -MathUtils::PIOver2));
	*/

	// 음악 시작
	
	/*
	// 구를 오디오 시스템과 함께 생성해 여러 사운드 재생
	a = new Actor(this);
	a->SetPosition(Vector3d(500.0f, -75.0f, 0.0f));
	a->SetScale(1.0f);
	// mc = new MeshComponent(a);
	// mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = new AudioComponent(a);
	ac->PlayEvent("event:/FireLoop");
	*/
	
}


void Game::UnLoadData() {
	// ~Actor 함수가 RemoveActor를 호출하므로 다른 스타일의 루프를 사용
	// actor 포인터 메모리 삭제
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// UIStack 제거
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}

	// 랜더러 삭제
	if (mRenderer)
	{
		mRenderer->UnLoadData();
	}

	// font 삭제
	for (auto f : mFonts)
	{
		f.second->Unload();
		delete f.second;
	}

	// 뼈대 삭제
	for (auto s : mSkeletons)
	{
		delete s.second;
	}

	// 애니메이션 삭제
	for (auto a : mAnims)
	{
		delete a.second;
	}
}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
		case SDLK_ESCAPE:
		{
			// pause 메뉴 생성
			new PauseMenu(this);
			break;
		}
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
		case '5':
		{
			// load english text
			LoadText("Assets/English.gptext");
			break;
		}
		case '6':
		{
			// load Russian text
			LoadText("Assets/Russian.gptext");
			break;
		}
		case SDL_BUTTON_LEFT:
		{
			// mFPSActor->Shoot();
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

void Game::LoadText(const std::string& fileName)
{
	//  맵 초기화
	mText.clear();
	// 파일 열기
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}

	// 파일 내욜 전부 읽기
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// Open this file in rapidJSON
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}

	// text 맵으로 parsing
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	// 텍스트 찾기
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

Skeleton* Game::GetSkeleton(const std::string& fileName)
{
	auto iter = mSkeletons.find(fileName);
	if (iter != mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(fileName))
		{
			mSkeletons.emplace(fileName, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}

		return sk;
	}
}


Animation* Game::GetAnimation(const std::string& fileName)
{
	auto iter = mAnims.find(fileName);
	if (iter != mAnims.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(fileName))
		{
			mAnims.emplace(fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}

		return anim;
	}
}
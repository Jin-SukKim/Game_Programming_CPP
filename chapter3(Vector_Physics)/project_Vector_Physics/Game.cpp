#include "PreCompiled.hpp"

const int thickness = 15;
const float paddleH = 100.f;
// constructor
Game::Game() : mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mUpdatingActors(false) {};

// �ʱ�ȭ�� �����ϸ� true �ƴϸ� false return
bool Game::Initialize() {
	/*	SDL_Init �Լ��� ����� SDL ���̺귯���� �ʱ�ȭ
		�� �Լ��� �ʱ�ȭ�� �Ϸ��� ��� ����ý�����
		��Ʈ or ���갪�� �Ķ���ͷ� �޴´�.
		(Audio, Video, Haptic(force feedback), Gamecontrollor etc)

		�������� return, 0�� �ƴϸ� �ʱ�ȭ�� �����ߴٴ� ���̴�.
	*/
	int sdlResult = SDL_Init(SDL_INIT_VIDEO); // ���� ����ý��۸� �ʱ�ȭ


	// 0�� �ƴϸ� �ʱ�ȭ ���з� false return
	if (sdlResult != 0) {
		// console�� ���� �޽��� ���
		// SDL_Log�� C printf �Լ��� ���� ���� ���
		SDL_Log("Unable to initialize SLD : %s", SDL_GetError());
		return false;
	}

	sdlResult = IMG_Init(IMG_INIT_PNG); // �̹��� �ʱ�ȭ
	if (sdlResult == 0) {
		SDL_Log("Unable to initialize IMG : %s", SDL_GetError());
		return false;
	}

	/*	������ ����
		�Ķ���� ����	 :
			����, ��ġ�� x/y ��ǥ(���� ��� ���� ����),
			�ʺ�� ����, ���� ���� �÷���)

		�÷��� ���� : ��üȭ��, ���� ����� �ػ󵵷� ��üȭ��, opengl ���̺귯�� �����߰�, ũ�� ����
	*/
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2) - Side Scroll Game",	// ������ ����
		100,	// ������ ���� ��� x��ǥ
		100,	// ������ ���� ��� y��ǥ
		1024,	// ������ �ʺ�
		768,	// ������ ����
		0		// �÷���(0�� �÷��� ���� X
	);

	// ������ ������ �����ϸ� nullptr return
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	/*	�Ķ����:
	*	�������� ������,
	*	� �׷��� ī�带 �������(-1�� default�� SDL�� ����)
	*	�ʱ�ȭ �÷���(����ȭ�� ������(�׷��� �ϵ���� Ȱ��), ���� ����ȭ Ȱ��ȭ ����)
	*/
	mRenderer = SDL_CreateRenderer(
		mWindow,	// �������� ���� ������ ������
		-1,			// �Ϲ������� -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	LoadData();
	mTicksCount = SDL_GetTicks();
	// SDL �ʱ�ȭ�� ������ ����
	return true;
}

// ���� ���Ḧ ���� SDL�� �ݴ´�.
void Game::ShutDown() {
	UnLoadData();
	IMG_Quit();
	// ������ ����
	SDL_DestroyRenderer(mRenderer);
	// SDL_Window ��ü�� �����Ѵ�.
	SDL_DestroyWindow(mWindow);
	// SDL�� �ݴ´�.
	SDL_Quit();
}

void Game::ProcessInput() {

	SDL_Event event;

	// �̺�Ʈ ť�� Ʈ�Ф� �����ӿ��� ���� �̺�Ʈ�� ������ �� �־�
	// ��� �̺�Ʈ�� ��ȸ�ϸ� �ٷ���Ѵ�.
	// �̺�Ʈ ť���� �̺�Ʈ�� �߰��ϸ� true return
	while (SDL_PollEvent(&event)) {
		// ���� ������ �̺�Ʈ �ٷ��
		switch (event.type) {
		case SDL_QUIT: // ������ X Ŭ�� or ����Ű ����� ������ �ݱ�
			mIsRunning = false;
			break;
		}
	}

	// Ű������ ���� ���
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// �̽������� Ű�� �����ٸ� ���� ����
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// Ship �Է�
	mShip->ProcessKeyboard(state);

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
	for (auto pending : mPendingActors) {
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


}

void Game::GenerateOutput() {
	// �������� �ܻ����� ����
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		0,		// B
		255		// A
	);

	// back buffer�� �ܻ����� Ŭ����
	SDL_RenderClear(mRenderer);

	// ��ü ���� ��� �׸���

	// ��������Ʈ ������Ʈ ���� �׷��ֱ�
	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}

	// front buffer�� back buffer ��ȯ
	SDL_RenderPresent(mRenderer);
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

SDL_Texture* Game::LoadTexture(const char* fileName) {
	// ���Ϸκ��� �̹��� �ε�
	// �����ϸ� �����͸� return�ϰ� �����ϸ� nullptr return
	SDL_Surface* surf = IMG_Load(fileName);
	if (!surf) {
		SDL_Log("Failed to load texture file %s", fileName);
		return nullptr;
	}

	// �ؽ��� ����
	// SDL_Surface�� SDL�� ȭ�鿡 �׸��µ� �ʿ��� ������ SDL_Texture�� ��ȯ
	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf); // ����� ������, ��ȯ�� SDL_Surface
	SDL_FreeSurface(surf); // SDL_Surface �ʱ�ȭ
	if (!tex) {
		SDL_Log("Failed to convert surface to texture for %s", fileName);
		return nullptr;
	}

	return tex;
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;
	// �ҷ��� Texture�� �̹� �ʿ� �����ϴ��� Ȯ��
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		// �ʿ� �����Ѵٸ� SDL_Texture�� �ٷ� return
		tex = iter->second;
	}
	// �ʿ� ���ٸ� �ؽ��ĸ� �ε��Ѵ�.
	else {
		tex = LoadTexture(fileName.c_str());
		// Load�� Texture�� �ʿ� �־��ش�.
		mTextures.emplace(fileName, tex);
	}
	return tex;
}

// Game�� ��� Actor�� �����Ѵ�.
void Game::LoadData() {
	// Player's ship �����
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
	// ~Actor �Լ��� RemoveActor�� ȣ���ϹǷ� �ٸ� ��Ÿ���� ������ ���
	// actor ������ �޸� ����
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// Texture ����
	for (auto i : mTextures) {
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();

}

void Game::AddSprite(SpriteComponent* sprite) {
	// ���ĵ� ���Ϳ��� �����ؾ� �� ��ġ�� ã�´�.
	// �ڽź��� �׸��� �������� ū ���� ���
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// �ݺ��� ��ġ �տ� ��Ҹ� �����Ѵ�.
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	// swap�� ������ ����Ʈ���Ƿ� ���� ���Ѵ�.
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
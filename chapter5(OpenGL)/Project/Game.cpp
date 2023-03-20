#include "PreCompiled.hpp"

// constructor
Game::Game() : mWindow(nullptr), mSpriteShader(nullptr),
mIsRunning(true), mUpdatingActors(false) {};

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

	/*
		OpenGL ������ �����ϱ� �� OpenGl�� �����̳� ���� ����,
		�Ķ���͵鿡 ���� �Ӽ� ������ �����ϴ�.
		OpenGL ������ �Ӽ� ����(�����츦 �����ϱ� ���� ����� ��)
		�Լ� ȣ���� �����ϸ� 0�� ��ȯ�ϰ� �����ϸ� ������ return�Ѵ�.
	
	SDL_GL_SetAttribute(
		SDL_GLattr attr,	// ������ �Ӽ�
		int value			// �ش� �Ӽ��� ��
	);

	*/
	// �ھ� OpenGL �������� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// OpenGL 3.3 �������� ����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// RGBA ä�θ��� 8��Ʈ ũ���� ���� ���� ��û
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	// ���� ���۸� Ȱ��ȭ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL�� �ϵ���� ������ ����ϵ��� ����
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


	/*	������ ����
		�Ķ���� ����	 :
			����, ��ġ�� x/y ��ǥ(���� ��� ���� ����),
			�ʺ�� ����, ���� ���� �÷���)

		�÷��� ���� : ��üȭ��, ���� ����� �ػ󵵷� ��üȭ��, opengl ���̺귯�� �����߰�, ũ�� ����
	*/
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 5) - OpenGL",	// ������ ����
		100,	// ������ ���� ��� x��ǥ
		100,	// ������ ���� ��� y��ǥ
		1024,	// ������ �ʺ�
		768,	// ������ ����
		SDL_WINDOW_OPENGL	// OpenGL�� ���
	);

	// ������ ������ �����ϸ� nullptr return
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// ������ �����츦 OpenGL�� Context�� ����
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW �ʱ�ȭ
	glewExperimental = GL_TRUE;	// �Ϻ� �÷������� �ھ� ���ؽ�Ʈ ���� �߻��� ���� �ִ� �ʱ�ȭ ������ ���´�.
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	// �Ϻ� �÷������� GLEW�� ���� �ڵ带 ��������.
	// �׷��Ƿ� �������� ����
	glGetError();
	
	// ���̴�
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// ���� �迭 ��ü ����
	InitSpriteVerts();

	LoadData();

	mTicksCount = SDL_GetTicks();
	// SDL �ʱ�ȭ�� ������ ����
	return true;
}

// ���� ���Ḧ ���� SDL�� �ݴ´�.
void Game::ShutDown() {
	UnLoadData();
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	// SDL Context ����
	SDL_GL_DeleteContext(mContext);
	// SDL_Window ��ü�� �����Ѵ�.
	SDL_DestroyWindow(mWindow);
	// SDL�� �ݴ´�.
	SDL_Quit();
}

// ProcessInput ���ο��� Actor�� �Ǵٸ� Actor�� ����ų�
// Component�� �ٷ�� loop �տ����� mUpdatingActors bool ���� True�� �����ž� �Ѵ�.
// ���ο� Actor�� �߰��� ���� mActors ��� mPendingActors�� �߰��ؾ� �Ѵ�.
// ���Ͱ� �ݺ��Ǵ� ���� mActors�� �������� �ʵ����ؾ��Ѵ�.
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

	// ��� Actor�� �ݺ��ϸ鼭 �� Actor�� ProcessInput�� ȣ��
	mUpdatingActors = true;
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


}

void Game::GenerateOutput() {
	// OpenGL�� ������ ����
	glClearColor(0.86f, 0.86f, 0.86f, 1.0f); // ȸ��
	// ���� ���� �ʱ�ȭ
	glClear(GL_COLOR_BUFFER_BIT);

	// ����� �׸���

	glEnable(GL_BLEND);
	// ���� ���ۿ� alpha blending�� ������ش�.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ��������Ʈ ���̴��� vertex �迭 ��ü�� Ȱ��ȭ
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// ��� ��������Ʈ�� �׸���.
	for (auto sprite : mSprites) {
		sprite->Draw(mSpriteShader);
	}

	// ���۸� �����ؼ� ����� ���
	SDL_GL_SwapWindow(mWindow);
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


Texture* Game::GetTexture(const std::string& fileName) {
	Texture* tex = nullptr;
	// �ҷ��� Texture�� �̹� �ʿ� �����ϴ��� Ȯ��
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		// �ʿ� �����Ѵٸ� Texture�� �ٷ� return
		tex = iter->second;
	}
	// �ʿ� ���ٸ� �ؽ��ĸ� �ε��Ѵ�.
	else {
		tex = new Texture();
		if (tex->Load(fileName)) {
			// Load�� Texture�� �ʿ� �־��ش�.
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

// Game�� ��� Actor�� �����Ѵ�.
void Game::LoadData() {
	// �÷��̾�
	mShip = new Ship(this);
	mShip->SetRotation(MathUtils::PIOver2);

	// Asteroids ����
	const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++) {
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
		i.second->Unload();
		delete i.second;
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

	// ������ �ε��� ���� ������ ��������Ʈ �簢���� �迭�̴�.
	// ���� ���ۿ� 4���� ������, �ε��� ���ۿ� 6���� �ε����� �ִ�.
	// (�簢������ 2���� �ﰢ�� ����)
	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

// ���̴� ������ �ε��ϰ� ���̴��� Ȱ��ȭ
bool Game::LoadShaders() {
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Transform.vert", "Shaders/Basic.frag"))
	{
		return false;
	}
	mSpriteShader->SetActive();

	// 1024x768�� ������ ��-���� ����� ����
	Matrix4x4 viewProj = Matrix4x4::CreateSimpleViewProj(1024.f, 768.f);
	// Vertex shader�� ����
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	
	return true;
}

// ����
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
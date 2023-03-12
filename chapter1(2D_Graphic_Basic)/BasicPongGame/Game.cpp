#include "Game.hpp"

const int thickness = 15;
const float paddleH = 100.f;
// constructor
Game::Game() : mWindow(nullptr), mIsRunning(true), mTicksCount(0), mPaddleDir(0) {};

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

	/*	������ ����
		�Ķ���� ����	 :
			����, ��ġ�� x/y ��ǥ(���� ��� ���� ����),
			�ʺ�� ����, ���� ���� �÷���) 

		�÷��� ���� : ��üȭ��, ���� ����� �ػ󵵷� ��üȭ��, opengl ���̺귯�� �����߰�, ũ�� ����
	*/
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1) - pong game",	// ������ ����
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

	// �е� ù ��ġ
	mPaddlePos.x = 15.f;
	mPaddlePos.y = 768.0f / 2.0f;

	// �� ù ��ġ
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;

	// ���� �ӵ� ����
	mBallVel.x = -200.f;
	mBallVel.y = 235.f;

	// SDL �ʱ�ȭ�� ������ ����
	return true;
}

// ���� ���Ḧ ���� SDL�� �ݴ´�.
void Game::ShutDown() {
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

	// �е��� ��ġ ����, ���� �Է� �� 0
	mPaddleDir = 0;

	// w �Է½� ���� �����δ�(-1, ����y)
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}

	// s �Է½� ������ �����δ�(+1, ���y)
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
	}
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

	// ��Ÿ �ð� �Լ��� ���� ������ ������Ʈ ����
	if (mPaddleDir != 0) {
		// �е��� ����� �ʴ� 300.f �ȼ��� �ӵ�, ��Ÿ �ð����� y��ġ ����
		mPaddlePos.y += mPaddleDir * 300.f * deltaTime;

		// �е��� ȭ�� ������ ������� ����
		if (mPaddlePos.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos.y = paddleH / 2.f + thickness;
		}

		if (mPaddlePos.y > (768.f - paddleH / 2.f - thickness)) {
			mPaddlePos.y = 768.f - paddleH / 2.f - thickness;
		}
	}

	// ���� �ӵ��� ���� ��ġ ����
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// ���� ��� ���� �浹�ϸ� y�� ����
	// �������� �ٲ� �� ���� ������ ���� �浹��
	// ������ ���� ���� ��ų�� �����Ƿ� y �ӵ��� �������ش�.
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1.f;
	}

	// ���� �ϴ� �� �浹
	if (mBallPos.y >= (768.f - thickness) && mBallVel.y > 0.0f) {
		mBallVel.y *= -1.f;
	}

	// ���� ���� �� �浹
	if (mBallPos.x >= (1024 - thickness) && mBallVel.x > 0.0f) {
		mBallVel.x *= -1.f;
	}

	// �е�� ���� y���� ����
	float diff = mPaddlePos.y - mBallPos.y;
	// ���밪
	diff = diff > 0.f ? diff : -diff;

	// ���� �е��� �浹
	if (
		// �е�� ���� y���� ����� �۰�
		diff <= (paddleH / 2.f) &&
		// ���� �ùٸ� x ���� ���� �ְ�
		mBallPos.x <= 25.f && mBallPos.x >= 20.f &&
		// ���� �������� �̵� ���̶��
		mBallVel.x < 0.f) 
	{
		mBallVel.x *= -1.f;
	}

	// ���� ȭ�� ������ ������ ������ ������.
	if (mBallPos.x <= 0.f) {
		mIsRunning = false;
	}
}

void Game::GenerateOutput() {
	// �������� �ܻ����� ����
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G
		255,	// B
		255		// A
	);

	// back buffer�� �ܻ����� Ŭ����
	SDL_RenderClear(mRenderer);

	// ��ü ���� ��� �׸���

	// ������ ������� ����
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// �簢�� ����
	SDL_Rect wall{
		// �簢���� ��ũ�� ��ǥ�� ��ġ
		0,			// ���� ��� x
		0,			// ���� ��� y
		1024,		// �ʺ�
		thickness	// ����
	};

	// SDL_Rect �����͸� �޴� �Լ���
	// �簢�� �׸���
	SDL_RenderFillRect(mRenderer, &wall);

	// bottom wall �׸���
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// right wall �׸���
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	// paddle �׸���
	SDL_Rect paddle{
		// float���� SDL_Rect�� ����ϴ� int�� ��ȯ
		static_cast<int>(mPaddlePos.x - thickness/2.f),
		static_cast<int>(mPaddlePos.y - paddleH/2.f),
		thickness,
		paddleH
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	// ball �׸���
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2.f),
		static_cast<int>(mBallPos.y - thickness / 2.f),
		thickness,
		thickness

	};
	SDL_RenderFillRect(mRenderer, &ball);

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


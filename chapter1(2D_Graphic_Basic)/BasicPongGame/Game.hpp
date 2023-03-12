#pragma once
#include <SDL.h> // SDL_Windows ������ ����

struct Vector2 {
	float x;
	float y;
};

class Game {
private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput(); // ������ input
	void UpdateGame();	// input�� �������� �������� ������Ʈ
	void GenerateOutput(); // �ٲ� ���� ������

	// SDL�� ������ ������
	SDL_Window* mWindow;
	// ������ ��� ����Ǿ� �ϴ����� �Ǵ�
	bool mIsRunning;
	// �׷����� �׸��� ���� ������
	SDL_Renderer* mRenderer;

	// �е�
	Vector2 mPaddlePos;
	// ��
	Vector2 mBallPos;

	// ��Ÿ Ÿ��
	Uint32 mTicksCount;
	// direction of paddle
	int mPaddleDir;
	// ���� �ӵ�
	Vector2 mBallVel;
public:
	// constructor
	Game();
	// ���� �ʱ�ȭ
	bool Initialize();
	// ������ ������ ������ ���� ������ ����
	void RunLoop();
	// ���� ����
	void ShutDown();
};
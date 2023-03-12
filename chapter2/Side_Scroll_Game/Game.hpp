#pragma once
#include <SDL.h> // SDL_Windows ������ ����
#include <vector>
#include <unordered_map>
#include <string>
#include "Actor.hpp"
#include "SpriteComponent.hpp"

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

	// ��Ÿ Ÿ��
	Uint32 mTicksCount;

	// Ȱ��ȭ�� Actor
	std::vector<Actor*> mActors;
	// ��� ���� Actor
	std::vector<Actor*> mPendingActors;
	// ��� vector�� actor�� �߰��� �� ���� ���� Ȯ��
	bool mUpdatingActors;

	// ��� ACtor�� �����Ѵ�.
	void loadData();
	// ������ �����ϱ�
	void unloadData();

	// �ؽ��� ������ ��
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// ���ӿ� �׷����� ��������Ʈ
	std::vector<SpriteComponent*> mSprites;
public:
	// constructor
	Game();
	// ���� �ʱ�ȭ
	bool Initialize();
	// ������ ������ ������ ���� ������ ����
	void RunLoop();
	// ���� ����
	void ShutDown();

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

	// �̹��� �ε� ���� ĸ��ȭ
	SDL_Texture* LoadTexture(const char* fileName);
	// Texture �ʿ��� Texture �������� �Լ�
	SDL_Texture* GetTexture(const std::string& fileName);

	// mDrawOrder ������ ��������Ʈ ������ ����
	void AddSprite(SpriteComponent* sprite);
	// ��������Ʈ ����
	void RemoveSprite(SpriteComponent* sprite);

};
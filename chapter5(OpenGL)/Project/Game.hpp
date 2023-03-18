#pragma once

class Game {
public:
	// constructor
	Game();
	// ���� �ʱ�ȭ
	bool Initialize();
	// ������ ������ ������ ���� ������ ����
	void RunLoop();
	// ���� ����
	void ShutDown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// �̹��� �ε� ���� ĸ��ȭ
	SDL_Texture* LoadTexture(const char* fileName);
	// Texture �ʿ��� Texture �������� �Լ�
	SDL_Texture* GetTexture(const std::string& fileName);

	// mDrawOrder ������ ��������Ʈ ������ ����
	void AddSprite(class SpriteComponent* sprite);
	// ��������Ʈ ����
	void RemoveSprite(class SpriteComponent* sprite);



private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput(); // ������ input
	void UpdateGame();	// input�� �������� �������� ������Ʈ
	void GenerateOutput(); // �ٲ� ���� ������

	// SDL�� ������ ������
	SDL_Window* mWindow;
	// ������ ��� ����Ǿ� �ϴ����� �Ǵ�
	bool mIsRunning;

	// ��Ÿ Ÿ��
	Uint32 mTicksCount;

	// Ȱ��ȭ�� Actor
	std::vector<class Actor*> mActors;
	// ��� ���� Actor
	std::vector<class Actor*> mPendingActors;
	// ��� vector�� actor�� �߰��� �� ���� ���� Ȯ��
	bool mUpdatingActors;

	// ��� ACtor�� �����Ѵ�.
	void LoadData();
	// ������ �����ϱ�
	void UnLoadData();

	// �ؽ��� ������ ��
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// ���ӿ� �׷����� ��������Ʈ
	std::vector<class SpriteComponent*> mSprites;

	// OpenGL ���ؽ�Ʈ
	SDL_GLContext mContext;

	class VertexArray* mSpriteVerts;
	void InitSpriteVerts();

};
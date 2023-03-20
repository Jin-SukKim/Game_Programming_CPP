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

	// Texture �ʿ��� Texture �������� �Լ�
	class Texture* GetTexture(const std::string& fileName);

	// mDrawOrder ������ ��������Ʈ ������ ����
	void AddSprite(class SpriteComponent* sprite);
	// ��������Ʈ ����
	void RemoveSprite(class SpriteComponent* sprite);

	// ����
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() {
		return mAsteroids;
	}

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
	std::unordered_map<std::string, class Texture*> mTextures;

	// ���ӿ� �׷����� ��������Ʈ
	std::vector<class SpriteComponent*> mSprites;

	// OpenGL ���ؽ�Ʈ
	SDL_GLContext mContext;

	// �ﰢ���� ���� �迭
	class VertexArray* mSpriteVerts;
	void InitSpriteVerts();

	// ���̴�, ��������Ʈ�� �׸��µ� ���ȴ�.
	class Shader* mSpriteShader;
	bool LoadShaders();

	// ���� ������Ʈ
	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;

};
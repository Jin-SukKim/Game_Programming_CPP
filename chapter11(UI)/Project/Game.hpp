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

	// ȭ���� �׸� ������
	class Renderer* GetRenderer() { return mRenderer; }
	// ����� �ý���
	class AudioSystem* GetAudioSystem() {
		return mAudioSystem;
	}
	class PhysWorld* GetPhysWorld() {
		return mPhysWorld;
	}

	class Font* GetFont(const std::string& fileName);

	// UI
	// ������ ������ ��ȯ�Ѵ�.
	const std::vector<class UIScreen*>& GetUIStack() {
		return mUIStack;
	}
	// Ư�� UIScreen�� ���ÿ� �߰��Ѵ�.
	void PushUI(class UIScreen* screen);

	enum GameState {
		EGameplay,
		EPaused,
		EQuit
	};
	GameState GetState() const {
		return mGameState;
	}
	void SetState(GameState state)
	{
		mGameState = state;
	}

	class HUD* GetHUD() { return mHUD; }
	class FPSActor* GetPlayer() {
		return mFPSActor;
	}
	
	// gptext ������ parsing�ϰ� mText�� �߰�
	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	// ����	
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() {
		return mPlanes;
	}
private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput(); // ������ input
	void UpdateGame();	// input�� �������� �������� ������Ʈ
	void GenerateOutput(); // �ٲ� ���� ������

	// ������
	class Renderer* mRenderer;

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

	// ����� �ý���
	AudioSystem* mAudioSystem;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	// �Է�ó��
	void HandleKeyPress(int key);

	// �Է� �ý���
	class InputSystem* mInputSystem;
	GameState mGameState;
	// �浹 �ý���
	class PhysWorld* mPhysWorld;

	std::unordered_map<std::string, class Font*> mFonts;

	// UI
	// ������ ���� UI ����
	std::vector<class UIScreen*> mUIStack;
	class HUD* mHUD;

	// �ؽ�Ʈ ����ȭ
	std::unordered_map<std::string, std::string> mText;

	// Game-specific
	void ChangeCamera(int mode);
	class FPSActor* mFPSActor;
	class SpriteComponent* mCrosshair;
	class FollowActor* mFollowActor;
	class OrbitActor* mOrbitActor;
	class SplineActor* mSplineActor;

	std::vector<class PlaneActor*> mPlanes;
};
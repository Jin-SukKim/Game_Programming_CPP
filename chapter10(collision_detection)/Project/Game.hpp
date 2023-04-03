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
	// �浹 �ý���
	class PhysWorld* mPhysWorld;

	// Game-specific
	void ChangeCamera(int mode);
	class FPSActor* mFPSActor;
	class SpriteComponent* mCrosshair;
	class FollowActor* mFollowActor;
	class OrbitActor* mOrbitActor;
	class SplineActor* mSplineActor;

	std::vector<class PlaneActor*> mPlanes;
};
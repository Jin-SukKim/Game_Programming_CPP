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

	class CameraActor* mCameraActor;

	// ����� �ý���
	AudioSystem* mAudioSystem;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	// �Է�ó��
	void HandleKeyPress(int key);
};
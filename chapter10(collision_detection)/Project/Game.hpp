#pragma once

class Game {
public:
	// constructor
	Game();
	// 게임 초기화
	bool Initialize();
	// 게임이 끝나기 전까지 게임 루프를 실행
	void RunLoop();
	// 게임 종료
	void ShutDown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	// 화면을 그릴 랜더러
	class Renderer* GetRenderer() { return mRenderer; }
	// 오디오 시스템
	class AudioSystem* GetAudioSystem() {
		return mAudioSystem;
	}
	class PhysWorld* GetPhysWorld() {
		return mPhysWorld;
	}

	// 게임	
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() {
		return mPlanes;
	}
private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput(); // 유저의 input
	void UpdateGame();	// input을 바탕으로 로직등을 업데이트
	void GenerateOutput(); // 바뀐 씬을 랜더링

	// 렌더러
	class Renderer* mRenderer;

	// 게임이 계쏙 실행되야 하는지를 판단
	bool mIsRunning;

	// 델타 타임
	Uint32 mTicksCount;

	// 활성화된 Actor
	std::vector<class Actor*> mActors;
	// 대기 중인 Actor
	std::vector<class Actor*> mPendingActors;
	// 어느 vector에 actor를 추가할 지 갱신 여부 확인
	bool mUpdatingActors;

	// 모든 ACtor를 생성한다.
	void LoadData();
	// 데이터 삭제하기
	void UnLoadData();

	// 오디오 시스템
	AudioSystem* mAudioSystem;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	// 입력처리
	void HandleKeyPress(int key);

	// 입력 시스템
	class InputSystem* mInputSystem;
	// 충돌 시스템
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
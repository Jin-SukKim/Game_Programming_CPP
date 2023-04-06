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

	class Font* GetFont(const std::string& fileName);

	// UI
	// 참조로 스택을 반환한다.
	const std::vector<class UIScreen*>& GetUIStack() {
		return mUIStack;
	}
	// 특정 UIScreen을 스택에 추가한다.
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
	
	// gptext 파일을 parsing하고 mText에 추가
	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

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
	GameState mGameState;
	// 충돌 시스템
	class PhysWorld* mPhysWorld;

	std::unordered_map<std::string, class Font*> mFonts;

	// UI
	// 게임을 위한 UI 스택
	std::vector<class UIScreen*> mUIStack;
	class HUD* mHUD;

	// 텍스트 현지화
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
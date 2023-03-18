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

	// 이미지 로딩 과정 캡슐화
	SDL_Texture* LoadTexture(const char* fileName);
	// Texture 맵에서 Texture 가져오는 함수
	SDL_Texture* GetTexture(const std::string& fileName);

	// mDrawOrder 순서로 스프라이트 정렬해 저장
	void AddSprite(class SpriteComponent* sprite);
	// 스프라이트 삭제
	void RemoveSprite(class SpriteComponent* sprite);



private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput(); // 유저의 input
	void UpdateGame();	// input을 바탕으로 로직등을 업데이트
	void GenerateOutput(); // 바뀐 씬을 랜더링

	// SDL이 생성한 윈도우
	SDL_Window* mWindow;
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

	// 텍스쳐 관리용 맵
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// 게임에 그려야할 스프라이트
	std::vector<class SpriteComponent*> mSprites;

	// OpenGL 콘텍스트
	SDL_GLContext mContext;

	class VertexArray* mSpriteVerts;
	void InitSpriteVerts();

};
#pragma once
#include <SDL.h> // SDL_Windows 포인터 참조
#include <vector>
#include <unordered_map>
#include <string>
#include "Actor.hpp"
#include "SpriteComponent.hpp"

class Game {
private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput(); // 유저의 input
	void UpdateGame();	// input을 바탕으로 로직등을 업데이트
	void GenerateOutput(); // 바뀐 씬을 랜더링

	// SDL이 생성한 윈도우
	SDL_Window* mWindow;
	// 게임이 계쏙 실행되야 하는지를 판단
	bool mIsRunning;
	// 그래픽을 그리기 위한 렌더러
	SDL_Renderer* mRenderer;

	// 델타 타임
	Uint32 mTicksCount;

	// 활성화된 Actor
	std::vector<Actor*> mActors;
	// 대기 중인 Actor
	std::vector<Actor*> mPendingActors;
	// 어느 vector에 actor를 추가할 지 갱신 여부 확인
	bool mUpdatingActors;

	// 모든 ACtor를 생성한다.
	void loadData();
	// 데이터 삭제하기
	void unloadData();

	// 텍스쳐 관리용 맵
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// 게임에 그려야할 스프라이트
	std::vector<SpriteComponent*> mSprites;
public:
	// constructor
	Game();
	// 게임 초기화
	bool Initialize();
	// 게임이 끝나기 전까지 게임 루프를 실행
	void RunLoop();
	// 게임 종료
	void ShutDown();

	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);

	// 이미지 로딩 과정 캡슐화
	SDL_Texture* LoadTexture(const char* fileName);
	// Texture 맵에서 Texture 가져오는 함수
	SDL_Texture* GetTexture(const std::string& fileName);

	// mDrawOrder 순서로 스프라이트 정렬해 저장
	void AddSprite(SpriteComponent* sprite);
	// 스프라이트 삭제
	void RemoveSprite(SpriteComponent* sprite);

};
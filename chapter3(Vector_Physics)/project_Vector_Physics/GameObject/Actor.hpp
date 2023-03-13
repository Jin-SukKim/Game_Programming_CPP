#pragma once

// 게임 객체 모델의 기본 클래스
class Actor {
public:
	// Actor의 상태를 추적하는데 사용
	// EActive일때만 갱신, EDead상태는 게임에게 제거하라고 통지 등
	enum State { EActive, EPaused, EDead };

	// 생성자
	Actor(class Game* game);
	// 소멸자
	virtual ~Actor();

	// Setters
	void SetPosition(const Vector2d& pos) { mPosition = pos; }
	void SetState(State state) { mState = state; }
	void SetScale(float scale) { mScale = scale; }
	void SetRotation(float rotation) { mRotation = rotation; }

	// Getters
	State GetState() const { return mState; }
	const Vector2d GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }

	class Game* GetGame() { return mGame; }

	// Game에서 호출하는 Update 함수
	void Update(float deltaTime);
	// Actor에 부착된 모든 컴포넌트를 업데이트
	void UpdateComponents(float deltaTime);
	// 특정 Actor에 특화된 업데이트 코드
	// 기본 구현은 비어있으나 서브클래스는 함수를 재정의해 변경가능
	virtual void UpdateActor(float deltaTime) {}

	// 컴포넌트 추가/제거
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// 전방 벡터
	Vector2d GetForward();
private:
	// Actor의 상태
	State mState;
	// 변환
	Vector2d mPosition;	// Actor의 중심점
	float mScale;		// Actor의 배율(100%의 경우 1.f)
	float mRotation;	// 회전 각도 (Radian)

	// Actor가 보유한 컴포넌트들
	std::vector<class Component*> mComponents;
	class Game* mGame;

};
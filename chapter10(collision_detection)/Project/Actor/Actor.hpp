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
	// Actor의 크기, 회전, 이동에 관한 setting이 있으면
	// world space에도 변화가 있으므로 worldtransform을 true로 설정
	void SetPosition(const Vector3d& pos) { 
		mPosition = pos; 
		mRecomputeWorldTransform = true;
	}
	void SetState(State state) { mState = state; }
	void SetScale(float scale) { 
		mScale = scale; 
		mRecomputeWorldTransform = true;
	}
	void SetRotation(const Quaternion& rotation) { 
		mRotation = rotation; 
		mRecomputeWorldTransform = true;
	}
	void SetRotation(float rotation) { 
		mRotation2 = rotation;  
		mRecomputeWorldTransform = true; 
	}
	float GetRotation2() const { return mRotation2; }


	// Getters
	State GetState() const { return mState; }
	const Vector3d GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	Quaternion GetRotation() const { return mRotation; }

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

	// 전방 벡터 - 초기 전방 벡터(+x)를 회전 쿼터니언으로 변환해 얻는다.
	Vector3d GetForward() const;
	// 좌우 이동
	Vector3d GetRight() const;

	// Game이 호출하는 ProcessInput함수
	void ProcessInput(const struct InputState& state);
	// 특정 Actor를 위한 입력 코드
	virtual void ActorInput(const struct InputState& state) {};

	// 세계 변환 행렬(model space -> world space)
	void ComputeWorldTransform();
	const Matrix4x4& GetWorldTransform() const {
		return mWorldTransform;
	}

	// 회전값을 수정하는 함수
	void RotateToNewForward(const Vector3d& forward);

private:
	// Actor의 상태
	State mState;

	// Actor가 보유한 컴포넌트들
	std::vector<class Component*> mComponents;
	class Game* mGame;

	// transform
	Matrix4x4 mWorldTransform;
	Vector3d mPosition;	// Actor의 중심점
	float mScale;		// Actor의 배율(100%의 경우 1.f)
	Quaternion mRotation;	// 회전 각도 (Radian)
	bool mRecomputeWorldTransform;

	float mRotation2;

};
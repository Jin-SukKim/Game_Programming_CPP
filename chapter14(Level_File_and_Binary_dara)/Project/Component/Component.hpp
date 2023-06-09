#pragma once
class Component {
public:
	// 타입 정보 구현
	enum TypeID
	{
		TComponent = 0,
		TAudioComponent,
		TBallMove,
		TBoxComponent,
		TCameraComponent,
		TFollowCamera,
		TMeshComponent,
		TMoveComponent,
		TSkeletalMeshComponent,
		TSpriteComponent,
		TMirrorCamera,
		TPointLightComponent,
		TTargetComponent,

		NUM_COMPONENT_TYPES
	};
	static const char* TypeNames[NUM_COMPONENT_TYPES];

	// 생성자
	// (업데이트 순석밧이 작을수록 컴포넌트는 더 빨리 갱신된다)
	Component(class Actor* owner, int updateOrder = 100);
	// 소멸자
	virtual ~Component();
	// delta time으로 컴포넌트 업데이트
	virtual void Update(float deltaTime) {}

	int GetUpdateOrder() const {
		return mUpdateOrder;
	}

	virtual void ProcessInput(const struct InputState& state) {}

	// 소유자의 세계 변환이 갱신될 때 컴포넌트한테 통지한다.
	virtual void OnUpdateWorldTransform() {};

	class Actor* GetOwner() {
		return mOwner;
	}

	virtual TypeID GetType() const = 0;

	// Load/Save
	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const;

	// 정확한 타입의 속성의 컴포넌트를 만든다.
	template <typename T>
	static Component* Create(class Actor* actor, const rapidjson::Value& inObj)
	{
		// Dynamically allocate component of type T
		T* t = new T(actor);
		// Call LoadProperties on new component
		t->LoadProperties(inObj);
		return t;
	}
// 상속관계의 클래스끼리 공유한다.
protected:
	// 소유자 actor
	// 컴포넌트가 필요하다고 판단되는 변환 데이터 및 여러 정보에 접근하기 위해 Actor 포인터를 가진다.
	class Actor* mOwner;

	// 컴포넌트 업데이트 순서
	/*
		갱신 순서를 지정하면 매우 유용하다.
		ex) 1인칭 카메라 컴포넌트의 경우 이동 컴포넌트가
		플레이어를 이동시킨 다음 갱신되어야한다.
		이 순서 유지를 위해 Actor의 AddComponent함수는 컴포넌트 추가 시
		컴포넌트 벡터를 정렬한다.
	*/
	int mUpdateOrder;

};
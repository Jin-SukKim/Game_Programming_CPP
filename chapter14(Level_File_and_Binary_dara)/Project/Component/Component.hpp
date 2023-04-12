#pragma once
class Component {
public:
	// Ÿ�� ���� ����
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

	// ������
	// (������Ʈ �������� �������� ������Ʈ�� �� ���� ���ŵȴ�)
	Component(class Actor* owner, int updateOrder = 100);
	// �Ҹ���
	virtual ~Component();
	// delta time���� ������Ʈ ������Ʈ
	virtual void Update(float deltaTime) {}

	int GetUpdateOrder() const {
		return mUpdateOrder;
	}

	virtual void ProcessInput(const struct InputState& state) {}

	// �������� ���� ��ȯ�� ���ŵ� �� ������Ʈ���� �����Ѵ�.
	virtual void OnUpdateWorldTransform() {};

	class Actor* GetOwner() {
		return mOwner;
	}

	virtual TypeID GetType() const = 0;

	// Load/Save
	virtual void LoadProperties(const rapidjson::Value& inObj);
	virtual void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const;

	// ��Ȯ�� Ÿ���� �Ӽ��� ������Ʈ�� �����.
	template <typename T>
	static Component* Create(class Actor* actor, const rapidjson::Value& inObj)
	{
		// Dynamically allocate component of type T
		T* t = new T(actor);
		// Call LoadProperties on new component
		t->LoadProperties(inObj);
		return t;
	}
// ��Ӱ����� Ŭ�������� �����Ѵ�.
protected:
	// ������ actor
	// ������Ʈ�� �ʿ��ϴٰ� �ǴܵǴ� ��ȯ ������ �� ���� ������ �����ϱ� ���� Actor �����͸� ������.
	class Actor* mOwner;

	// ������Ʈ ������Ʈ ����
	/*
		���� ������ �����ϸ� �ſ� �����ϴ�.
		ex) 1��Ī ī�޶� ������Ʈ�� ��� �̵� ������Ʈ��
		�÷��̾ �̵���Ų ���� ���ŵǾ���Ѵ�.
		�� ���� ������ ���� Actor�� AddComponent�Լ��� ������Ʈ �߰� ��
		������Ʈ ���͸� �����Ѵ�.
	*/
	int mUpdateOrder;

};
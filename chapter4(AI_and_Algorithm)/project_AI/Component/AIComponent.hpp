class AIComponent : public Component {
public :
	AIComponent(class Actor* owner);

	// ���� ���¿� �ش��ϴ� ���� �Լ��� ȣ��
	void Update(float deltaTime) override;
	
	// ������ �Լ��� ���� ����� ���̸� �ٷ� �� �ִ�.
	// ���� ���� �Լ��� �� �Լ��� ȣ���ؼ� ���� ���̸� �ʱ�ȭ�Ѵ�.
	void ChangeState(const std::string& name);

	// �� ���¸� �ʿ� �߰��Ѵ�.
	void RegisterState(class AIState* state);
private:
	// ������ �̸��� AIState �ν��Ͻ��� �����Ѵ�.
	std::unordered_map<std::string, class AIState*> mStateMap;
	// AI�� ���� ����
	class AIState* mCurrentState;
};
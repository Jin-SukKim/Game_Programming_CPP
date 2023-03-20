#include "..\PreCompiled.hpp"

AIComponent::AIComponent(Actor* owner) : Component(owner), mCurrentState(nullptr) {}

// ���°� �����ϸ� �ش� ������ Update�Լ� ȣ��
void AIComponent::Update(float deltaTime) {
	if (mCurrentState) {
		mCurrentState->Update(deltaTime);
	}
}

// ���� �� mStateMap�� ���� �Ķ���ͷ� ���� ���� �̸���
// ���¸� ã���� �� ���·� �����Ѵ�.
void AIComponent::ChangeState(const std::string& name) {
	// ���� ���¸� �������´�
	if (mCurrentState) {
		mCurrentState->OnExit();
	}

	// �ʿ��� �����ϰ��� �ϴ� ���ο� ���¸� ã�´�.
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end()) {
		// ���ο� ���·� ����
		mCurrentState = iter->second;
		mCurrentState->OnEnter();
	}
	else {
		// ���� ���¸� ã�� �� ������ ���� �޽��� ���
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

// �ʿ� AIState�� ���� �̸��� �߰�
void AIComponent::RegisterState(AIState* state) {
	mStateMap.emplace(state->GetName(), state);
}
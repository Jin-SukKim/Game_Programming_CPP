#include "..\PreCompiled.hpp"

AIComponent::AIComponent(Actor* owner) : Component(owner), mCurrentState(nullptr) {}

// 상태가 존재하면 해당 상태의 Update함수 호출
void AIComponent::Update(float deltaTime) {
	if (mCurrentState) {
		mCurrentState->Update(deltaTime);
	}
}

// 상태 맵 mStateMap을 보고 파라미터로 받은 상태 이름의
// 상태를 찾으면 이 상태로 전이한다.
void AIComponent::ChangeState(const std::string& name) {
	// 현재 상태를 빠져나온다
	if (mCurrentState) {
		mCurrentState->OnExit();
	}

	// 맵에서 변경하고자 하는 새로운 상태를 찾는다.
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end()) {
		// 새로운 상태로 변경
		mCurrentState = iter->second;
		mCurrentState->OnEnter();
	}
	else {
		// 다음 상태를 찾을 수 없으면 에러 메시지 출력
		SDL_Log("Could not find AIState %s in state map", name.c_str());
		mCurrentState = nullptr;
	}
}

// 맵에 AIState와 상태 이름을 추가
void AIComponent::RegisterState(AIState* state) {
	mStateMap.emplace(state->GetName(), state);
}
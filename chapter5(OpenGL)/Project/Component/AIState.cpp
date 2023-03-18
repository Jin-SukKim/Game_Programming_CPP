#include "PreCompiled.hpp"

AIState::AIState(class AIComponent* owner) : mOwner(owner) {}

AIPatrol::AIPatrol(AIComponent* owner) : AIState(owner) {}

AIDeath::AIDeath(AIComponent* owner) : AIState(owner) {}

AIAttack::AIAttack(AIComponent* owner) : AIState(owner) {}

// 캐릭터가 죽을 때 AIDeath로 상태를 변경
// 전이를 초기화하기 위해 소유자 컴포넌트의 ChangeState 함수를 호출
void AIPatrol::Update(float deltaTime) {
	bool daed = true;
	if (daed) {
		// AIComponent 상태를 변경하라고 알림
		mOwner->ChangeState("Death");
	}

}

void AIPatrol::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIPatrol::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIDeath::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIDeath::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIDeath::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

void AIAttack::Update(float deltaTime)
{
	SDL_Log("Updating %s state", GetName());
}

void AIAttack::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void AIAttack::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}

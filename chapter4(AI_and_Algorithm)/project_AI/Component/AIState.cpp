#include "PreCompiled.hpp"

AIState::AIState(class AIComponent* owner) : mOwner(owner) {}

AIPatrol::AIPatrol(AIComponent* owner) : AIState(owner) {}

AIDeath::AIDeath(AIComponent* owner) : AIState(owner) {}

AIAttack::AIAttack(AIComponent* owner) : AIState(owner) {}

// ĳ���Ͱ� ���� �� AIDeath�� ���¸� ����
// ���̸� �ʱ�ȭ�ϱ� ���� ������ ������Ʈ�� ChangeState �Լ��� ȣ��
void AIPatrol::Update(float deltaTime) {
	bool daed = true;
	if (daed) {
		// AIComponent ���¸� �����϶�� �˸�
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

#include "..\..\PreCompiled.hpp"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder) {}

void CameraComponent::SetViewMatrix(const Matrix4x4& view)
{
	Game* game = mOwner->GetGame();
	game->GetRenderer()->SetViewMatrix(view);
	game->GetAudioSystem()->SetListener(view);
}
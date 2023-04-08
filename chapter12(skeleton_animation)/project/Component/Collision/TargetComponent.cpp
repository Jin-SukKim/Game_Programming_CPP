#include "..\..\PreCompiled.hpp"

TargetComponent::TargetComponent(Actor* owner) : Component(owner)
{
	mOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

TargetComponent::~TargetComponent()
{
	mOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}


#include "PreCompiled.hpp"

PlaneActor::PlaneActor(Game* game) : Actor(game) {
	SetScale(10.f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}
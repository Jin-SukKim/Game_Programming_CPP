#include "..\PreCompiled.hpp"

TargetActor::TargetActor(Game* game) :Actor(game)
{
	SetRotation(Quaternion(Vector3d::UnitZ, MathUtils::PI));
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);

	// 충돌 박스
	BoxComponent* bc = new BoxComponent(this);
	bc->SetObjectBox(mesh->GetBox());

	new TargetComponent(this);
}
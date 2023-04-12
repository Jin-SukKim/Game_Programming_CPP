#include "..\PreCompiled.hpp"

PlaneActor::PlaneActor(Game* game) : Actor(game) {
	SetScale(10.f);
	MeshComponent* mc = new MeshComponent(this);

	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);

	// 충돌 박스 추가
	mBox = new BoxComponent(this);
	mBox->SetObjectBox(mesh->GetBox());

	game->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
	GetGame()->RemovePlane(this);
}
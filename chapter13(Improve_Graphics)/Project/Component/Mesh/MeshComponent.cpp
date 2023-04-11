#include "..\..\PreCompiled.hpp"

MeshComponent::MeshComponent(Actor* owner, bool isSkeletal)
	: Component(owner), mMesh(nullptr), mTextureIndex(0), 
	mIsSkeletal(isSkeletal), mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh) {
		// 세계 변환 설정
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());

		// 정반사 값 세팅
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// 활성화될 텍스처 설정
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) {
			// 텍스처 활성화
			t->SetActive();
		}

		// 메시 vertex 배열 설정
		VertexArray* va = mMesh->GetVertexArray();
		// 메시 활성화
		va->SetActive();

		// 메시 그리기
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
}
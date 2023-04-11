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
		// ���� ��ȯ ����
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());

		// ���ݻ� �� ����
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

		// Ȱ��ȭ�� �ؽ�ó ����
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t) {
			// �ؽ�ó Ȱ��ȭ
			t->SetActive();
		}

		// �޽� vertex �迭 ����
		VertexArray* va = mMesh->GetVertexArray();
		// �޽� Ȱ��ȭ
		va->SetActive();

		// �޽� �׸���
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(),
			GL_UNSIGNED_INT, nullptr);
	}
}
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

void MeshComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	std::string meshFile;
	if (JsonHelper::GetString(inObj, "meshFile", meshFile))
	{
		SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh(meshFile));
	}

	int idx;
	if (JsonHelper::GetInt(inObj, "textureIndex", idx))
	{
		mTextureIndex = static_cast<size_t>(idx);
	}

	JsonHelper::GetBool(inObj, "visible", mVisible);
	JsonHelper::GetBool(inObj, "isSkeletal", mIsSkeletal);
}


void MeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	if (mMesh)
	{
		JsonHelper::AddString(alloc, inObj, "meshFile", mMesh->GetFileName());
	}
	JsonHelper::AddInt(alloc, inObj, "textureIndex", static_cast<int>(mTextureIndex));
	JsonHelper::AddBool(alloc, inObj, "visible", mVisible);
	JsonHelper::AddBool(alloc, inObj, "isSkeletal", mIsSkeletal);
}

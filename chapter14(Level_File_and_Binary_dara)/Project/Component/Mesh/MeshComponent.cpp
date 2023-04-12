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

#include "..\..\PreCompiled.hpp"

PointLightComponent::PointLightComponent(Actor* owner) : Component(owner)
{
	owner->GetGame()->GetRenderer()->AddPointLight(this);
}

PointLightComponent::~PointLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	// �ܺ� ���������� ���� ��ȯ�� ������(�޽� ���������� ������)
	// �׸��� ���� ��ġ�� ��ü�� �̵�
	Matrix4x4 scale = Matrix4x4::CreateScale(mOwner->GetScale() 
		* mOuterRadius / mesh->GetRadius());
	Matrix4x4 trans = Matrix4x4::CreateTranslation(mOwner->GetPosition());
	Matrix4x4 worldTransform = scale * trans;
	shader->SetMatrixUniform("uWorldTransform", worldTransform);

	// ���� ���̴� ����� ����
	shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
	shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);

	// ��ü�� �׸���.
	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(),
		GL_UNSIGNED_INT, nullptr);

}

void PointLightComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);
	JsonHelper::GetVector3(inObj, "color", mDiffuseColor);
	JsonHelper::GetFloat(inObj, "innerRadius", mInnerRadius);
	JsonHelper::GetFloat(inObj, "outerRadius", mOuterRadius);
}

void PointLightComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	JsonHelper::AddVector3(alloc, inObj, "color", mDiffuseColor);
	JsonHelper::AddFloat(alloc, inObj, "innerRadius", mInnerRadius);
	JsonHelper::AddFloat(alloc, inObj, "outerRadius", mOuterRadius);
}

#include "PreCompiled.hpp"

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
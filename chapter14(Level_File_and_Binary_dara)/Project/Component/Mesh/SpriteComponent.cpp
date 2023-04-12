#include "..\..\PreCompiled.hpp"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
	Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), 
	mTexWidth(0), mTexHeight(0), mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

// mTexture ������ �����ϸ� SDL_QueryTexture�� �����
// �ؽ����� �ʺ�� ���̸� ��´�.
void SpriteComponent::SetTexture(Texture* texture) {
	// ��������Ʈ�� �ؽ��� ����
	mTexture = texture;
	// �ؽ�ó�� �ʺ�� ���� ���
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}

// OpenGL���� ���̴��� �̿��� ������Ʈ�� �׸���
void SpriteComponent::Draw(Shader* shader) {
	if (mTexture) {
		// �ؽ����� �ʺ�� ���̷� �簢���� ������
		Matrix4x4 scaleMat = Matrix4x4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		// ũ�� ��ȯ ���
		Matrix4x4 world = scaleMat * mOwner->GetWorldTransform();
		// ���̴��� ũ�� ��ȯ ����� ����
		shader->SetMatrixUniform("uWorldTransform", world);

		// ���� �ؽ�ó ����
		mTexture->SetActive();

		// �簢���� �׸���.
		// OpenGL�� �׸��� �Լ�
		glDrawElements(
			GL_TRIANGLES,	// �׷��� �� ������/������Ƽ���� Ÿ��(�ﰢ��)
			6,				// index buffer�� index ��(�簢���̹Ƿ� 6��)
			GL_UNSIGNED_INT,// index�� Ÿ��, unsigned int�� �����ߴ�.
			nullptr
		);
	}
}
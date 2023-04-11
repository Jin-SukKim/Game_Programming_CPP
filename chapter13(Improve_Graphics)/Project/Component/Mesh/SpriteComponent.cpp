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

// mTexture 변수를 설정하며 SDL_QueryTexture를 사용해
// 텍스쳐의 너비와 높이를 얻는다.
void SpriteComponent::SetTexture(Texture* texture) {
	// 스프라이트의 텍스쳐 설정
	mTexture = texture;
	// 텍스처의 너비와 높이 얻기
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
}

// OpenGL에서 셰이더를 이용해 오브젝트를 그린다
void SpriteComponent::Draw(Shader* shader) {
	if (mTexture) {
		// 텍스터의 너비와 높이로 사각형을 스케일
		Matrix4x4 scaleMat = Matrix4x4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		// 크기 변환 행렬
		Matrix4x4 world = scaleMat * mOwner->GetWorldTransform();
		// 셰이더에 크기 변환 행렬을 설정
		shader->SetMatrixUniform("uWorldTransform", world);

		// 현재 텍스처 설정
		mTexture->SetActive();

		// 사각형을 그린다.
		// OpenGL의 그리기 함수
		glDrawElements(
			GL_TRIANGLES,	// 그려야 할 폴리곤/프리미티브의 타입(삼각형)
			6,				// index buffer의 index 수(사각형이므로 6개)
			GL_UNSIGNED_INT,// index의 타입, unsigned int로 설정했다.
			nullptr
		);
	}
}
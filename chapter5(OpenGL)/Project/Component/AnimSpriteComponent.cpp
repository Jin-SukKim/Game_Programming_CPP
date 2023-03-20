#include "..\PreCompiled.hpp"

AnimSpriteComponent::AnimSpriteComponent(class Actor* owner, int drawOrder)
	: SpriteComponent(owner), mCurrFrame(0.f), mAnimFPS(24.f) {}

void AnimSpriteComponent::SetAnimTextures(const std::vector<Texture*>& textures) {
	mAnimTextures = textures;

	if (mAnimTextures.size() > 0) {
		// 첫 번째 프레임을 설정한다.
		mCurrFrame = 0.f; // 초기화
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		// 프레임 레이트와 delta time을 기반으로
		// 현재 프레임 갱신
		mCurrFrame += mAnimFPS * deltaTime;

		// mCurrFrame이 텍스처의 수보다 작게 유지되는지를 확인한다.
		// 애니메이션 텍스처 수를 초과하면 현재 프레임을 래핑한다.
		// 즉 시작 지점으로 되돌아간다.
		while (mCurrFrame >= mAnimTextures.size()) {
			mCurrFrame -= mAnimTextures.size();
		}

		// 현재 텍스처를 설정
		// mCurrFrame을 int형으로 타입 캐스트한 후 mAnimTextures로부터
		// 원하는 텍스처를 얻은 뒤 SetTuexture를 호출한다.
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}

}
#include "BGSpriteComponent.hpp"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder), mScrollSpeed(0.f) {}

// 오프셋 값을 초기화하고, 각각의 배경을 이전 배경의 오른쪽에 위치시킨다.
void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures) {
	int count = 0;
	for (auto tex : textures) {
		BGTexture temp;
		temp.mTexture = tex;
		// 각 텍스처의 오프셋은 화면의 너비 * count다
		// 각 배경 이미지의 너비가 화면 너비와 같다고 가정하지만
		// 가변 크기에도 대응할 수 있다.
		temp.mOffset.x = mScreenSize.x * count;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}

// 이미지를 무한 반복
void BGSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	// 한 이미지가 화면에서 완전히 벗어나는 시점을 고려하며
	// 각 배경의 오프셋을 갱신한다.
	for (auto& bg : mBGTextures) {
		// x 오프셋 값을 갱신
		bg.mOffset.x += mScrollSpeed * deltaTime;

		// 이 텍스처가 화면을 완전히 벗어나면 오프셋 값을
		// 마지막 배경 텍스처의 오른쪽 위치로 초기화한다.
		if (bg.mOffset.x < -mScreenSize.x) {
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

// 각 배경 텍스처를 SDL_RenderCopy를 사용해 그려준다.
// 배경의 위치는 프레임마다 오프셋을 기준으로 조정되며, 이것으로 배경의 스크롤이 가능하다.
void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	// 각 배경 텍스처 그리기
	for (auto& bg : mBGTextures) {
		SDL_Rect r;
		// 텍스처 크기는 화면 크기와 같다고 가정
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		// SDL_Rect의 좌표, 배경의 위치는 프레임마다 오프셋 기준으로 조정한다.
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);

		// 배경 그리기
		SDL_RenderCopy(
			renderer,
			bg.mTexture,
			nullptr,
			&r
		);
	}

}

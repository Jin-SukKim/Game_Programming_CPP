#include "..\PreCompiled.hpp"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
	Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}

// mTexture 변수를 설정하며 SDL_QueryTexture를 사용해
// 텍스쳐의 너비와 높이를 얻는다.
void SpriteComponent::SetTexture(SDL_Texture* texture) {
	// 스프라이트의 텍스쳐 설정
	mTexture = texture;
	// 텍스처의 너비와 높이 얻기
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

// SDL에는 Texture를 그리기 위해 2가지 함수를 제공한다.
// SDL_RenderCopy는 간단한 함수이고
// 스프라이트 회전 등의 기능을 사용하려면 SDL_RenderCopyEX를 사용해야한다.
/*
	텍스쳐를 렌더링 타깃에 그린다 성공하면 0을 return, 실패하면 음수를 반환
	int SDL_RenderCopyEx(
		SDL_Renderer* renderer,		// 그려질 렌더링 타깃
		SDL_Texture* texture,		// 그릴 텍스쳐
		const SDL_Rect* srcrect,	// 그릴 텍스쳐의 일부 영역 (전체면 nullptr)
		const SDL_Rect* dstrect,	// 타깃에 그릴 사각형 영역
		double angle,				// 회전 각도(각도, 시계 방향)
		const SDL_Point* center,	// 회전 중심점(중심이면 nullptr)
		SDL_RenderFlip flip			// 텍스처를 플립하는 방법(대개 SDL_FLIP_NONE)
	);
*/
void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		SDL_Rect r;

		// 소유자의 스케일(배율)값으로 너비/높이를 정한다.
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());


		// SDL_Rect의 좌표는 이미지의 상단 왼쪽 모서리에 해당하는데,
		// Actor의 위치 값은 Actor의 중심점이므로 Actor 상단 모서리의 좌푯값을 계산한다.
		// 소유자의 위치를 중심으로 사각형을 배치
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// 스프라이트를 그린다
		SDL_RenderCopyEx(
			renderer,
			mTexture,
			nullptr,
			&r,
			// SDL는 Degree를 사용하는데 Actor는 Radian을 사용한다.
			// SDL에서 양의 각도는 시계 방향인데 단위 원(반시계 방향)과 반대다.
			-MathUtils::ToDegrees(mOwner->GetRotation()), // 라디안을 각도로 변환, 각도 반전
			nullptr,
			SDL_FLIP_NONE
		);
	}
}
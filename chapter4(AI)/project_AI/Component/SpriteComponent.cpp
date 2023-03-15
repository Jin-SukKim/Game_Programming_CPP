#include "..\PreCompiled.hpp"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder) :
	Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}

// mTexture ������ �����ϸ� SDL_QueryTexture�� �����
// �ؽ����� �ʺ�� ���̸� ��´�.
void SpriteComponent::SetTexture(SDL_Texture* texture) {
	// ��������Ʈ�� �ؽ��� ����
	mTexture = texture;
	// �ؽ�ó�� �ʺ�� ���� ���
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

// SDL���� Texture�� �׸��� ���� 2���� �Լ��� �����Ѵ�.
// SDL_RenderCopy�� ������ �Լ��̰�
// ��������Ʈ ȸ�� ���� ����� ����Ϸ��� SDL_RenderCopyEX�� ����ؾ��Ѵ�.
/*
	�ؽ��ĸ� ������ Ÿ�꿡 �׸��� �����ϸ� 0�� return, �����ϸ� ������ ��ȯ
	int SDL_RenderCopyEx(
		SDL_Renderer* renderer,		// �׷��� ������ Ÿ��
		SDL_Texture* texture,		// �׸� �ؽ���
		const SDL_Rect* srcrect,	// �׸� �ؽ����� �Ϻ� ���� (��ü�� nullptr)
		const SDL_Rect* dstrect,	// Ÿ�꿡 �׸� �簢�� ����
		double angle,				// ȸ�� ����(����, �ð� ����)
		const SDL_Point* center,	// ȸ�� �߽���(�߽��̸� nullptr)
		SDL_RenderFlip flip			// �ؽ�ó�� �ø��ϴ� ���(�밳 SDL_FLIP_NONE)
	);
*/
void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		SDL_Rect r;

		// �������� ������(����)������ �ʺ�/���̸� ���Ѵ�.
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());


		// SDL_Rect�� ��ǥ�� �̹����� ��� ���� �𼭸��� �ش��ϴµ�,
		// Actor�� ��ġ ���� Actor�� �߽����̹Ƿ� Actor ��� �𼭸��� ��ǩ���� ����Ѵ�.
		// �������� ��ġ�� �߽����� �簢���� ��ġ
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// ��������Ʈ�� �׸���
		SDL_RenderCopyEx(
			renderer,
			mTexture,
			nullptr,
			&r,
			// SDL�� Degree�� ����ϴµ� Actor�� Radian�� ����Ѵ�.
			// SDL���� ���� ������ �ð� �����ε� ���� ��(�ݽð� ����)�� �ݴ��.
			-MathUtils::ToDegrees(mOwner->GetRotation()), // ������ ������ ��ȯ, ���� ����
			nullptr,
			SDL_FLIP_NONE
		);
	}
}
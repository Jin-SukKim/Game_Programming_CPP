#pragma once

class SpriteComponent : public Component {
public:
	// 그리기 순석밧이 낮을수록 더 뒤쪽에서 그린다
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	// 스프라이트를 그리기 위한 랜더러를 받는 Draw 함수
	virtual void Draw(class Shader* shader);
	// 스프라이트에 Texture를 주기위한 함수
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

protected:
	// 그릴 텍스처
	class Texture* mTexture;
	// 화가 알고리즘에 사용하는 그리기 순서
	int mDrawOrder;
	// 텍스처의 너비/높이
	int mTexWidth;
	int mTexHeight;
};
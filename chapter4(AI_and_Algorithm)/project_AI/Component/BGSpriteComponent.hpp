#pragma once
// 배경을 만들기 위한 클래스
class BGSpriteComponent : public SpriteComponent {
public:
	// 기본 그리기 순서값을 낮은 값으로 설정 (배경이다)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	// 부모로부터 override한 Update/draw 함수
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	// 배경에 사용되는 텍스처 설정
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	// 화면 사이즈 설정
	void SetScreenSize(const Vector2d& size) {
		mScreenSize = size;
	}
	// 스크롤 속도값 설정
	void SetScrollSpeed(float speed) {
		mScrollSpeed = speed;
	}
	// 스크롤 속도값 얻기
	float GetScrollSpeed() const {
		return mScrollSpeed;
	}
private:
	// 각 배경 이미지와 배경 이미지의 오프셋 값을 캡슐화한 구조체
	struct BGTexture {
		SDL_Texture* mTexture;
		// 스크롤 효과를 위해 텍스처의 위치를 프레임마다 갱신하는데 사용
		Vector2d mOffset;
	};

	std::vector<BGTexture> mBGTextures;
	Vector2d mScreenSize;
	float mScrollSpeed;

};

#pragma once
class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// 애니메이션을 프레임마다 갱신
	void Update(float deltaTime) override; // Compoenent의 업데이트 override
	// 애니메이션에 사용되는 텍스처 설정
	void SetAnimTextures(const std::vector<class Texture*>& textures);
	// 애니메이션 FPS를 얻는 함수
	float GetAnimFPS() const {
		return mAnimFPS;
	}
	// 애니메이션 FPS 설정
	void SetAnimFPS(float fps) {
		mAnimFPS = fps;
	}
private:
	// 애니메이션에 사용되는 모든 텍스쳐
	std::vector<class Texture*> mAnimTextures;
	// 현재 프레임, 해당 프렝임이 시작된 후 경과된 시간을 알 수 있다.
	float mCurrFrame;
	// 애니메이션 프레임 레이트
	float mAnimFPS;

};
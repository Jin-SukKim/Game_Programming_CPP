#pragma once
#include "SpriteComponent.hpp"

class AnimeSpriteComponent : public SpriteComponent {
public :
	AnimeSpriteComponent(Actor* owner, int drawOrder = 100);
	// �ִϸ��̼��� �����Ӹ��� ����
	void Update(float deltaTime) override; // Compoenent�� ������Ʈ override
	// �ִϸ��̼ǿ� ���Ǵ� �ؽ�ó ����
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// �ִϸ��̼� FPS�� ��� �Լ�
	float GetAnimFPS() const {
		return mAnimFPS;
	}
	// �ִϸ��̼� FPS ����
	void SetAnimFPS(float fps) {
		mAnimFPS = fps;
	}
private:
	// �ִϸ��̼ǿ� ���Ǵ� ��� �ؽ���
	std::vector<SDL_Texture*> mAnimTextures;
	// ���� ������, �ش� �������� ���۵� �� ����� �ð��� �� �� �ִ�.
	float mCurrFrame;
	// �ִϸ��̼� ������ ����Ʈ
	float mAnimFPS;
	
};
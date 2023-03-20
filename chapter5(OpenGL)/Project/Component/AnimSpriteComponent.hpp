#pragma once
class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// �ִϸ��̼��� �����Ӹ��� ����
	void Update(float deltaTime) override; // Compoenent�� ������Ʈ override
	// �ִϸ��̼ǿ� ���Ǵ� �ؽ�ó ����
	void SetAnimTextures(const std::vector<class Texture*>& textures);
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
	std::vector<class Texture*> mAnimTextures;
	// ���� ������, �ش� �������� ���۵� �� ����� �ð��� �� �� �ִ�.
	float mCurrFrame;
	// �ִϸ��̼� ������ ����Ʈ
	float mAnimFPS;

};
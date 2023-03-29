#include "..\PreCompiled.hpp"

AnimSpriteComponent::AnimSpriteComponent(class Actor* owner, int drawOrder)
	: SpriteComponent(owner), mCurrFrame(0.f), mAnimFPS(24.f) {}

void AnimSpriteComponent::SetAnimTextures(const std::vector<Texture*>& textures) {
	mAnimTextures = textures;

	if (mAnimTextures.size() > 0) {
		// ù ��° �������� �����Ѵ�.
		mCurrFrame = 0.f; // �ʱ�ȭ
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		// ������ ����Ʈ�� delta time�� �������
		// ���� ������ ����
		mCurrFrame += mAnimFPS * deltaTime;

		// mCurrFrame�� �ؽ�ó�� ������ �۰� �����Ǵ����� Ȯ���Ѵ�.
		// �ִϸ��̼� �ؽ�ó ���� �ʰ��ϸ� ���� �������� �����Ѵ�.
		// �� ���� �������� �ǵ��ư���.
		while (mCurrFrame >= mAnimTextures.size()) {
			mCurrFrame -= mAnimTextures.size();
		}

		// ���� �ؽ�ó�� ����
		// mCurrFrame�� int������ Ÿ�� ĳ��Ʈ�� �� mAnimTextures�κ���
		// ���ϴ� �ؽ�ó�� ���� �� SetTuexture�� ȣ���Ѵ�.
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}

}
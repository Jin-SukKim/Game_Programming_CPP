#pragma once
// ����� ����� ���� Ŭ����
class BGSpriteComponent : public SpriteComponent {
public:
	// �⺻ �׸��� �������� ���� ������ ���� (����̴�)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	// �θ�κ��� override�� Update/draw �Լ�
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	// ��濡 ���Ǵ� �ؽ�ó ����
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	// ȭ�� ������ ����
	void SetScreenSize(const Vector2d& size) {
		mScreenSize = size;
	}
	// ��ũ�� �ӵ��� ����
	void SetScrollSpeed(float speed) {
		mScrollSpeed = speed;
	}
	// ��ũ�� �ӵ��� ���
	float GetScrollSpeed() const {
		return mScrollSpeed;
	}
private:
	// �� ��� �̹����� ��� �̹����� ������ ���� ĸ��ȭ�� ����ü
	struct BGTexture {
		SDL_Texture* mTexture;
		// ��ũ�� ȿ���� ���� �ؽ�ó�� ��ġ�� �����Ӹ��� �����ϴµ� ���
		Vector2d mOffset;
	};

	std::vector<BGTexture> mBGTextures;
	Vector2d mScreenSize;
	float mScrollSpeed;

};

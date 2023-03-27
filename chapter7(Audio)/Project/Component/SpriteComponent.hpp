#pragma once

class SpriteComponent : public Component {
public:
	// �׸��� �������� �������� �� ���ʿ��� �׸���
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	// ��������Ʈ�� �׸��� ���� �������� �޴� Draw �Լ�
	virtual void Draw(class Shader* shader);
	// ��������Ʈ�� Texture�� �ֱ����� �Լ�
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

protected:
	// �׸� �ؽ�ó
	class Texture* mTexture;
	// ȭ�� �˰��� ����ϴ� �׸��� ����
	int mDrawOrder;
	// �ؽ�ó�� �ʺ�/����
	int mTexWidth;
	int mTexHeight;
};
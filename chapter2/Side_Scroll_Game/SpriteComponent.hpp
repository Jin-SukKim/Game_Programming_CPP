#include "Component.hpp"
#include <SDL.h>
#include "Math.hpp"

class SpriteComponent : public Component {
public:
	// �׸��� �������� �������� �� ���ʿ��� �׸���
	SpriteComponent(Actor* owner, int drawOrder = 100);
	~SpriteComponent();
	// ��������Ʈ�� �׸��� ���� �������� �޴� Draw �Լ�
	virtual void Draw(SDL_Renderer* renderer);
	// ��������Ʈ�� Texture�� �ֱ����� �Լ�
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const {
		return mDrawOrder;
	}
	int GetTexWidth() const {
		return mTexWidth;
	}
	int GetTexHeight() const {
		return mTexHeight;
	}

protected:
	// �׸� �ؽ�ó
	SDL_Texture* mTexture;
	// ȭ�� �˰��� ����ϴ� �׸��� ����
	int mDrawOrder;
	// �ؽ�ó�� �ʺ�/����
	int mTexWidth;
	int mTexHeight;
};
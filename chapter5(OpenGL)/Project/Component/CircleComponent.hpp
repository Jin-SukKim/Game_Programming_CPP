// Actor�� �浹 ������ ���� Ŭ����
class CircleComponent : public Component {
public:
	CircleComponent(class Actor* owner);

	// ���� ������ ����
	void SetRadius(float radius) {
		mRadius = radius;
	}
	float GetRadius() const;

	// ������ Actor�� ��ġ
	const Vector2d GetCenter() const;

private:
	float mRadius;
};

// ���� �׽�Ʈ
bool Intersect(const CircleComponent& a, const CircleComponent& b);
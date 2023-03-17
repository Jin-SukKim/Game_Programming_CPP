// Actor의 충돌 감지를 위한 클래스
class CircleComponent : public Component {
public:
	CircleComponent(class Actor* owner);

	// 원의 반지름 설정
	void SetRadius(float radius) {
		mRadius = radius;
	}
	float GetRadius() const;

	// 소유자 Actor의 위치
	const Vector2d GetCenter() const;

private:
	float mRadius;
};

// 교차 테스트
bool Intersect(const CircleComponent& a, const CircleComponent& b);
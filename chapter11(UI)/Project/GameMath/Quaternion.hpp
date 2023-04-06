// 단위 쿼터니언, 보통 4d 벡터로 계산해 function을 재선언해준다.
class Quaternion {
public:
	// 쿼터니언은 4개의 값을 가진다.
	float x;
	float y;
	float z;
	float w;
	
	static const Quaternion Identity;

	Quaternion();

	// 쿼터니언을 각각의 값으로 생성하기
	explicit Quaternion(float inX, float inY, float inZ, float inW);
	// 축과 각도로부터 쿼터니언 생성 (축 벡터는 이미 정규화했다고 가정)
	explicit Quaternion(const Vector3d& axis, float angle);
	
	// 쿼터니언의 4가지 값을 설정하기
	void Set(float inX, float inY, float inZ, float inW) {
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	// 켤레복소수로 역 쿼터니언, q^-1
	void Conjugate() {
		x *= -1.f;
		y *= -1.f;
		z *= -1.f;
	}

	// 쿼터니언 크기^2
	float LengthSq() const;
	// 쿼터니언 크기
	float Length() const;

	// 쿼터니언 정규화
	void Normalize();
	static Quaternion Normalize(const Quaternion& q);

	// 선형 보간
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f);
	
	// 내적
	static float Dot(const Quaternion& a, const Quaternion& b);

	// 구면 선형 보간 - 보다 정확한 보간 지원
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f);

	// 쿼터니언 곱셈 잇기 (q로 회전한 뒤 p의 회전은 그라스만 곱 pq를 사용)
	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p);
};
class Vector3d {
public:
	float x;
	float y;
	float z;

	static const Vector3d Zero;
	static const Vector3d UnitX;
	static const Vector3d UnitY;
	static const Vector3d UnitZ;
	static const Vector3d NegUnitX;
	static const Vector3d NegUnitY;
	static const Vector3d NegUnitZ;
	static const Vector3d Infinity;
	static const Vector3d NegInfinity;

	Vector3d();
	explicit Vector3d(float inX, float inY, float inZ);
	
	// const float pointer�� ��ȯ
	const float* GetAsFloatPtr() const;

	// x,y,z ����
	void Set(float inX, float inY, float inZ);

	// ���� ����
	Vector3d operator+(const Vector3d& v) const;
	Vector3d& operator+=(const Vector3d& v);
	// ���� ����
	Vector3d operator-(const Vector3d& v) const;
	Vector3d& operator-=(const Vector3d& v);
	// ���� ����
	Vector3d operator*(const Vector3d& v) const;
	Vector3d operator*(float scalar) const;
	Vector3d& operator*=(float scalar);
	Vector3d& operator*=(const Vector3d& v);

	// ���� ����^2
	float LengthSq() const;
	// ������ ����
	float Length() const;
	// ����ȭ
	void Normalize();
	static Vector3d Normalize(const Vector3d& v);
	// ����
	static float Dot(const Vector3d& v, const Vector3d& u);
	// ����
	static Vector3d Cross(const Vector3d& v, const Vector3d& u);
	// ���� ����
	static Vector3d Lerp(const Vector3d& v, const Vector3d& u, float f);
	// �ݻ� ����
	static Vector3d Reflect(const Vector3d& v, const Vector3d& u);
	// �̵�
	static Vector3d Transform(const Vector3d& v, const class Matrix4x4& mat, float w = 1.f);
	static Vector3d TransformWithPersDiv(const Vector3d& v, const class Matrix4x4& mat, float w = 1.f);
	
	// Vector3d�� Quaternion���� ��ȯ
	static Vector3d Transform(const Vector3d& v, const class Quaternion& q);
};
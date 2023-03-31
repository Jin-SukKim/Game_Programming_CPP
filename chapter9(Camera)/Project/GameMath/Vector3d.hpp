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
	
	// const float pointer∑Œ ∫Ø»Ø
	const float* GetAsFloatPtr() const;

	// x,y,z ºº∆√
	void Set(float inX, float inY, float inZ);

	// ∫§≈Õ µ°º¿
	Vector3d operator+(const Vector3d& v) const;
	Vector3d& operator+=(const Vector3d& v);
	// ∫§≈Õ ª¨º¿
	Vector3d operator-(const Vector3d& v) const;
	Vector3d& operator-=(const Vector3d& v);
	// ∫§≈Õ ∞ˆº¿
	Vector3d operator*(const Vector3d& v) const;
	Vector3d operator*(float scalar) const;
	Vector3d& operator*=(float scalar);
	Vector3d& operator*=(const Vector3d& v);

	// ∫§≈Õ ±Ê¿Ã^2
	float LengthSq() const;
	// ∫§≈Õ¿« ±Ê¿Ã
	float Length() const;
	// ¡§±‘»≠
	void Normalize();
	static Vector3d Normalize(const Vector3d& v);
	// ≥ª¿˚
	static float Dot(const Vector3d& v, const Vector3d& u);
	// ø‹¿˚
	static Vector3d Cross(const Vector3d& v, const Vector3d& u);
	// º±«¸ ∫∏∞£
	static Vector3d Lerp(const Vector3d& v, const Vector3d& u, float f);
	// π›ªÁ ∫§≈Õ
	static Vector3d Reflect(const Vector3d& v, const Vector3d& u);
	// ¿Ãµø
	static Vector3d Transform(const Vector3d& v, const class Matrix4x4& mat, float w = 1.f);
	static Vector3d TransformWithPersDiv(const Vector3d& v, const class Matrix4x4& mat, float w = 1.f);
	
	// Vector3d∏¶ Quaternion¿∏∑Œ ∫Ø»Ø
	static Vector3d Transform(const Vector3d& v, const class Quaternion& q);
};
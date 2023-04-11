class Matrix4x4 {
public :
	float mat[4][4];
	static const Matrix4x4 Identity;
	Matrix4x4();

	explicit Matrix4x4(float inMat[4][4]);

	// float 포인터로 변환
	const float* GetAsFloatPtr() const;

	// 행렬 곱
	Matrix4x4 operator*(const Matrix4x4& m);
	friend Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
	Matrix4x4& operator*=(const Matrix4x4& m);

	// 행렬 뒤집기 - 매우 느림
	void Invert();
	
	// 행렬의 Vector값 얻기
	Vector3d GetTranslation() const;

	// 행렬의 x 축 값 얻기(forward)
	Vector3d GetXAxis() const;
	// y 축 값 얻기(left)
	Vector3d GetYAxis() const;
	// Z 축 값 얻기 (up)
	Vector3d GetZAxis() const;

	// 행렬의 크기 벡터
	Vector3d GetScale() const;

	// 크기 행렬
	static Matrix4x4 CreateScale(float xScale, float yScale, float zScale);
	static Matrix4x4 CreateScale(const Vector3d& scaleVector);
	static Matrix4x4 CreateScale(float scale);

	// x축 회전
	static Matrix4x4 CreateRotationX(float theta);
	// y축 회전
	static Matrix4x4 CreateRotationY(float theta);
	// z축 회전
	static Matrix4x4 CreateRotationZ(float theta);

	// 이동 행렬
	static Matrix4x4 CreateTranslation(const Vector3d& trans);

	// 뷰 행렬
	static Matrix4x4 CreateLookAt(const Vector3d& eye, const Vector3d& target, const Vector3d& up);
	// 직교 투영 행렬
	static Matrix4x4 CreateOrtho(float width, float height, float near, float far);
	// 원근 투영 행렬
	static Matrix4x4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far);
	
	// View-Projection 행렬
	static Matrix4x4 CreateSimpleViewProj(float width, float height);

	// 쿼터니언에서 matrix4x4 생성
	static Matrix4x4 CreateFromQuaternion(const class Quaternion& q);
};
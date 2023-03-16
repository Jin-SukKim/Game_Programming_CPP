class Matrix4x4 {
public :
	float mat[4][4];
	static const Matrix4x4 Identity;
	Matrix4x4();

	explicit Matrix4x4(float inMat[4][4]);

	// float �����ͷ� ��ȯ
	const float* GetAsFloatPtr() const;

	// ��� ��
	Matrix4x4 operator*(const Matrix4x4& m);
	Matrix4x4& operator*=(const Matrix4x4& m);

	// ��� ������ - �ſ� ����
	void Invert();

	// ����� Vector�� ���
	Vector3d GetTranslattion() const;

	// ����� x �� �� ���(forward)
	Vector3d GetXAxis() const;
	// y �� �� ���(left)
	Vector3d GetYAxis() const;
	// Z �� �� ��� (up)
	Vector3d GetZAxis() const;

	// ����� ũ�� ����
	Vector3d GetScale() const;

	// ũ�� ���
	static Matrix4x4 CreateScale(float xScale, float yScale, float zScale);
	static Matrix4x4 CreateScale(const Vector3d& scaleVector);
	static Matrix4x4 CreateScale(float scale);

	// x�� ȸ��
	static Matrix4x4 CreateRotationX(float theta);
	// y�� ȸ��
	static Matrix4x4 CreateRotationY(float theta);
	// z�� ȸ��
	static Matrix4x4 CreateRotationZ(float theta);
	
	// Quaternion(�����)���� ��ĸ����
	// static Matrix4x4 CreateFromQuaternion(const class Quaternion& q);

	// �̵� ���
	static Matrix4x4 CreateTranslation(const Vector3d& trans);

	static Matrix4x4 CreateLookAt(const Vector3d& eye, const Vector3d& target, const Vector3d& up);
	static Matrix4x4 CreateOrtho(float width, float height, float near, float far);
	static Matrix4x4 CreatePerspectiveFOV(float fovY, float width, float height, float near, float far);
	
	// View-Projection ���
	static Matrix4x4 CreateSimpleViewProj(float width, float height);
};
#include "../PreCompiled.hpp"

static float m4Ident[4][4] =
{
	{ 1.0f, 0.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f }
};

const Matrix4x4 Matrix4x4::Identity(m4Ident);

Matrix4x4::Matrix4x4() {
	*this = Matrix4x4::Identity;
}

Matrix4x4::Matrix4x4(float inMat[4][4]) {
	memcpy(mat, inMat, 16 * sizeof(float));
}

const float* Matrix4x4::GetAsFloatPtr() const {
	return reinterpret_cast<const float*>(&mat[0][0]);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& b) {
	Matrix4x4 retVal;
	// row 0
	retVal.mat[0][0] =
		mat[0][0] * b.mat[0][0] +
		mat[0][1] * b.mat[1][0] +
		mat[0][2] * b.mat[2][0] +
		mat[0][3] * b.mat[3][0];

	retVal.mat[0][1] =
		mat[0][0] * b.mat[0][1] +
		mat[0][1] * b.mat[1][1] +
		mat[0][2] * b.mat[2][1] +
		mat[0][3] * b.mat[3][1];

	retVal.mat[0][2] =
		mat[0][0] * b.mat[0][2] +
		mat[0][1] * b.mat[1][2] +
		mat[0][2] * b.mat[2][2] +
		mat[0][3] * b.mat[3][2];

	retVal.mat[0][3] =
		mat[0][0] * b.mat[0][3] +
		mat[0][1] * b.mat[1][3] +
		mat[0][2] * b.mat[2][3] +
		mat[0][3] * b.mat[3][3];

	// row 1
	retVal.mat[1][0] =
		mat[1][0] * b.mat[0][0] +
		mat[1][1] * b.mat[1][0] +
		mat[1][2] * b.mat[2][0] +
		mat[1][3] * b.mat[3][0];

	retVal.mat[1][1] =
		mat[1][0] * b.mat[0][1] +
		mat[1][1] * b.mat[1][1] +
		mat[1][2] * b.mat[2][1] +
		mat[1][3] * b.mat[3][1];

	retVal.mat[1][2] =
		mat[1][0] * b.mat[0][2] +
		mat[1][1] * b.mat[1][2] +
		mat[1][2] * b.mat[2][2] +
		mat[1][3] * b.mat[3][2];

	retVal.mat[1][3] =
		mat[1][0] * b.mat[0][3] +
		mat[1][1] * b.mat[1][3] +
		mat[1][2] * b.mat[2][3] +
		mat[1][3] * b.mat[3][3];

	// row 2
	retVal.mat[2][0] =
		mat[2][0] * b.mat[0][0] +
		mat[2][1] * b.mat[1][0] +
		mat[2][2] * b.mat[2][0] +
		mat[2][3] * b.mat[3][0];

	retVal.mat[2][1] =
		mat[2][0] * b.mat[0][1] +
		mat[2][1] * b.mat[1][1] +
		mat[2][2] * b.mat[2][1] +
		mat[2][3] * b.mat[3][1];

	retVal.mat[2][2] =
		mat[2][0] * b.mat[0][2] +
		mat[2][1] * b.mat[1][2] +
		mat[2][2] * b.mat[2][2] +
		mat[2][3] * b.mat[3][2];

	retVal.mat[2][3] =
		mat[2][0] * b.mat[0][3] +
		mat[2][1] * b.mat[1][3] +
		mat[2][2] * b.mat[2][3] +
		mat[2][3] * b.mat[3][3];

	// row 3
	retVal.mat[3][0] =
		mat[3][0] * b.mat[0][0] +
		mat[3][1] * b.mat[1][0] +
		mat[3][2] * b.mat[2][0] +
		mat[3][3] * b.mat[3][0];

	retVal.mat[3][1] =
		mat[3][0] * b.mat[0][1] +
		mat[3][1] * b.mat[1][1] +
		mat[3][2] * b.mat[2][1] +
		mat[3][3] * b.mat[3][1];

	retVal.mat[3][2] =
		mat[3][0] * b.mat[0][2] +
		mat[3][1] * b.mat[1][2] +
		mat[3][2] * b.mat[2][2] +
		mat[3][3] * b.mat[3][2];

	retVal.mat[3][3] =
		mat[3][0] * b.mat[0][3] +
		mat[3][1] * b.mat[1][3] +
		mat[3][2] * b.mat[2][3] +
		mat[3][3] * b.mat[3][3];

	return retVal;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& m) {
	(*this) = (*this) * m;
	return (*this);
}

void Matrix4x4::Invert() {
	// Thanks slow math
	// This is a really janky way to unroll everything...
	float tmp[12];
	float src[16];
	float dst[16];
	float det;

	// Transpose matrix
	// row 1 to col 1
	src[0] = mat[0][0];
	src[4] = mat[0][1];
	src[8] = mat[0][2];
	src[12] = mat[0][3];

	// row 2 to col 2
	src[1] = mat[1][0];
	src[5] = mat[1][1];
	src[9] = mat[1][2];
	src[13] = mat[1][3];

	// row 3 to col 3
	src[2] = mat[2][0];
	src[6] = mat[2][1];
	src[10] = mat[2][2];
	src[14] = mat[2][3];

	// row 4 to col 4
	src[3] = mat[3][0];
	src[7] = mat[3][1];
	src[11] = mat[3][2];
	src[15] = mat[3][3];

	// Calculate cofactors
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	// Calculate determinant
	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

	// Inverse of matrix is divided by determinant
	det = 1 / det;
	for (int j = 0; j < 16; j++)
	{
		dst[j] *= det;
	}

	// Set it back
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat[i][j] = dst[i * 4 + j];
		}
	}
}

Vector3d Matrix4x4::GetTranslattion() const {
	return Vector3d(mat[3][0], mat[3][1], mat[3][2]);
}

Vector3d Matrix4x4::GetXAxis() const {
	return Vector3d::Normalize(Vector3d(mat[0][0], mat[0][1], mat[0][2]));
}


Vector3d Matrix4x4::GetYAxis() const {
	return Vector3d::Normalize(Vector3d(mat[1][0], mat[1][1], mat[1][2]));
}


Vector3d Matrix4x4::GetZAxis() const {
	return Vector3d::Normalize(Vector3d(mat[2][0], mat[2][1], mat[2][2]));
}

Vector3d Matrix4x4::GetScale() const {
	Vector3d retVal;
	retVal.x = Vector3d(mat[0][0], mat[0][1], mat[0][2]).Length();
	retVal.y = Vector3d(mat[1][0], mat[1][1], mat[1][2]).Length();
	retVal.z = Vector3d(mat[2][0], mat[2][1], mat[2][2]).Length();
	return retVal;
}

Matrix4x4 Matrix4x4::CreateScale(float xScale, float yScale, float zScale) {
	float temp[4][4] =
	{
		{ xScale, 0.0f, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f, 0.0f },
		{ 0.0f, 0.0f, zScale, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreateScale(const Vector3d& scaleVector) {
	return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
}

Matrix4x4 Matrix4x4::CreateScale(float scale) {
	return CreateScale(scale, scale, scale);
}

Matrix4x4 Matrix4x4::CreateRotationX(float theta) {
	float temp[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f , 0.0f },
		{ 0.0f, MathUtils::Cos(theta), MathUtils::Sin(theta), 0.0f },
		{ 0.0f, -MathUtils::Sin(theta), MathUtils::Cos(theta), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreateRotationY(float theta) {
	float temp[4][4] =
	{
		{ MathUtils::Cos(theta), 0.0f, -MathUtils::Sin(theta), 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ MathUtils::Sin(theta), 0.0f, MathUtils::Cos(theta), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreateRotationZ(float theta) {
	float temp[4][4] =
	{
		{ MathUtils::Cos(theta), MathUtils::Sin(theta), 0.0f, 0.0f },
		{ -MathUtils::Sin(theta), MathUtils::Cos(theta), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreateTranslation(const Vector3d& trans) {
	float temp[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ trans.x, trans.y, trans.z, 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreateLookAt(const Vector3d& eye, const Vector3d& target, const Vector3d& up)
{
	Vector3d zaxis = Vector3d::Normalize(target - eye);
	Vector3d xaxis = Vector3d::Normalize(Vector3d::Cross(up, zaxis));
	Vector3d yaxis = Vector3d::Normalize(Vector3d::Cross(zaxis, xaxis));
	Vector3d trans;
	trans.x = -Vector3d::Dot(xaxis, eye);
	trans.y = -Vector3d::Dot(yaxis, eye);
	trans.z = -Vector3d::Dot(zaxis, eye);

	float temp[4][4] =
	{
		{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
		{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
		{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
		{ trans.x, trans.y, trans.z, 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreateOrtho(float width, float height, float near, float far)
{
	float temp[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
		{ 0.0f, 0.0f, near / (near - far), 1.0f }
	};
	return Matrix4x4(temp);
}

Matrix4x4 Matrix4x4::CreatePerspectiveFOV(float fovY, float width, float height, float near, float far)
{
	float yScale = MathUtils::Cot(fovY / 2.0f);
	float xScale = yScale * height / width;
	float temp[4][4] =
	{
		{ xScale, 0.0f, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f, 0.0f },
		{ 0.0f, 0.0f, far / (far - near), 1.0f },
		{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
	};
	return Matrix4x4(temp);
}

// Create "Simple" View-Projection Matrix from Chapter 6
Matrix4x4 Matrix4x4::CreateSimpleViewProj(float width, float height)
{
	float temp[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};
	return Matrix4x4(temp);
}
#pragma once
#include <algorithm>
#include "Vector2.hpp"
#include <vector>
namespace Math {
	class Matrix3 {
	public:
		float mat[3][3];
		static const Matrix3 Identity;

		Matrix3() {
			*this = Matrix3::Identity;
		}

		explicit Matrix3(float inMat[3][3]) {
			std::copy(&mat[0][0], &mat[0][0] + 3 * 3, &inMat[0][0]);
		}

		// float pointer�� ��ȯ
		const float* GetAsFloatPtr() const {
			// reinterpret_cast : ������ ������ Ÿ�Գ��� ��ȯ�� ���
			return reinterpret_cast<const float*>(&mat[0][0]);
		}

		// Matrix ���ϱ�
		Matrix3 operator*(const Matrix3& m) {
			Matrix3 retVal;
			// dot product(����)���� ����.
			// row 0
			retVal.mat[0][0] =
				mat[0][0] * m.mat[0][0] +
				mat[0][1] * m.mat[1][0] +
				mat[0][2] * m.mat[2][0];

			retVal.mat[0][1] =
				mat[0][0] * m.mat[0][1] +
				mat[0][1] * m.mat[1][1] +
				mat[0][2] * m.mat[2][1];

			retVal.mat[0][2] =
				mat[0][0] * m.mat[0][2] +
				mat[0][1] * m.mat[1][2] +
				mat[0][2] * m.mat[2][2];

			// row 1
			retVal.mat[1][0] =
				mat[1][0] * m.mat[0][0] +
				mat[1][1] * m.mat[1][0] +
				mat[1][2] * m.mat[2][0];

			retVal.mat[1][1] =
				mat[1][0] * m.mat[0][1] +
				mat[1][1] * m.mat[1][1] +
				mat[1][2] * m.mat[2][1];

			retVal.mat[1][2] =
				mat[1][0] * m.mat[0][2] +
				mat[1][1] * m.mat[1][2] +
				mat[1][2] * m.mat[2][2];

			// row 2
			retVal.mat[2][0] =
				mat[2][0] * m.mat[0][0] +
				mat[2][1] * m.mat[1][0] +
				mat[2][2] * m.mat[2][0];

			retVal.mat[2][1] =
				mat[2][0] * m.mat[0][1] +
				mat[2][1] * m.mat[1][1] +
				mat[2][2] * m.mat[2][1];

			retVal.mat[2][2] =
				mat[2][0] * m.mat[0][2] +
				mat[2][1] * m.mat[1][2] +
				mat[2][2] * m.mat[2][2];

			return retVal;
		}

		Matrix3& operator*=(const Matrix3& m) {
			*this = *this * m;
			return *this;
		}

		// scale matrix�� �����.(ũ�⺯ȯ)
		static Matrix3 CreateScale(float xScale, float yScale) {
			float temp[3][3] =
			{
				{ xScale, 0.0f, 0.0f },
				{ 0.0f, yScale, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
			};
			return Matrix3(temp);
		}
		static Matrix3 CreateScale(const Vector2& scaleVector) {
			return CreateScale(scaleVector.x, scaleVector.y);
		}

		static Matrix3 CreateScale(const float scale) {
			return CreateScale(scale, scale);
		}

		// ȸ����ȯ�� ���� matrix
		static Matrix3 CreateRotation(float radian) {
			float temp[3][3] =
			{
				{ MathUtils::Cos(radian), MathUtils::Sin(radian), 0.0f },
				{ -MathUtils::Sin(radian), MathUtils::Cos(radian), 0.0f },
				{ 0.0f, 0.0f, 1.0f }
			};
			return Matrix3(temp);
		}

		// �̵���ȯ(xy-plane)
		static Matrix3 CreateTranslation(const Vector2& trans) {
			float temp[3][3] =
			{
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ trans.x, trans.y, 1.0f },
			};
			return Matrix3(temp);
		}

	};
}
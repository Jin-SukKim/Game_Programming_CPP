#pragma once
namespace Math {

	struct MathUtils {

		// 컴파일 타임에 값이 무조건 정해져있다고 알려준다.
		static constexpr float PI = 3.14159265358979323846f;
		static constexpr float TwoPI = PI * 2.0f;
		static constexpr float PIOver2 = PI / 2.0f;
		static constexpr float HalfPI = 1.57079632679f;
		static constexpr float InvPI = 0.31830988618f;

		inline static float ToRadians(float degress) {
			return degress * PI / 180.f;
		}

		inline static float ToDegrees(float radians) {
			return radians * 180.f / PI;
		}

		// 0에 가까운 극한
		inline static bool NearZero(float val, float epsilon = 0.0001f) {
			if (fabs(val) <= epsilon) {
				return true;
			}
			else {
				return false;
			}
		}

		template <typename T>
		T Max(const T& a, const T& b) {
			return a < b ? b : a;
		}

		template <typename T>
		inline static T Min(const T& a, const T& b) {
			return a < b ? a : b;
		}

		// 범위안에 값이 있는지 확인하고 값을 반환
		// 하지만 범위 밖에 있는 경우 최소값이나 최댓값을 반환
		// 즉, 좌표의 범위를 제한할 수 있다.
		template <typename T>
		inline static  T Clamp(const T& value, const T& lower, const T& upper) {
			return Min(upper, Max(lower, value));
		}

		inline static  float Abs(float value)
		{
			return fabs(value);
		}

		inline static  float Cos(float angle)
		{
			return cosf(angle);
		}

		inline static  float Sin(float angle)
		{
			return sinf(angle);
		}

		inline static  float Tan(float angle)
		{
			return tanf(angle);
		}

		inline static  float Acos(float value)
		{
			return acosf(value);
		}

		inline static  float Atan2(float y, float x)
		{
			return atan2f(y, x);
		}

		inline static  float Cot(float angle)
		{
			return 1.0f / Tan(angle);
		}

		inline static float Sqrt(float value) {
			return sqrt(value);
		}

		// 나머지 계산
		inline static float Fmod(float numer, float denom) {
			return fmod(numer, denom);
		}
	};
}
class BoneTransform
{
public:
    // 본의 회전값은 뒤에 나올 회전 보간을 위해 쿼터니언을 활용
    Quaternion mRotation;
    // 본의 위치값 벡터
    Vector3d mTranslation;
    // 행렬로 변환, 멤버 데이터를 사용해 회전 행렬과 이동 행렬을 만들어 곱한다.
    Matrix4x4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& a, 
		const BoneTransform& b, float f);
};
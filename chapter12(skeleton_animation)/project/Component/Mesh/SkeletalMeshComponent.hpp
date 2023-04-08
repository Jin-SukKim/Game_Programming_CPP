class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// 메시 컴포넌트를 그린다.
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) {
		mSkeleton = sk;
	}

	// 멤버 변수를 새롭게 설정한 뒤 행렬 팔레트 계산 함수를 호출하고
	// 애니메이션 재생 시간을 반환한다.
	float PlayAnimation(const class Animation* anim, float playRate = 1.f);

protected:
	// 현재 전역 포즈 행렬 * 전역 인버스 바인드 포즈 행렬 계산해 행렬 팔레트 항목을 반환
	void ComputeMatrixPalette();
	const class Skeleton* mSkeleton;

	// 행렬 팔레트
	MatrixPalette mPalette;
	// 현재 재생되고 있는 애니메이션
	const class Animation* mAnimation;
	// 애니메이션 재생 속도(1.0이 정상 속도)
	float mAnimPlayRate;
	// 애니메이션의 현재 시간
	float mAnimTime;
};
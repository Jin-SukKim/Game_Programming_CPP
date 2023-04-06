class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SetHorzDist(float dist) { 
		mHorzDist = dist; 
	}
	void SetVertDist(float dist) {
		mVertDist = dist;
	}
	void SetTargetDist(float dist) {
		mTargetDist = dist;
	}
	void SetSpringConstant(float spring) {
		mSpringConstant = spring;
	}

	// 카메라 초기화
	void SnapToIdeal();

private:
	Vector3d ComputeCameraPos() const;

	// 카메라 위치
	Vector3d mActualPos;
	// 카메라 속도
	Vector3d mVelocity;
	// 수평 거리
	float mHorzDist;
	// 수직 거리
	float mVertDist;
	// 타깃 거리
	float mTargetDist;
	// 스프링(탄력성)
	float mSpringConstant;
};
class MirrorCamera : public CameraComponent
{
public:
	MirrorCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
private:
	// 카메라 초기화
	Vector3d ComputeCameraPos() const;

	// 수평 거리
	float mHorzDist;
	// 수직 거리
	float mVertDist;
	// 타깃 거리
	float mTargetDist;
};
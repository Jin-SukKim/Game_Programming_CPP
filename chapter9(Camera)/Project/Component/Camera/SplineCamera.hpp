// 스플라인 구조체
struct Spline
{
	// 스플라인에 대한 제어점
	// (세그먼트상에 n개의 점이 있다면 총 n+2개의 점이 필요
	std::vector<Vector3d> mControlPoints;
	// startIdx = P1인 스플라인 세그먼트가 주어졌을 때
	// t값을 토대로 위치를 계싼
	Vector3d Compute(size_t startIdx, float t) const;
	size_t GetNumPoints() const {
		return mControlPoints.size();
	}
};

class SplineCamera : public CameraComponent
{
public:
	SplineCamera(class Actor* owner);

	void Update(float deltaTime) override;
	// 스플라인 재시작
	void Restart();

	void SetSpeed(float speed) {
		mSpeed = speed;
	}
	void SetSpline(const Spline& spline) {
		mPath = spline;
	}
	void SetPaused(bool pause) {
		mPaused = pause;
	}
private:
	// 스플라인 경로
	Spline mPath;
	// 현재 제어점 index
	size_t mIndex;
	// 제어점 t
	float mT;
	// 초당 t 변화율
	float mSpeed;
	// 카메라 이동 여부
	bool mPaused;

};
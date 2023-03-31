// ���ö��� ����ü
struct Spline
{
	// ���ö��ο� ���� ������
	// (���׸�Ʈ�� n���� ���� �ִٸ� �� n+2���� ���� �ʿ�
	std::vector<Vector3d> mControlPoints;
	// startIdx = P1�� ���ö��� ���׸�Ʈ�� �־����� ��
	// t���� ���� ��ġ�� ���
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
	// ���ö��� �����
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
	// ���ö��� ���
	Spline mPath;
	// ���� ������ index
	size_t mIndex;
	// ������ t
	float mT;
	// �ʴ� t ��ȭ��
	float mSpeed;
	// ī�޶� �̵� ����
	bool mPaused;

};
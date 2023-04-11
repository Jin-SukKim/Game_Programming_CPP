class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
	// 뷰 행렬을 렌더러와 오디오 시스템에 전달
	void SetViewMatrix(const Matrix4x4& view);
};
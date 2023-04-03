class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
	// �� ����� �������� ����� �ý��ۿ� ����
	void SetViewMatrix(const Matrix4x4& view);
};
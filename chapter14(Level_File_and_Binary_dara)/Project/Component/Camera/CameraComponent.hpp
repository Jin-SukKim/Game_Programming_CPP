class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);

	TypeID GetType() const override { return TCameraComponent; }
protected:
	// �� ����� �������� ����� �ý��ۿ� ����
	void SetViewMatrix(const Matrix4x4& view);
};
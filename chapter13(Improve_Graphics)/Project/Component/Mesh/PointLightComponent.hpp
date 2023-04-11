class PointLightComponent : public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();

	// ������ ���ϸ� �׸���.
	void Draw(class Shader* shader, class Mesh* mesh);

	// �л� ����
	Vector3d mDiffuseColor;
	// ������ ������
	float mInnerRadius;
	float mOuterRadius;
};
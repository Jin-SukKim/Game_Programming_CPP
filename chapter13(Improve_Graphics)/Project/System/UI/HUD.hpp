class HUD : public UIScreen
{
public:
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);

protected:
	void UpdateCrosshair(float deltaTime);
	void UpdateRadar(float deltaTime);

	class Texture* mHealthBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;

	// ��� Ÿ��
	std::vector<class TargetComponent*> mTargetComps;
	// ���̴��� ������� ��ȣ�� 2D ������
	std::vector<Vector2d> mBlips;
	// ���̴��� ����, ��������
	float mRadarRange;
	float mRadarRadius;
	// �������� ���� ���ϰ� �ִ���
	bool mTargetEnemy;

};
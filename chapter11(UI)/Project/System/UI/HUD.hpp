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

	// 모든 타겟
	std::vector<class TargetComponent*> mTargetComps;
	// 레이더에 상대적인 신호의 2D 오프셋
	std::vector<Vector2d> mBlips;
	// 레이더의 범위, 반지름값
	float mRadarRange;
	float mRadarRadius;
	// 조준점이 적을 향하고 있는지
	bool mTargetEnemy;

};
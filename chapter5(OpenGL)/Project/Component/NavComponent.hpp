// MoveComponent�� actor�� �չ������� �̵���ų �� �ִ�. ��δ� ������ �̷���� �ִ�.
// �ᱹ ������ ���� ���󰡸� ��θ� ���󰡴� ���̴�.
// NacComponent�� actor�� ��θ� ���� �̵��� �� �ְ� �Ѵ�.

class NavComponent : public MoveComponent {
public:
	NavComponent(class Actor* owner, int updateOrder = 10);

	// Acttor�� ��λ� ���� �̵� ������ �����ߴ��� ���θ� Ȯ��
	void Update(float deltaTime) override;

	// Ư�� ���� ���ϰ��� Actor�� ȸ����Ų��.
	void TurnTo(const Vector2d& pos);

	
	// ��λ��� ���� ������ ��ȯ
	Vector2d GetNextPoint();

	// ���� ����
	void StartPath(const class Tile* start);
private:
	Vector2d mNextPoint;
	const class Tile* mNextNode;
};

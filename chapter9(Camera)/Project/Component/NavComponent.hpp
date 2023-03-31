// MoveComponent�� actor�� �չ������� �̵���ų �� �ִ�. ��δ� ������ �̷���� �ִ�.
// �ᱹ ������ ���� ���󰡸� ��θ� ���󰡴� ���̴�.
// NacComponent�� actor�� ��θ� ���� �̵��� �� �ְ� �Ѵ�.

class NavComponent : public MoveComponent {
public:
	NavComponent(class Actor* owner, int updateOrder = 10);

	// Acttor�� ��λ� ���� �̵� ������ �����ߴ��� ���θ� Ȯ��
	void Update(float deltaTime) override;

	// Ư�� ���� ���ϰ��� Actor�� ȸ����Ų��.
	void TurnTo(const Vector3d& pos);


	// ��λ��� ���� ������ ��ȯ
	Vector3d GetNextPoint();

private:
	Vector2d mNextPoint;
};

// MoveComponent는 actor를 앞방향으로 이동시킬 수 있다. 경로는 점으로 이루어져 있다.
// 결국 점에서 점을 따라가면 경로를 따라가는 것이다.
// NacComponent는 actor가 경로를 따라 이동할 수 있게 한다.

class NavComponent : public MoveComponent {
public:
	NavComponent(class Actor* owner, int updateOrder = 10);

	// Acttor가 경로상 다음 이동 지점에 도달했는지 여부를 확인
	void Update(float deltaTime) override;

	// 특정 점을 향하고자 Actor를 회전시킨다.
	void TurnTo(const Vector2d& pos);

	
	// 경로상의 다음 지점을 반환
	Vector2d GetNextPoint();

	// 시작 지점
	void StartPath(const class Tile* start);
private:
	Vector2d mNextPoint;
	const class Tile* mNextNode;
};

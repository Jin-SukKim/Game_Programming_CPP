class Grid : public Actor {
public:
	Grid(class Game* game);

	// ���콺 Ŭ�� 
	void ProcessClick(int x, int y);

	// A* ����� ��ã��
	bool FindPath(class Tile* start, class Tile* goal);

	// Ÿ�� ����
	void BuildTower();

	// ���۰� �� Ÿ�� ���
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;

private:
	// Ÿ�� ����
	void SelectTile(size_t row, size_t col);

	// ����� �ؽ�ó ����
	void UpdatePathTiles(class Tile* start);

	// ���� ���õ� Ÿ��
	class Tile* mSelectedTile;

	// 2D ���� Ÿ�� �׸���
	std::vector<std::vector<class Tile*>> mTiles;

	// ���� ���� �ð�
	float mNextEnemy;

	// ��� ��
	const size_t NumRows = 7;
	const size_t NumCols = 16;

	// ���� �� y�� ���� ��ġ
	const float StartY = 192.f;
	// �� Ÿ���� ���̿� ����
	const float TileSize = 64.f;
	// �� ���� �ð�
	const float EnemyTime = 1.5f;
};
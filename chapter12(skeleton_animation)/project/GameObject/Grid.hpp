class Grid : public Actor {
public:
	Grid(class Game* game);

	// 마우스 클릭 
	void ProcessClick(int x, int y);

	// A* 사용해 길찾기
	bool FindPath(class Tile* start, class Tile* goal);

	// 타워 짓기
	void BuildTower();

	// 시작과 끝 타일 얻기
	class Tile* GetStartTile();
	class Tile* GetEndTile();

	void UpdateActor(float deltaTime) override;

private:
	// 타일 선택
	void SelectTile(size_t row, size_t col);

	// 경로의 텍스처 갱신
	void UpdatePathTiles(class Tile* start);

	// 현재 선택된 타일
	class Tile* mSelectedTile;

	// 2D 벡터 타일 그리드
	std::vector<std::vector<class Tile*>> mTiles;

	// 다음 적의 시간
	float mNextEnemy;

	// 행과 열
	const size_t NumRows = 7;
	const size_t NumCols = 16;

	// 왼쪽 위 y의 시작 위치
	const float StartY = 192.f;
	// 각 타일의 넓이와 높이
	const float TileSize = 64.f;
	// 각 적의 시간
	const float EnemyTime = 1.5f;
};
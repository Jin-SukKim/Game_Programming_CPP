#include "PreCompiled.hpp"

Grid::Grid(class Game* game) : Actor(game), mSelectedTile(nullptr) {
	// 7 rows, 16 cols
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++) {
		mTiles[i].resize(NumCols);
	}

	// 타일 생성
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(
				Vector2d(TileSize / 2.f + j * TileSize,
					StartY + i * TileSize));
		}
	}

	// 시작과 끝 타일 설정
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	// 이웃 lists 설정
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			if (i > 0) {
				mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
			}
			if (i < NumRows - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
			}
			if (j > 0)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
			}
			if (j < NumCols - 1)
			{
				mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
			}
		}
	}

	// 길 찾기(거꾸로)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col) {
	// 타일이 선택될 수 있는지 확인
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) {
		// 이전에 선택한걸 취소해준다.
		if (mSelectedTile) {
			mSelectedTile->ToggleSelect();
		}
		mSelectedTile = mTiles[row][col];
		mSelectedTile->ToggleSelect();
	}
}

void Grid::ProcessClick(int x, int y) {
	y -= static_cast<int>(StartY - TileSize / 2);
	if (y >= 0) {
		x /= static_cast<int>(TileSize);
		y /= static_cast<int>(TileSize);
		if (x >= 0 && x < static_cast<int>(NumCols)
			&& y >= 0 && y < static_cast<int>(NumRows))
		{
			SelectTile(y, x);
		}
	}
}

// A* 길찾기 알고리즘
bool Grid::FindPath(Tile* start, Tile* goal) {
	// 길찾기 알고리즘을 위한 초기화
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			mTiles[i][j]->g = 0.f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	// 현재 노드를 시작 노드로 설정
	Tile* current = start;
	// 닫힌 집합에 추가
	current->mInClosedSet = true;

	do {
		// 이웃 노드를 열린 집합에 추가
		for (Tile* neighbor : current->mAdjacent) {
			// 막힌 길
			if (neighbor->mBlocked) {
				continue;
			}
			// 닫힌 집합에 없는 노드만 검사
			if (!neighbor->mInClosedSet) {
				// 열린 집합에 없는 경우
				if (!neighbor->mInOpenSet) {
					// 열린 집합에 없으므로 현재 노드를 부모로 설정
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// g(x) 는 부모의 경로 비용 + 이웃 노드로 가는 비용
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				// 이미 열린 집합에 있다면
				else {
					// 현재 노드가 부모가 되면 g(x)의 비용 계산
					float newG = current->g + TileSize;
					if (newG < neighbor->g){
						neighbor->mParent = current;
						neighbor->g = newG;
						// g(x)가 바꼈으므로 f(x)를 다시 계산
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		// 열린 집합이 비었으면 가능한 경로가 없다는 뜻
		if (openSet.empty()) {
			break;
		}

		// 이웃 노드들 중 가장 적은 비용의 노드를 열린 집합에서 선택
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) {
				return a->f < b->EDefault;
			});

		// 가장 적은 비용의 노드를 현재 노드로 변경하고 닫힌 집합으로 변경
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;

	} while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start) {
	// 시작/끝 타일을 뺀 나머지 타일을 기본 상태로 초기화
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15)) {
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	
	// 길찾기 함수로 찾은 경로 타일 설정
	Tile* t = start->mParent;
	while (t != GetEndTile()) {
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

void Grid::BuildTower() {
	// 선택된 타일 중 막힌 타일이 아닌지 확인
	if (mSelectedTile && !mSelectedTile->mBlocked) {
		mSelectedTile->mBlocked = true;

		// 타워 건설하면 적의 경로가 다 막히는지 확인
		if (FindPath(GetEndTile(), GetStartTile())) {
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else {
			// 경로를 다 막는다면 건설하지 못하게 한다
			mSelectedTile->mBlocked = false;
			FindPath(GetEndTile(), GetStartTile());
		}
		UpdatePathTiles(GetStartTile());
	}
}

Tile* Grid::GetStartTile() {
	return mTiles[3][0];
}

Tile* Grid::GetEndTile() {
	return mTiles[3][15];
}

void Grid::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);

	// 적 스폰
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.f) {
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}
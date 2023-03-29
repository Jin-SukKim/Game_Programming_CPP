#include "PreCompiled.hpp"

Grid::Grid(class Game* game) : Actor(game), mSelectedTile(nullptr) {
	// 7 rows, 16 cols
	mTiles.resize(NumRows);
	for (size_t i = 0; i < mTiles.size(); i++) {
		mTiles[i].resize(NumCols);
	}

	// Ÿ�� ����
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			mTiles[i][j] = new Tile(GetGame());
			mTiles[i][j]->SetPosition(
				Vector2d(TileSize / 2.f + j * TileSize,
					StartY + i * TileSize));
		}
	}

	// ���۰� �� Ÿ�� ����
	GetStartTile()->SetTileState(Tile::EStart);
	GetEndTile()->SetTileState(Tile::EBase);

	// �̿� lists ����
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

	// �� ã��(�Ųٷ�)
	FindPath(GetEndTile(), GetStartTile());
	UpdatePathTiles(GetStartTile());

	mNextEnemy = EnemyTime;
}

void Grid::SelectTile(size_t row, size_t col) {
	// Ÿ���� ���õ� �� �ִ��� Ȯ��
	Tile::TileState tstate = mTiles[row][col]->GetTileState();
	if (tstate != Tile::EStart && tstate != Tile::EBase) {
		// ������ �����Ѱ� ������ش�.
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

// A* ��ã�� �˰���
bool Grid::FindPath(Tile* start, Tile* goal) {
	// ��ã�� �˰����� ���� �ʱ�ȭ
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			mTiles[i][j]->g = 0.f;
			mTiles[i][j]->mInOpenSet = false;
			mTiles[i][j]->mInClosedSet = false;
		}
	}

	std::vector<Tile*> openSet;

	// ���� ��带 ���� ���� ����
	Tile* current = start;
	// ���� ���տ� �߰�
	current->mInClosedSet = true;

	do {
		// �̿� ��带 ���� ���տ� �߰�
		for (Tile* neighbor : current->mAdjacent) {
			// ���� ��
			if (neighbor->mBlocked) {
				continue;
			}
			// ���� ���տ� ���� ��常 �˻�
			if (!neighbor->mInClosedSet) {
				// ���� ���տ� ���� ���
				if (!neighbor->mInOpenSet) {
					// ���� ���տ� �����Ƿ� ���� ��带 �θ�� ����
					neighbor->mParent = current;
					neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
					// g(x) �� �θ��� ��� ��� + �̿� ���� ���� ���
					neighbor->g = current->g + TileSize;
					neighbor->f = neighbor->g + neighbor->h;
					openSet.emplace_back(neighbor);
					neighbor->mInOpenSet = true;
				}
				// �̹� ���� ���տ� �ִٸ�
				else {
					// ���� ��尡 �θ� �Ǹ� g(x)�� ��� ���
					float newG = current->g + TileSize;
					if (newG < neighbor->g){
						neighbor->mParent = current;
						neighbor->g = newG;
						// g(x)�� �ٲ����Ƿ� f(x)�� �ٽ� ���
						neighbor->f = neighbor->g + neighbor->h;
					}
				}
			}
		}

		// ���� ������ ������� ������ ��ΰ� ���ٴ� ��
		if (openSet.empty()) {
			break;
		}

		// �̿� ���� �� ���� ���� ����� ��带 ���� ���տ��� ����
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[](Tile* a, Tile* b) {
				return a->f < b->EDefault;
			});

		// ���� ���� ����� ��带 ���� ���� �����ϰ� ���� �������� ����
		current = *iter;
		openSet.erase(iter);
		current->mInOpenSet = false;
		current->mInClosedSet = true;

	} while (current != goal);

	return (current == goal) ? true : false;
}

void Grid::UpdatePathTiles(class Tile* start) {
	// ����/�� Ÿ���� �� ������ Ÿ���� �⺻ ���·� �ʱ�ȭ
	for (size_t i = 0; i < NumRows; i++) {
		for (size_t j = 0; j < NumCols; j++) {
			if (!(i == 3 && j == 0) && !(i == 3 && j == 15)) {
				mTiles[i][j]->SetTileState(Tile::EDefault);
			}
		}
	}
	
	// ��ã�� �Լ��� ã�� ��� Ÿ�� ����
	Tile* t = start->mParent;
	while (t != GetEndTile()) {
		t->SetTileState(Tile::EPath);
		t = t->mParent;
	}
}

void Grid::BuildTower() {
	// ���õ� Ÿ�� �� ���� Ÿ���� �ƴ��� Ȯ��
	if (mSelectedTile && !mSelectedTile->mBlocked) {
		mSelectedTile->mBlocked = true;

		// Ÿ�� �Ǽ��ϸ� ���� ��ΰ� �� �������� Ȯ��
		if (FindPath(GetEndTile(), GetStartTile())) {
			Tower* t = new Tower(GetGame());
			t->SetPosition(mSelectedTile->GetPosition());
		}
		else {
			// ��θ� �� ���´ٸ� �Ǽ����� ���ϰ� �Ѵ�
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

	// �� ����
	mNextEnemy -= deltaTime;
	if (mNextEnemy <= 0.f) {
		new Enemy(GetGame());
		mNextEnemy += EnemyTime;
	}
}
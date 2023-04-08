class Tile : public Actor {
public:
	friend class Grid;
	enum TileState {
		EDefault,
		EPath,
		EStart,
		EBase
	};

	Tile(class Game* game);

	// 타일의 상태
	void SetTileState(TileState state);
	TileState GetTileState() const {
		return mTileState;
	}

	// 타일 선택
	void ToggleSelect();
	const Tile* GetParent() const {
		return mParent;
	}

private:
	// 길찾기용
	std::vector<Tile*> mAdjacent;
	Tile* mParent;
	float f;	// g + h
	float g;	// 실제 비용
	float h;	// 휴리스틱 비용
	bool mInOpenSet;
	bool mInClosedSet;
	bool mBlocked;

	void UpdateTexture();
	class SpriteComponent* mSprite;
	TileState mTileState;
	bool mSelected;
};
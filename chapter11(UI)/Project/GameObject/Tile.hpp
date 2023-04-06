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

	// Ÿ���� ����
	void SetTileState(TileState state);
	TileState GetTileState() const {
		return mTileState;
	}

	// Ÿ�� ����
	void ToggleSelect();
	const Tile* GetParent() const {
		return mParent;
	}

private:
	// ��ã���
	std::vector<Tile*> mAdjacent;
	Tile* mParent;
	float f;	// g + h
	float g;	// ���� ���
	float h;	// �޸���ƽ ���
	bool mInOpenSet;
	bool mInClosedSet;
	bool mBlocked;

	void UpdateTexture();
	class SpriteComponent* mSprite;
	TileState mTileState;
	bool mSelected;
};
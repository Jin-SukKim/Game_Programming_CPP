#include "PreCompiled.hpp"

Tile::Tile(class Game* game)
	: Actor(game), f(0.f), g(0.f), h(0.f),
	mBlocked(false), mSprite(nullptr), mTileState(EDefault), mSelected(false) 
{
	mSprite = new SpriteComponent(this);
	UpdateTexture();
}

// 타일의 상태
void Tile::SetTileState(TileState state) {
	mTileState = state;
	UpdateTexture();
}

void Tile::ToggleSelect() {
	mSelected = !mSelected;
	UpdateTexture();
}

void Tile::UpdateTexture() {
	std::string text;
	switch (mTileState)
	{
	case Tile::EPath:
		if (mSelected)
			text = "Assets/TileGreySelected.png";
		else
			text = "Assets/TileGrey.png";
		break;
	case Tile::EStart:
		text = "Assets/TileTan.png";
		break;
	case Tile::EBase:
		text = "Assets/TileGreen.png";
		break;
	case Tile::EDefault:
	default:
		if (mSelected)
			text = "Assets/TileBrownSelected.png";
		else
			text = "Assets/TileBrown.png";
		break;
	}

	mSprite->SetTexture(GetGame()->GetTexture(text));
}
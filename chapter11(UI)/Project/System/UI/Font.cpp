#include "PreCompiled.hpp"

Font::Font(class Game* game) :mGame(game) {}

Font::~Font() {}

bool Font::Load(const std::string& fileName)
{
	// 해당 폰트 사이즈를 지원
	// 생성하고자 하는 폰트 크기의 벡터를 정의한다
	std::vector<int> fontSizes =
	{
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};

	// 각 크기의 폰트 사이즈마다 한 번씩 TTF_OpenFont 호출
	for (auto& size : fontSizes)
	{
		// .ttf 파일로부터 특정한 크기로 폰트를 로드하며
		// 해당 크기와 일치하는 TTF_Font 폰트 데이터의 포인터를 반환한다.
		// 폰트 크기마다 한번 씩 호출
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}

		// 호출한 TTF_Font를 맵에 추가
		mFontData.emplace(size, font);
	}

	return true;
}

void Font::Unload()
{
	for (auto & font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}

Texture* Font::RenderText(const std::string& text,
	const Vector3d& color, int pointSize)
{
	Texture* texture = nullptr;
	// SDL_Color 타입으로 색상을 변환
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// 해당 크기의 폰트 데이터 검색
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = mGame->GetText(text);
		// 텍스트를 그린다(알파값으로 블렌딩됨
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
		
		if (surf != nullptr)
		{
			// SDL_surface 객체를 texture 객체로 변환
			texture = new Texture();
			texture->CreateFromSurface(surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsupported", pointSize);
	}

	return texture;

}
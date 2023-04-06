#include "PreCompiled.hpp"

Font::Font(class Game* game) :mGame(game) {}

Font::~Font() {}

bool Font::Load(const std::string& fileName)
{
	// �ش� ��Ʈ ����� ����
	// �����ϰ��� �ϴ� ��Ʈ ũ���� ���͸� �����Ѵ�
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

	// �� ũ���� ��Ʈ ������� �� ���� TTF_OpenFont ȣ��
	for (auto& size : fontSizes)
	{
		// .ttf ���Ϸκ��� Ư���� ũ��� ��Ʈ�� �ε��ϸ�
		// �ش� ũ��� ��ġ�ϴ� TTF_Font ��Ʈ �������� �����͸� ��ȯ�Ѵ�.
		// ��Ʈ ũ�⸶�� �ѹ� �� ȣ��
		TTF_Font* font = TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}

		// ȣ���� TTF_Font�� �ʿ� �߰�
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
	// SDL_Color Ÿ������ ������ ��ȯ
	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// �ش� ũ���� ��Ʈ ������ �˻�
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;
		const std::string& actualText = mGame->GetText(text);
		// �ؽ�Ʈ�� �׸���(���İ����� ������
		SDL_Surface* surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);
		
		if (surf != nullptr)
		{
			// SDL_surface ��ü�� texture ��ü�� ��ȯ
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
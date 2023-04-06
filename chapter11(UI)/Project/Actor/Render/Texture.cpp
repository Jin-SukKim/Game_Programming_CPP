#include "..\..\PreCompiled.hpp"

Texture::Texture() : mTextureID(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

// 텍스처 가져오기
bool Texture::Load(const std::string& fileName) {
	int channels = 0;

	// SOIL로 이미지 가져오기
	unsigned char* image = SOIL_load_image(fileName.c_str(),
		&mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

	// 성공적으로 가져왔는지 확인
	if (image == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	// 이미지가 사용하는 포맷을 채널수로 확인
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// OpenGL의 텍스처 오브젝트 생성
	glGenTextures(1, &mTextureID);
	// 텍스처 오브젝트 활성화
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// 이미지 데이터를 텍스처 오브젝트에 복사
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, image);

	// SOIL에서 가져온 이미지 데이터 해제
	SOIL_free_image_data(image);

	// 이중 선형 필터링 활성화
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() {
	// 텍스처 오브젝트 삭제
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive() {
	// 텍스처 오브젝트 활성화
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mWidth = surface->w;
	mHeight = surface->h;

	// GL 텍스처 생성
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, surface->pixels);

	// 이중 선형 필터링 활성화
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
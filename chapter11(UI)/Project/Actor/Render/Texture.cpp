#include "..\..\PreCompiled.hpp"

Texture::Texture() : mTextureID(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

// �ؽ�ó ��������
bool Texture::Load(const std::string& fileName) {
	int channels = 0;

	// SOIL�� �̹��� ��������
	unsigned char* image = SOIL_load_image(fileName.c_str(),
		&mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

	// ���������� �����Դ��� Ȯ��
	if (image == nullptr)
	{
		SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	// �̹����� ����ϴ� ������ ä�μ��� Ȯ��
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// OpenGL�� �ؽ�ó ������Ʈ ����
	glGenTextures(1, &mTextureID);
	// �ؽ�ó ������Ʈ Ȱ��ȭ
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// �̹��� �����͸� �ؽ�ó ������Ʈ�� ����
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, image);

	// SOIL���� ������ �̹��� ������ ����
	SOIL_free_image_data(image);

	// ���� ���� ���͸� Ȱ��ȭ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload() {
	// �ؽ�ó ������Ʈ ����
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive() {
	// �ؽ�ó ������Ʈ Ȱ��ȭ
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mWidth = surface->w;
	mHeight = surface->h;

	// GL �ؽ�ó ����
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, surface->pixels);

	// ���� ���� ���͸� Ȱ��ȭ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
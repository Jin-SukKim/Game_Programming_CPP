#include "..\..\PreCompiled.hpp"

GBuffer::GBuffer() : mBufferID(0) {}

GBuffer::~GBuffer() {}

// 지정된 너비와 높이로 G 버퍼를 생성한다.
bool GBuffer::Create(int width, int height)
{
	// 프레임 버퍼 개체 생성하고 mBufferID에 저장
	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

	// 깊이 버퍼 추가
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// G 버퍼의 각 출력을 위한 텍스처를 생성
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		Texture* tex = new Texture();
		// 각 텍스처는 32비트 float 요소 필요
		tex->CreateForRendering(width, height, GL_RGB32F);
		mTextures.emplace_back(tex);
		// 생성한 텍스처를 색상 출력으로 사용
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
			tex->GetTextureID(), 0);
	}

	// 색상 출력 벡터 생성
	std::vector<GLenum> attachments;
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}
	// 프래그먼트 셰이더가 그릴 버퍼 리스트 설정
	glDrawBuffers(static_cast<GLsizei>(attachments.size()),
		attachments.data());

	// 제ㅐㄷ로 설정했는지 검증
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE)
	{
		Destroy();
		return false;
	}

	return true;
}

void GBuffer::Destroy()
{
	glDeleteFramebuffers(1, &mBufferID);
	for (Texture* t : mTextures)
	{
		t->Unload();
		delete t;
	}
}

Texture* GBuffer::GetTexture(Type type)
{
	if (mTextures.size() > 0)
	{
		return mTextures[type];
	}
	else
	{
		return nullptr;
	}
}

void GBuffer::SetTexturesActive()
{
	for (int i = 0; i < NUM_GBUFFER_TEXTURES; i++)
	{
		mTextures[i]->SetActive(i);
	}
}
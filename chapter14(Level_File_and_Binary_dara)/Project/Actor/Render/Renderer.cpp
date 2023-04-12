#include "..\..\PreCompiled.hpp"

Renderer::Renderer(Game* game) 
	: mGame(game), mSpriteShader(nullptr), 
	mMeshShader(nullptr), mSkinnedShader(nullptr), mMirrorBuffer(0)
	,mMirrorTexture(nullptr), mGBuffer(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float screenWidth, float screenHeigth) {
	mScreenHeight = screenHeigth;
	mScreenWidth = screenWidth;

	// OpenGL Attribute setting
	// OpenGL profile�� �ھ� �������Ϸ� ����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	// OpenGL ���� 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// ���� ���۴� RGBA ä�� ���
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ���� ���۸� Ȱ��ȭ
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL�� �ϵ���� ������ ����ϵ��� ����(GPU ���)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// ������ ����
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 12) - Skeleton",
		100,	// ������ ���� ��� x��ǥ
		100,	// ������ ���� ��� y��ǥ
		static_cast<int>(mScreenWidth),	// ������ �ʺ�
		static_cast<int>(mScreenHeight), // ������ ����
		SDL_WINDOW_OPENGL // OpenGL ���
	);

	// ������ ���� ���н�
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// ������ �����츦 OpenGL Context�� ����
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW �ʱ�ȭ
	// �Ϻ� �÷������� �ھ� context ���� �߻��ϴ� �ʱ�ȭ ���� ����
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// �Ϻ� �÷��������� GLEW�� ���� �ڵ带 �������Ƿ� ������ ����
	glGetError();

	// ���̴�
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// ���� �迭 ��ü ����
	CreateSpriteVerts();

	// ��̷� �ؽ�ó�� �����ӹ��� ����
	if (!CreateMirrorTarget())
	{
		SDL_Log("Failed to create render target for mirror.");
		return false;
	}

	// GBuffer ��ü ����
	mGBuffer = new GBuffer();
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	if (!mGBuffer->Create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}

	// ���� �޽�
	mPointLightMesh = GetMesh("Assets/PointLight.gpmesh");

	return true;
}

bool Renderer::LoadShaders() {
	// ��������Ʈ ���̴� ����(2D)
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	// ���̴� Ȱ��ȭ
	mSpriteShader->SetActive();

	// ��-���� ��� ����
	Matrix4x4 viewProj = Matrix4x4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	// vertex shader�� ����
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// �޽� ���̴� ����(3D)
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	// ���̴� Ȱ��ȭ
	mMeshShader->SetActive();

	// ��-���� ��� ����
	// �����
	mView = Matrix4x4::CreateLookAt(
		Vector3d::Zero, // ī�޶� ��ġ
		Vector3d::UnitX, // Ÿ�� ��ġ
		Vector3d::UnitZ); // ���� ����
	// �������
	mProjection = Matrix4x4::CreatePerspectiveFOV(
		MathUtils::ToRadians(70.f),	// ���� FOV
		mScreenWidth, // ���� �ʺ�
		mScreenHeight, // ���� ����
		10.f,		// ����� ������ �Ÿ� 
		10000.f	// �� ������ �Ÿ�
	);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// skinned shader ����
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	// ��Ű�� �޽� �׸���
	mSkinnedShader->SetActive();
	// ��-���� ��� ����
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// GBuffe�� ���̴� ����
	mGGlobalShader = new Shader();
	if (!mGGlobalShader->Load("Shaders/GBufferGlobal.vert", 
		"Shaders/GBufferGlobal.frag"))
	{
		return false;
	}

	// G ���۸� ���̴��� �����ϵ��� �ε��� ������ �� ���÷��� ������Ų��.
	mGGlobalShader->SetActive();
	// frag shader�� ����� 3���� sampler2D uniform�� �����Ѵ�.
	mGGlobalShader->SetIntUniform("uGDiffuse", 0);
	mGGlobalShader->SetIntUniform("uGNormal", 1);
	mGGlobalShader->SetIntUniform("uGWorldPos", 2);

	// ��-���� ����� �ϳ��� ��������Ʈ�� �׸��� ���� ���
	mGGlobalShader->SetMatrixUniform("uViewProj", viewProj);
	// ���� ��ȯ�� ȭ�鿡 �°� ��������Ʈ�� �����ϰ� y���� ������Ų��.
	// (�ſ� �ؽ�ó�� �׸� �� ó�� �������� y���� �ذ��� ����)
	Matrix4x4 gbufferWorld = Matrix4x4::CreateScale(mScreenWidth,
		-mScreenHeight, 1.f);
	mGGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);

	//  ���� ���̴�
	mGPointLightShader = new Shader();
	if (!mGPointLightShader->Load("Shaders/BasicMesh.vert",
		"Shaders/GBufferPointLight.frag"))
	{
		return false;
	}

	// ���� ����
	mGPointLightShader->SetActive();
	mGPointLightShader->SetIntUniform("uGDiffuse", 0);
	mGPointLightShader->SetIntUniform("uGNormal", 1);
	mGPointLightShader->SetIntUniform("uGWorldPos", 2);
	mGPointLightShader->SetVector2Uniform("uScreenDimensions",
		Vector2d(mScreenWidth, mScreenHeight));
	
	return true;
}

void Renderer::CreateSpriteVerts()
{
	// x, y, z ��ǥ, ��������,  UV ��ǥ
	float vertices[] = {
		-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, // top left, vertex 0
		 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f,// top right, vertex 1
		 0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f,// bottom right, vertex 2
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f // bottom left, vertex 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// ������ �ε��� ���� ������ ��������Ʈ �簢���� �迭�̴�.
	// ���� ���ۿ� 4���� ������, �ε��� ���ۿ� 6���� �ε����� �ִ�.
	// (�簢������ 2���� �ﰢ�� ����)
	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::ShutDown() {
	// ��̷� �ؽ�ó ����
	if (mMirrorTexture != nullptr)
	{
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mMirrorTexture->Unload();
		delete mMirrorTexture;
	}
	// GBuffer ����
	if (mGBuffer != nullptr)
	{
		mGBuffer->Destroy();
		delete mGBuffer;
	}

	// ���� ����
	while (!mPointLights.empty())
	{
		delete mPointLights.back();
	}

	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnLoadData() {
	// �ؽ�ó ����
	for (auto i : mTextures) {
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
	// �޽� ����
	for (auto i : mMeshes) {
		i.second->UnLoad();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::AddSprite(SpriteComponent* sprite) {
	// ���ĵ� ���Ϳ��� �����ؾ� �� ��ġ�� ã�´�.
	// �׸��� ������ ���� �־��ش�.
	int myDrawOrder = sprite->GetDrawOrder();

	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// ������ ��ġ �տ� ��Ҹ� ����
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
	 // swap�� ������ �����߸��Ƿ� ���Ѵ�.
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	// �ҷ��� texture�� �̹� �ִ��� Ȯ��
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		// �ʿ� �����ϸ� return
		tex = iter->second;
	}
	// ���ٸ� �ε��ؼ� �����´�.
	else
	{
		tex = new Texture();
		if (tex->Load(fileName)) {
			// Load�� �ؽ�ó�� �ʿ� �߰�
			mTextures.emplace(fileName, tex);
		}
		// �ε��ϴµ� ����
		else
		{
			delete tex;
			tex = nullptr;
		}
	}

	return tex;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	// �ҷ��� �޽ð� �̹� �ִ��� Ȯ��
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end()) {
		// �ʿ� �����Ѵٸ� �ִ� �� return
		m = iter->second;
	}
	// ���ٸ� load�ؼ� ��������
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			// Load�� �޽ø� �ʿ� �߰�
			mMeshes.emplace(fileName, m);
		}
		// �ε��ϴµ� ����
		else
		{
			delete m;
			m = nullptr;
		}
	}

	return m;
}

void Renderer::AddMeshComp(MeshComponent* mesh) {
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else
	{
		mMeshComps.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else {
		auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(iter);
	}
}

void Renderer::Draw()
{
	// ���� �ſ� �ؽ�ó�� �׸��� (����Ʈ �����ϰ�: 0.25)
	Draw3DScene(mMirrorBuffer, mMirrorView, mProjection, 0.25f);
	// 3D ����� G ���ۿ� �׸���.
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, 1.0f, false);
	// ������ ���۸� 0���� �ǵ�����(ȭ�� ������ ����)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// G ���۸� ���� ȭ���� �׸���.
	DrawFromGBuffer();

	// ��� ��������Ʈ ������Ʈ �׸���
	// ���� ���� ��Ȱ��ȭ
	glDisable(GL_DEPTH_TEST);
	// ���� ���� Ȱ��ȭ
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// ���̴� Ȱ��ȭ
	mSpriteShader->SetActive();
	// vertex ���� Ȱ��ȭ
	mSpriteVerts->SetActive();

	// ��� ��������Ʈ �׸���
	for (auto sprite : mSprites)
	{
		//if (sprite->GetVisible()) {
			sprite->Draw(mSpriteShader);
		//}
	}

	// UI screen �׸���
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mSpriteShader);
	}

	// ���� ��ȯ
	SDL_GL_SwapWindow(mWindow);

}

// �� �ݻ�
void Renderer::SetLightUniforms(Shader* shader, const Matrix4x4& view)
{
	// ī�޶� ��ġ�� invert�� view ���(�����)���� ���� �� �ִ�.
	Matrix4x4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// �ֺ���
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	// ���Ɽ
	
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);
}

Vector3d Renderer::Unproject(const Vector3d& screenPoint) const
{
	// ȭ�� ��ǥ�� ��ġ ��ǥ�� ��ȯ (-1���� +1 ����)
	Vector3d deviceCoord = screenPoint;
	// x = [-screenWidth/2, +screenWidth/2]
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	// y = [-screenHeight/2, +screenHeight/2]
	deviceCoord.y /= (mScreenHeight) * 0.5f;
	// z = [0, 1)
	Matrix4x4 v = mView;
	Matrix4x4 p = mProjection;
	// ���͸� ���������� ��ķ� ��ȯ
	Matrix4x4 unprojection = v * p;
	unprojection.Invert();
	return Vector3d::TransformWithPersDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3d& outStart, Vector3d& outDir) const
{
	// �������� ��´� (����� ����� ȭ����� �߽�)
	Vector3d screenPoint(0.f, 0.f, 0.f);
	outStart = Unproject(screenPoint);
	// ������ ��´� (����� ���� �� ��� ���̿� �ִ� ȭ����� �߽���)
	screenPoint.z = 0.9f;
	Vector3d end = Unproject(screenPoint);

	// ���� ���͸� ��´�.
	outDir = end - outStart;
	outDir.Normalize();
}

bool Renderer::CreateMirrorTarget()
{
	// ȭ�� ũ���� 4���� 1�� ũ��� ��̷� ũ�⸦ ����
	int width = static_cast<int>(mScreenWidth) / 4;
	int height = static_cast<int>(mScreenHeight) / 4;

	// �ſ� �ؽ�ó�� ���� ������ ���� ����
	glGenFramebuffers(1, &mMirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mMirrorBuffer);

	// ���� ���� �߰�
	GLuint depthBuffer;
	// ���� ���� ����
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// ������ ���ۿ� ���� ���� �ٿ��ش�.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// �������� ���� ����� �ؽ�ó ����
	mMirrorTexture = new Texture();
	// �ؽ�ó�� ���� RGB ���� ��û
	mMirrorTexture->CreateForRendering(width, height, GL_RGB);

	// �ſ� �ؽ�ó�� ������ ������ ��� Ÿ������ ����
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		mMirrorTexture->GetTextureID(), 0);

	// �� ������ ���۰� �׸��� ��� ���� ����Ʈ ����
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);	// �ſ� �ؽ�ó�� ������ �׸��ٰ� �˸�

	// ��� ������ ����� �ƴ��� Ȯ��
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// ������ �߻��ߴٸ� ������ ���� ����
		// �ؽ�ó �����ϰ� false return
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mMirrorTexture->Unload();
		delete mMirrorTexture;
		mMirrorTexture = nullptr;
		return false;
	}

	return true;

}

void Renderer::Draw3DScene(unsigned int framebuffer,
	const Matrix4x4& view, const Matrix4x4& proj, float viewPortScale, bool lit)
{
	// ���� ������ ���۸� ����
	// ��̷� �ؽ�ó�� ���� ���� ������ ���۸� ���
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// ����Ʈ �����ϰ����� ����Ʈ ũ�� ����(��üũ���� 1/4)
	glViewport(0, 0,
		static_cast<int>(mScreenWidth * viewPortScale),
		static_cast<int>(mScreenHeight * viewPortScale)
	);

	// Ŭ���� ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// ���� ����/���� ���� Ŭ����
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �޽� ������Ʈ �׸���
	// ���� ���� Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	// ���� ���� ��Ȱ��ȭ
	glDisable(GL_BLEND);

	// �⺻ �޽� ���̴� Ȱ��ȭ
	mMeshShader->SetActive();
	// ��-���� ��� ����
	mMeshShader->SetMatrixUniform("uViewProj", view * proj);

	// ���� ȿ��
	if (lit)
	{
		SetLightUniforms(mMeshShader, view);
	}
	for (auto mc : mMeshComps)
	{
		//if (mc->GetVisible()) {
		mc->Draw(mMeshShader);
		//}
	}

	// ��� skinned mesh �׸���
	mSkinnedShader->SetActive();
	// ��-���� ���
	mSkinnedShader->SetMatrixUniform("uViewProj", view * proj);
	// �� uniform ����
	if (lit)
	{
		SetLightUniforms(mSkinnedShader, view);
	}
	for (auto sk : mSkeletalMeshes)
	{
		//if (sk->GetVisible())
		//{
		sk->Draw(mSkinnedShader);
		//}
	}
}

void Renderer::DrawFromGBuffer()
{
	// ���� �׽�Ʈ ��Ȱ��ȭ (���� ���� ȿ���� ����)
	glDisable(GL_DEPTH_TEST);
	// ���� G ���� ���̴� Ȱ��ȭ
	mGGlobalShader->SetActive();
	// ��������Ʈ�� ���� ���ؽ� �簢�� Ȱ��ȭ
	mSpriteVerts->SetActive();
	// ���ø��Ϸ��� G ���� �ؽ�ó Ȱ��ȭ
	mGBuffer->SetTexturesActive();
	// ���� uniform ����
	SetLightUniforms(mGGlobalShader, mView);

	// �簢���� �׸���.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// G ������ ���� ���۸� �⺻ ������ ���۷� ����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// ���� �׽�Ʈ Ȱ��ȭ, ������ ���� ���۷��� ����� ��Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// ���� ���̴��� �޽ø� Ȱ��ȭ
	mGPointLightShader->SetActive();
	mPointLightMesh->GetVertexArray()->SetActive();

	// ��-���� ��� ����
	mGPointLightShader->SetMatrixUniform("uViewProj",
		mView * mProjection);
	// ���ø��� ���� G ���� �ؽ�ó ����
	mGBuffer->SetTexturesActive();

	// ���� ������ ���� ���� ���ؾ� �Ѵ�.
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// ������ �׸���.
	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mGPointLightShader, mPointLightMesh);
	}

}

void Renderer::AddPointLight(PointLightComponent* light)
{
	mPointLights.emplace_back(light);
}

void Renderer::RemovePointLight(PointLightComponent* light)
{
	auto iter = std::find(mPointLights.begin(), mPointLights.end(), light);
	mPointLights.erase(iter);
}

#include "..\..\PreCompiled.hpp"


Renderer::Renderer(Game* game) 
	: mGame(game), mSpriteShader(nullptr), 
	mMeshShader(nullptr), mSkinnedShader(nullptr) {}

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
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
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
		25.f,		// ����� ������ �Ÿ� 
		10000.f	// �� ������ �Ÿ�
	);

	// skinned shader ����
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	// ��Ű�� �޽� �׸���
	mSkinnedShader->SetActive();
	// ��-���� ��� ����
	mView = Matrix4x4::CreateLookAt(Vector3d::Zero, Vector3d::UnitX, Vector3d::UnitZ);
	mProjection = Matrix4x4::CreatePerspectiveFOV(MathUtils::ToRadians(70.0f),
		mScreenWidth, mScreenHeight, 10.0f, 10000.0f);
	
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);
	
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
	// clear ���� ����
	glClearColor(0.f, 0.f, 0.f, 1.f);	// ȸ��
	// ���� ���ۿ� ���� ���� �ʱ�ȭ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// �޽� ������Ʈ �׸���

	// ���� ���� Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	// ���� ���� ��Ȱ��ȭ
	glDisable(GL_BLEND);

	// �⺻ �޽� ���̴� Ȱ��ȭ
	mMeshShader->SetActive();
	// ��-���� ��� ����
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// ���� ȿ��
	SetLightUniforms(mMeshShader);

	for (auto mc : mMeshComps)
	{
		//if (mc->GetVisible()) {
			mc->Draw(mMeshShader);
		//}
	}

	// ��� skinned mesh �׸���
	mSkinnedShader->SetActive();
	// ��-���� ���
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// �� uniform ����
	SetLightUniforms(mSkinnedShader);
	for (auto sk : mSkeletalMeshes)
	{
		//if (sk->GetVisible())
		//{
			sk->Draw(mSkinnedShader);
		//}
	}

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
void Renderer::SetLightUniforms(Shader* shader)
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
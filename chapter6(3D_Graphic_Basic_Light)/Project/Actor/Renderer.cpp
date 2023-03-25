#include "PreCompiled.hpp"


Renderer::Renderer(Game* game) 
	: mGame(game), mSpriteShader(nullptr), mMeshShader(nullptr) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(float screenWidth, float screenHeigth) {
	mScreenHeight = screenHeigth;
	mScreenWidth = screenWidth;

	// OpenGL Attribute setting
	// OpenGL profile을 코어 프로파일로 설정
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	// OpenGL 버전 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// 색상 버퍼는 RGBA 채널 사용
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);

	// 더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL이 하드웨어 가속을 사용하도록 강제(GPU 사용)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// 윈도우 생성
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6) - OpenGL",
		100,	// 윈도우 좌측 상단 x좌표
		100,	// 윈도우 좌측 상단 y좌표
		static_cast<int>(mScreenWidth),	// 윈도우 너비
		static_cast<int>(mScreenHeight), // 윈도우 높이
		SDL_WINDOW_OPENGL // OpenGL 사용
	);

	// 윈도우 생성 실패시
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// 생성한 윈도우를 OpenGL Context에 지정
	mContext = SDL_GL_CreateContext(mWindow);

	// GLEW 초기화
	// 일부 플랫폼에서 코어 context 사용시 발생하는 초기화 에러 방지
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// 일부 플랫폼에서는 GLEW는 에러 코드를 내보내므로 에러값 제거
	glGetError();

	// 셰이더
	if (!LoadShaders()) {
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// 정점 배열 개체 생성
	CreateSpriteVerts();

	return true;
}

bool Renderer::LoadShaders() {
	// 스프라이트 셰이더 생성(2D)
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	// 셰이더 활성화
	mSpriteShader->SetActive();

	// 뷰-투영 행렬 생성
	Matrix4x4 viewProj = Matrix4x4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	// vertex shader에 전달
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// 메시 셰이더 생성(3D)
	mMeshShader = new Shader();
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	// 셰이더 활성화
	mMeshShader->SetActive();

	// 뷰-투영 행렬 생성

	// 뷰행렬
	mView = Matrix4x4::CreateLookAt(
		Vector3d::Zero, // 카메라 위치
		Vector3d::UnitX, // 타깃 위치
		Vector3d::UnitZ); // 상향 벡터
	// 투영행렬
	mProjection = Matrix4x4::CreatePerspectiveFOV(
		MathUtils::ToRadians(70.f),	// 수평 FOV
		mScreenWidth, // 뷰의 너비
		mScreenHeight, // 뷰의 높이
		25.f,		// 가까운 평면과의 거리 
		10000.f	// 먼 평면과의 거리
	);
	mMeshShader->SetMatrixUniform("uVeiwProj", mView * mProjection);

	return true;
}

void Renderer::CreateSpriteVerts()
{
	// x, y, z 좌표, 법선벡터,  UV 좌표
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

	// 정점과 인덱스 버퍼 변수는 스프라이트 사각형의 배열이다.
	// 정점 버퍼에 4개의 정점이, 인덱스 버퍼에 6개의 인덱스가 있다.
	// (사각형에는 2개의 삼각형 존재)
	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
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
	// 텍스처 삭제
	for (auto i : mTextures) {
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
	// 메시 삭제
	for (auto i : mMeshes) {
		i.second->UnLoad();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::AddSprite(SpriteComponent* sprite) {
	// 정렬된 벡터에서 삽입해야 할 위치를 찾는다.
	// 그리는 순서에 따라 넣어준다.
	int myDrawOrder = sprite->GetDrawOrder();

	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// 반족자 위치 앞에 요소를 삽입
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
	 // swap은 순서를 망가뜨리므로 못한다.
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	// 불러올 texture가 이미 있는지 확인
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		// 맵에 존재하면 return
		tex = iter->second;
	}
	// 없다면 로드해서 가져온다.
	else
	{
		tex = new Texture();
		if (tex->Load(fileName)) {
			// Load한 텍스처를 맵에 추가
			mTextures.emplace(fileName, tex);
		}
		// 로드하는데 실패
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
	// 불러올 메시가 이미 있는지 확인
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end()) {
		// 맵에 존재한다면 있는 걸 return
		m = iter->second;
	}
	// 없다면 load해서 가져오기
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			// Load한 메시를 맵에 추가
			mMeshes.emplace(fileName, m);
		}
		// 로드하는데 실패
		else
		{
			delete m;
			m = nullptr;
		}
	}

	return m;
}

void Renderer::AddMeshComp(MeshComponent* mesh) {
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

void Renderer::Draw()
{
	// clear 색상 설정
	glClearColor(0.f, 0.f, 0.f, 1.f);	// 회색
	// 색상 버퍼와 깊이 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 메시 컴포넌트 그리기

	// 깊이 버퍼 활성화
	glEnable(GL_DEPTH_TEST);
	// 알파 블렌디 비활성화
	glDisable(GL_BLEND);

	// 기본 메시 셰이더 활성화
	mMeshShader->SetActive();
	// 뷰-투영 행렬 갱신
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// 조명 효과
	SetLightUniforms(mMeshShader);

	for (auto mc : mMeshComps)
	{
		mc->Draw(mMeshShader);
	}

	// 모든 스프라이트 컴포넌트 그리기
	
	// 깊이 버퍼 비활성화
	glDisable(GL_DEPTH_TEST);
	// 알파 블렌딩 활성화
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// 셰이더 활성화
	mSpriteShader->SetActive();
	// vertex 버퍼 활성화
	mSpriteVerts->SetActive();

	// 모든 스프라이트 그리기
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// 버퍼 교환
	SDL_GL_SwapWindow(mWindow);

}

// 빛 반사
void Renderer::SetLightUniforms(Shader* shader)
{
	// 카메라 위치는 invert된 view 행렬(역행렬)에서 얻을 수 있다.
	Matrix4x4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// 주변광
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	// 방향광
	
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);
}
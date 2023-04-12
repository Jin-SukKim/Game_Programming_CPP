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
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// 더블 버퍼링 활성화
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL이 하드웨어 가속을 사용하도록 강제(GPU 사용)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// 윈도우 생성
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 12) - Skeleton",
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
	if (glewInit() != GLEW_OK)
	{
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

	// 백미러 텍스처와 프레임버퍼 생성
	if (!CreateMirrorTarget())
	{
		SDL_Log("Failed to create render target for mirror.");
		return false;
	}

	// GBuffer 객체 생성
	mGBuffer = new GBuffer();
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	if (!mGBuffer->Create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}

	// 점광 메시
	mPointLightMesh = GetMesh("Assets/PointLight.gpmesh");

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
	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag"))
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
		10.f,		// 가까운 평면과의 거리 
		10000.f	// 먼 평면과의 거리
	);
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// skinned shader 생성
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag"))
	{
		return false;
	}

	// 스키닝 메시 그리기
	mSkinnedShader->SetActive();
	// 뷰-투영 행렬 갱신
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// GBuffe용 셰이더 생성
	mGGlobalShader = new Shader();
	if (!mGGlobalShader->Load("Shaders/GBufferGlobal.vert", 
		"Shaders/GBufferGlobal.frag"))
	{
		return false;
	}

	// G 버퍼를 셰이더에 제공하도록 인덱스 값으로 각 샘플러를 연관시킨다.
	mGGlobalShader->SetActive();
	// frag shader에 사용할 3개의 sampler2D uniform과 연결한다.
	mGGlobalShader->SetIntUniform("uGDiffuse", 0);
	mGGlobalShader->SetIntUniform("uGNormal", 1);
	mGGlobalShader->SetIntUniform("uGWorldPos", 2);

	// 뷰-투영 행렬은 하나의 스프라이트를 그리기 위한 행렬
	mGGlobalShader->SetMatrixUniform("uViewProj", viewProj);
	// 세계 변환은 화면에 맞게 스프라이트를 조정하고 y값을 반전시킨다.
	// (거울 텍스처를 그릴 때 처럼 뒤집혀진 y문제 해결을 위해)
	Matrix4x4 gbufferWorld = Matrix4x4::CreateScale(mScreenWidth,
		-mScreenHeight, 1.f);
	mGGlobalShader->SetMatrixUniform("uWorldTransform", gbufferWorld);

	//  점광 셰이더
	mGPointLightShader = new Shader();
	if (!mGPointLightShader->Load("Shaders/BasicMesh.vert",
		"Shaders/GBufferPointLight.frag"))
	{
		return false;
	}

	// 변수 세팅
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
	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::ShutDown() {
	// 백미러 텍스처 삭제
	if (mMirrorTexture != nullptr)
	{
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mMirrorTexture->Unload();
		delete mMirrorTexture;
	}
	// GBuffer 삭제
	if (mGBuffer != nullptr)
	{
		mGBuffer->Destroy();
		delete mGBuffer;
	}

	// 점광 삭제
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
	// 먼저 거울 텍스처를 그린다 (뷰포트 스케일값: 0.25)
	Draw3DScene(mMirrorBuffer, mMirrorView, mProjection, 0.25f);
	// 3D 장면을 G 버퍼에 그린다.
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, 1.0f, false);
	// 프레임 버퍼를 0으로 되돌린다(화면 프레임 버퍼)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// G 버퍼를 통해 화면을 그린다.
	DrawFromGBuffer();

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
		//if (sprite->GetVisible()) {
			sprite->Draw(mSpriteShader);
		//}
	}

	// UI screen 그리기
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mSpriteShader);
	}

	// 버퍼 교환
	SDL_GL_SwapWindow(mWindow);

}

// 빛 반사
void Renderer::SetLightUniforms(Shader* shader, const Matrix4x4& view)
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

Vector3d Renderer::Unproject(const Vector3d& screenPoint) const
{
	// 화면 좌표르 장치 좌표로 변환 (-1에서 +1 범위)
	Vector3d deviceCoord = screenPoint;
	// x = [-screenWidth/2, +screenWidth/2]
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	// y = [-screenHeight/2, +screenHeight/2]
	deviceCoord.y /= (mScreenHeight) * 0.5f;
	// z = [0, 1)
	Matrix4x4 v = mView;
	Matrix4x4 p = mProjection;
	// 벡터를 언프로젝션 행렬로 변환
	Matrix4x4 unprojection = v * p;
	unprojection.Invert();
	return Vector3d::TransformWithPersDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3d& outStart, Vector3d& outDir) const
{
	// 시작점을 얻는다 (가까운 평면의 화면상의 중심)
	Vector3d screenPoint(0.f, 0.f, 0.f);
	outStart = Unproject(screenPoint);
	// 끝점을 얻는다 (가까운 평면과 먼 평면 사이에 있는 화면상의 중심점)
	screenPoint.z = 0.9f;
	Vector3d end = Unproject(screenPoint);

	// 방향 벡터를 얻는다.
	outDir = end - outStart;
	outDir.Normalize();
}

bool Renderer::CreateMirrorTarget()
{
	// 화면 크기의 4분의 1의 크기로 백미러 크기를 설정
	int width = static_cast<int>(mScreenWidth) / 4;
	int height = static_cast<int>(mScreenHeight) / 4;

	// 거울 텍스처를 위한 프레임 버퍼 생성
	glGenFramebuffers(1, &mMirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mMirrorBuffer);

	// 깊이 버퍼 추가
	GLuint depthBuffer;
	// 깊이 버퍼 생성
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// 프레임 버퍼에 깊이 버퍼 붙여준다.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// 렌더링을 위해 사용할 텍스처 생성
	mMirrorTexture = new Texture();
	// 텍스처에 대해 RGB 포맷 요청
	mMirrorTexture->CreateForRendering(width, height, GL_RGB);

	// 거울 텍스처를 프레임 버퍼의 출력 타깃으로 설정
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		mMirrorTexture->GetTextureID(), 0);

	// 이 프레임 버퍼가 그리는 출력 버퍼 리스트 설정
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);	// 거울 텍스처를 매핑해 그린다고 알림

	// 모든 설정이 제대로 됐는지 확인
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// 문제가 발생했다면 프레임 버퍼 삭제
		// 텍스처 삭제하고 false return
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
	// 현재 프레임 버퍼를 설정
	// 백미러 텍스처를 위해 만든 프레임 버퍼를 사용
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// 뷰포트 스케일값으로 뷰포트 크기 설정(전체크기의 1/4)
	glViewport(0, 0,
		static_cast<int>(mScreenWidth * viewPortScale),
		static_cast<int>(mScreenHeight * viewPortScale)
	);

	// 클리어 색상 설정
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 색상 버퍼/깊이 버퍼 클리어
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 메시 컴포넌트 그리기
	// 깊이 버퍼 활성화
	glEnable(GL_DEPTH_TEST);
	// 알파 블렌디 비활성화
	glDisable(GL_BLEND);

	// 기본 메시 셰이더 활성화
	mMeshShader->SetActive();
	// 뷰-투영 행렬 갱신
	mMeshShader->SetMatrixUniform("uViewProj", view * proj);

	// 조명 효과
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

	// 모든 skinned mesh 그리기
	mSkinnedShader->SetActive();
	// 뷰-투영 행렬
	mSkinnedShader->SetMatrixUniform("uViewProj", view * proj);
	// 빛 uniform 갱신
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
	// 깊이 테스트 비활성화 (전역 조명 효과를 위해)
	glDisable(GL_DEPTH_TEST);
	// 전역 G 버퍼 셰이더 활성화
	mGGlobalShader->SetActive();
	// 스프라이트를 위한 버텍스 사각형 활성화
	mSpriteVerts->SetActive();
	// 샘플링하려는 G 버퍼 텍스처 활성화
	mGBuffer->SetTexturesActive();
	// 조명 uniform 설정
	SetLightUniforms(mGGlobalShader, mView);

	// 사각형을 그린다.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// G 버퍼의 깊이 버퍼를 기본 프레임 버퍼로 복사
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// 깊이 테스트 활성화, 하지만 깊이 버퍼로의 쓰기는 비활성화
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	// 점광 셰이더와 메시를 활성화
	mGPointLightShader->SetActive();
	mPointLightMesh->GetVertexArray()->SetActive();

	// 뷰-투영 행렬 설정
	mGPointLightShader->SetMatrixUniform("uViewProj",
		mView * mProjection);
	// 샘플링을 위한 G 버퍼 텍스처 설정
	mGBuffer->SetTexturesActive();

	// 점광 색상은 기존 색상에 더해야 한다.
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// 점광을 그린다.
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

struct DirectionalLight
{
	// 방향광
	Vector3d mDirection;
	// 난반사 색상
	Vector3d mDiffuseColor;
	// 정반사 색상
	Vector3d mSpecColor;
};

class Renderer {
public:
	Renderer(class Game* game);
	~Renderer();

	// 렌더러를 초기화
	bool Initialize(float screenWidth, float screenHeight);
	// 렌더러 종료
	void ShutDown();

	// 텍스처와 메시를 unload
	void UnLoadData();
	// 프레임 그리기
	void Draw();

	// 그릴 스프라이트 추가
	void AddSprite(class SpriteComponent* sprite);
	// 스프라이트 제거
	void RemoveSprite(class SpriteComponent* sprite);
	// 텍스처 가져오기
	class Texture* GetTexture(const std::string& fileName);
	// 메시 가져오기
	class Mesh* GetMesh(const std::string& fileName);

	// 뷰 행렬 세팅
	void SetViewMatrix(const Matrix4x4& view) {
		mView = view;
	}

	// 메시 컴포넌트 추가
	void AddMeshComp(class MeshComponent* mesh);
	// 메시 컴포넌트 추가
	void RemoveMeshComp(class MeshComponent* mesh);

	// 주변광 세팅
	void SetAmbientLight(const Vector3d& ambient) {
		mAmbientLight = ambient;
	}
	const Vector3d& GetAmbientLight() const { return mAmbientLight; }

	// 방향광
	DirectionalLight& GetDirectionalLight() {
		return mDirLight;
	}

	// 화면 좌표르 장치 좌표로 변환 (-1에서 +1 범위)
	Vector3d Unproject(const Vector3d& screenPoint) const;
	// 화면 좌표의 시작 위치
	void GetScreenDirection(Vector3d& outStart, Vector3d& outDir) const;

	float GetScreenWidth() const {
		return mScreenWidth;
	}

	float GetScreenHeight() const {
		return mScreenHeight;
	}

	// 거울
	void SetMirrorView(const Matrix4x4& view) { mMirrorView = view; }
	class Texture* GetMirrorTexture() { return mMirrorTexture; }

	// 점광
	void AddPointLight(class PointLightComponent* light);
	void RemovePointLight(class PointLightComponent* light);

	class GBuffer* GetBuffer() {
		return mGBuffer;
	}
	
private:
	// 셰이더 로드
	bool LoadShaders();
	// 스프라이트의 vertex  배열
	void CreateSpriteVerts();

	// 텍스처 맵
	std::unordered_map<std::string, class Texture*> mTextures;
	// 메시 맵
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// 그려질 스프라이트들
	std::vector<class SpriteComponent*> mSprites;

	// 메시 컴포넌트
	std::vector<class MeshComponent*> mMeshComps;
	// 스켈레탈 메시
	std::vector<class SkeletalMeshComponent*> mSkeletalMeshes;

	// 게임
	class Game* mGame;

	// 셰이더
	class Shader* mSpriteShader;
	// vertex 배열
	class VertexArray* mSpriteVerts;

	// 메시 셰이더
	class Shader* mMeshShader;
	// skinned 셰이더
	class Shader* mSkinnedShader;

	// 3D 셰이더 그리기 용
	// 뷰 행렬
	Matrix4x4 mView;
	// 투영 행렬
	Matrix4x4 mProjection;

	// 화면 크기
	float mScreenWidth;
	float mScreenHeight;

	// window
	SDL_Window* mWindow;
	// Opengl Context
	SDL_GLContext mContext;

	// 주변반사 요소
	Vector3d mAmbientLight;
	// 방향광 요소
	DirectionalLight mDirLight;

	void SetLightUniforms(class Shader* shader, const Matrix4x4& view);

	// 거울(백미러) 프레임 버퍼 개체
	// 생성한 프레임 버퍼 개체의 ID 저장
	unsigned int mMirrorBuffer; 
	// 거울 텍스처
	// 프레임 버퍼와 연관된 텍스처 객체 저장
	class Texture* mMirrorTexture; 
	// 거울 뷰 행렬
	Matrix4x4 mMirrorView;

	// 거울 프레임 버퍼 생성
	bool CreateMirrorTarget();

	// 3D 장면을 여러번 그리는걸 지원하는 Draw 함수
	void Draw3DScene(
		unsigned int framebuffer,	// 프레임 버퍼의 ID
		const Matrix4x4& view,	// 뷰 행렬
		const Matrix4x4& proj,	// 투영 행렬
		float viewPortScale = 1.0f,	// 뷰포트의 크기
		bool lit = true
	);

	// GBuffer
	class GBuffer* mGBuffer;
	// GBuffer Shader
	class Shader* mGGlobalShader;
	// 점광
	class Shader* mGPointLightShader;
	std::vector<class PointLightComponent*> mPointLights;
	class Mesh* mPointLightMesh;

	// Gbuffer로부터 사각형을 그린다.
	void DrawFromGBuffer();
};
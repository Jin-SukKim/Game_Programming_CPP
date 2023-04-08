struct DirectionalLight
{
	// ���Ɽ
	Vector3d mDirection;
	// ���ݻ� ����
	Vector3d mDiffuseColor;
	// ���ݻ� ����
	Vector3d mSpecColor;
};

class Renderer {
public:
	Renderer(class Game* game);
	~Renderer();

	// �������� �ʱ�ȭ
	bool Initialize(float screenWidth, float screenHeight);
	// ������ ����
	void ShutDown();

	// �ؽ�ó�� �޽ø� unload
	void UnLoadData();
	// ������ �׸���
	void Draw();

	// �׸� ��������Ʈ �߰�
	void AddSprite(class SpriteComponent* sprite);
	// ��������Ʈ ����
	void RemoveSprite(class SpriteComponent* sprite);
	// �ؽ�ó ��������
	class Texture* GetTexture(const std::string& fileName);
	// �޽� ��������
	class Mesh* GetMesh(const std::string& fileName);

	// �� ��� ����
	void SetViewMatrix(const Matrix4x4& view) {
		mView = view;
	}

	// �޽� ������Ʈ �߰�
	void AddMeshComp(class MeshComponent* mesh);
	// �޽� ������Ʈ �߰�
	void RemoveMeshComp(class MeshComponent* mesh);

	// �ֺ��� ����
	void SetAmbientLight(const Vector3d& ambient) {
		mAmbientLight = ambient;
	}

	// ���Ɽ
	DirectionalLight& GetDirectionalLight() {
		return mDirLight;
	}

	// ȭ�� ��ǥ�� ��ġ ��ǥ�� ��ȯ (-1���� +1 ����)
	Vector3d Unproject(const Vector3d& screenPoint) const;
	// ȭ�� ��ǥ�� ���� ��ġ
	void GetScreenDirection(Vector3d& outStart, Vector3d& outDir) const;

	float GetScreenWidth() const {
		return mScreenWidth;
	}

	float GetScreenHeight() const {
		return mScreenHeight;
	}
	
private:
	// ���̴� �ε�
	bool LoadShaders();
	// ��������Ʈ�� vertex  �迭
	void CreateSpriteVerts();

	// �ؽ�ó ��
	std::unordered_map<std::string, class Texture*> mTextures;
	// �޽� ��
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// �׷��� ��������Ʈ��
	std::vector<class SpriteComponent*> mSprites;

	// �޽� ������Ʈ
	std::vector<class MeshComponent*> mMeshComps;
	// ���̷�Ż �޽�
	std::vector<class SkeletalMeshComponent*> mSkeletalMeshes;

	// ����
	class Game* mGame;

	// ���̴�
	class Shader* mSpriteShader;
	// vertex �迭
	class VertexArray* mSpriteVerts;

	// �޽� ���̴�
	class Shader* mMeshShader;
	// skinned ���̴�
	class Shader* mSkinnedShader;

	// 3D ���̴� �׸��� ��
	// �� ���
	Matrix4x4 mView;
	// ���� ���
	Matrix4x4 mProjection;

	// ȭ�� ũ��
	float mScreenWidth;
	float mScreenHeight;

	// window
	SDL_Window* mWindow;
	// Opengl Context
	SDL_GLContext mContext;

	// �ֺ��ݻ� ���
	Vector3d mAmbientLight;
	// ���Ɽ ���
	DirectionalLight mDirLight;

	void SetLightUniforms(class Shader* shader);
};
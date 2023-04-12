class Shader {
public:
	Shader();
	~Shader();

	// 주어진 이름으로 vertex/fragment shader 로드
	bool Load(const std::string& vertName,
		const std::string& fragName);
	void Unload();

	// 셰이더를 활성화된 셰이더 프로그램으로 설정
	void SetActive();
	
	// uniform 타입 지원
	void SetMatrixUniform(const char* name, const Matrix4x4& matrix);
	// uniform 타입의 배열
	void SetMatrixUniforms(const char* name, Matrix4x4* matrices, unsigned count);

	// fragment shader에 빛 반사를 위한 데이터 세팅(vetor3d)
	void SetVectorUniform(const char* name, const Vector3d& vector);
	void SetVector2Uniform(const char* name, const Vector2d& vector);
	// fragment shader에 빛 반사 데이터 세팅(float
	void SetFloatUniform(const char* name, float value);
	
	// int uniform 설정
	void SetIntUniform(const char* name, int value);


// 정점/인덱스 버퍼처럼 오브젝트 ID를 가진다.
private:
	// 지정된 셰이더를 컴파일
	bool CompileShader(const std::string& fileName,
		GLenum shaderType, GLuint& outShader);
	// 셰이더가 성곡적으로 컴파일됐는지 확인
	bool IsCompiled(GLuint shader);
	// Vertex/Fragment 프로그램이 연결됐는지 확인
	bool IsValidProgram();
	// 셰이더 오브젝트 ID를 저장
	// GLuint는 uint의 OpenGL 버전이다.
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
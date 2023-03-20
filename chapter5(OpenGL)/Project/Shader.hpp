class Shader {
public:
	Shader();
	~Shader();

	// �־��� �̸����� vertex/fragment shader �ε�
	bool Load(const std::string& vertName,
		const std::string& fragName);
	void Unload();

	// ���̴��� Ȱ��ȭ�� ���̴� ���α׷����� ����
	void SetActive();
	
	// uniform Ÿ�� ����
	void SetMatrixUniform(const char* name, const Matrix4x4& matrix);
	
// ����/�ε��� ����ó�� ������Ʈ ID�� ������.
private:
	// ������ ���̴��� ������
	bool CompileShader(const std::string& fileName,
		GLenum shaderType, GLuint& outShader);
	// ���̴��� ���������� �����ϵƴ��� Ȯ��
	bool IsCompiled(GLuint shader);
	// Vertex/Fragment ���α׷��� ����ƴ��� Ȯ��
	bool IsValidProgram();
	// ���̴� ������Ʈ ID�� ����
	// GLuint�� uint�� OpenGL �����̴�.
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
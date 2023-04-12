#include "..\..\PreCompiled.hpp"

Shader::Shader() : mShaderProgram(0), mVertexShader(0), mFragShader(0) {}

Shader::~Shader() {}

// �ﰢ���� �׸� �� Ȱ��ȭ�� ���̴��� ����Ѵ�.
void Shader::SetActive() {
	glUseProgram(mShaderProgram);
}


bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// vertex/fragment shader ������
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	// vertex/fragment shader�� ���� �����ϴ� ���̴� ���α׷� ����
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// ���α׷��� ���������� ����ƴ��� ����
	if (!IsValidProgram()) {
		return false;
	}

	return true;
}

// vertex/fragment ���̴��� �����Ѵ�.
void Shader::Unload() {
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}



bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType, GLuint& outShader) 
{
	// ������ ����
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// ��� �ؽ�Ʈ�� �о string���� �����
		std::stringstream sstream;
		// �б� ���ۿ��� ������ �����´�
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		// string�� C��Ÿ�� ���ڿ��� �����.
		const char* contentsChar = contents.c_str();

		// ������ Ÿ���� ���̴��� �����Ѵ�.
		outShader = glCreateShader(shaderType);
		// �ҽ� ���ڿ��� �����ϰ� ������ �õ�
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);

		if (!IsCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}

	return true;
}

bool Shader::IsCompiled(GLuint shader) {
	GLint status;
	// ������ ���¸� �����Ѵ�
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram() {
	GLint status;
	// ���̴� ���α׷� ���¸� ����
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}


void Shader::SetMatrixUniform(const char* name, const Matrix4x4& matrix) {
	// �ش� �̸��� uniform�� ã�´�
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// ��� �����͸� uniform�� �����Ѵ�.
	glUniformMatrix4fv(
		loc,					// Uniform ID
		1,						// ����� ��(�̹� ���� �ϳ�)
		GL_TRUE,				// �� ���� ���� TRUE
		matrix.GetAsFloatPtr()	// ��� �����Ϳ� ���� ������
	);
}

void Shader::SetMatrixUniforms(const char* name, Matrix4x4* matrices, unsigned count)
{
	// �ش� �̸��� uniform�� ã�´�
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// ��� �����͸� uniform�� �����Ѵ�.
	glUniformMatrix4fv(
		loc,					// Uniform ID
		count,						// ����� ��(�̹� ���� �ϳ�)
		GL_TRUE,				// �� ���� ���� TRUE
		matrices->GetAsFloatPtr()	// ��� �����Ϳ� ���� ������
	);
}

// �� �ݻ�
void Shader::SetVectorUniform(const char* name, const Vector3d& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// 3D ���� ������ ����
	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2d& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// Send the vector data
	glUniform2fv(loc, 1, vector.GetAsFloatPtr());
}


void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// float ������ ����
	glUniform1f(loc, value);
}


void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// int ������ ����
	glUniform1i(loc, value);
}
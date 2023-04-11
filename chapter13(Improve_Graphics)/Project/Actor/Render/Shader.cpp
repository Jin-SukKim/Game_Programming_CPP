#include "..\..\PreCompiled.hpp"

Shader::Shader() : mShaderProgram(0), mVertexShader(0), mFragShader(0) {}

Shader::~Shader() {}

// 삼각형을 그릴 때 활성화된 셰이더를 사용한다.
void Shader::SetActive() {
	glUseProgram(mShaderProgram);
}


bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	// vertex/fragment shader 컴파일
	if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	// vertex/fragment shader를 서로 연결하는 셰이더 프로그램 생성
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	// 프로그램이 성공적으로 연결됐는지 검증
	if (!IsValidProgram()) {
		return false;
	}

	return true;
}

// vertex/fragment 셰이더를 삭제한다.
void Shader::Unload() {
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragShader);
}



bool Shader::CompileShader(const std::string& fileName,
	GLenum shaderType, GLuint& outShader) 
{
	// 파일을 연다
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		// 모든 텍스트를 읽어서 string으로 만든다
		std::stringstream sstream;
		// 읽기 버퍼에서 내용을 가져온다
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		// string을 C스타일 문자열로 만든다.
		const char* contentsChar = contents.c_str();

		// 지정된 타입의 셰이더를 생성한다.
		outShader = glCreateShader(shaderType);
		// 소스 문자열을 설정하고 컴파일 시도
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
	// 컴파일 상태를 질의한다
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
	// 셰이더 프로그램 상태를 질의
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
	// 해당 이름의 uniform을 찾는다
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// 행렬 데이터를 uniform에 전송한다.
	glUniformMatrix4fv(
		loc,					// Uniform ID
		1,						// 행렬의 수(이번 경우는 하나)
		GL_TRUE,				// 행 벡터 사용시 TRUE
		matrix.GetAsFloatPtr()	// 행렬 데이터에 대한 포인터
	);
}

void Shader::SetMatrixUniforms(const char* name, Matrix4x4* matrices, unsigned count)
{
	// 해당 이름의 uniform을 찾는다
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// 행렬 데이터를 uniform에 전송한다.
	glUniformMatrix4fv(
		loc,					// Uniform ID
		count,						// 행렬의 수(이번 경우는 하나)
		GL_TRUE,				// 행 벡터 사용시 TRUE
		matrices->GetAsFloatPtr()	// 행렬 데이터에 대한 포인터
	);
}

// 빛 반사
void Shader::SetVectorUniform(const char* name, const Vector3d& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// 3D 벡터 데이터 전송
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
	// float 데이터 전속
	glUniform1f(loc, value);
}


void Shader::SetIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);
	// int 데이터 전속
	glUniform1i(loc, value);
}
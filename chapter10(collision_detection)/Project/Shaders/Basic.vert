// GLSL 언어 3.3 버전
#version 330

// Vertex의 속성
in vec3 inPosition;

// GLSL의 메인 함수는 파라미터를 받지 않는다.
// 대신 inPosition처럼 전역 변수로 받아야한다.
void main()
{
	// 셰이더 코드 작성
	gl_Position = vec4(inPosition, 1.0);
}

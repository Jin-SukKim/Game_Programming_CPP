// GLSL 언어 3.3 버전
#version 330

// 세계 변환 행렬과 뷰-투영 행렬을 위한 Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 3D inPosition 선언, vertex 속성
in vec3 inPosition;

// GLSL의 메인 함수는 파라미터를 받지 않는다.
// 대신 inPosition처럼 전역 변수로 받아야한다.
void main()
{
	// 셰이더 코드 작성

	// 동차 좌표로 변경
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}

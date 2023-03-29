// GLSL 언어 3.3 버전
#version 330

// 세계 변환 행렬과 뷰-투영 행렬을 위한 Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 복수개의 속성이 있으므로 layout을 활용
// 3D inPosition 선언, vertex 속성
layout(location=0) in vec3 inPosition;
// 법선 벡터
layout(location=1) in vec3 inNormal;
// 텍스처 좌표
layout(location=2) in vec2 inTexCoord;

// 텍스처 좌표를 fragment shader로 전달
out vec2 fragTexCoord;

// GLSL의 메인 함수는 파라미터를 받지 않는다.
// 대신 inPosition처럼 전역 변수로 받아야한다.
void main()
{
	// 셰이더 코드 작성

	// 동차 좌표로 변경
	vec4 pos = vec4(inPosition, 1.0);
	// model space -> world space -> clip space 좌표로 변환
	gl_Position = pos * uWorldTransform * uViewProj;

	// fragment shader에 넘길 변수 값 복사
	fragTexCoord = inTexCoord;
}

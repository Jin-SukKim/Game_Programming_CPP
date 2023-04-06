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
// 세계 공간에서의 법선
out vec3 fragNormal;
// 세계 공간에서의 위치
out vec3 fragWorldPos;

// GLSL의 메인 함수는 파라미터를 받지 않는다.
// 대신 inPosition처럼 전역 변수로 받아야한다.
void main()
{
	// 위치를 동차 좌표로 변환
	vec4 pos = vec4(inPosition, 1.0);
	// 위치를 세계 공간상의 위치로 변환
	pos = pos * uWorldTransform;
	// 세계 공간상의 위치 저장(빛 반사 계산을 위함)
	fragWorldPos = pos.xyz;
	// 위치를 클립 공간 좌표로 변환
	gl_Position = pos * uViewProj;

	// 법선을 세계 공간상의 법선으로 변환 (동차 좌표로 변환해 계산을 쉽게함, w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// 텍스처 좌푤르 프래그먼트 셰이더로 전달
	fragTexCoord = inTexCoord;
}

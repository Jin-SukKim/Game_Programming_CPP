// GLSL 3.3 버전 요청
#version 330

// vertex shader에서 받은 텍스처 좌표
in vec2 fragTexCoord;
// 전달받은 세계 공간에서의 법선
in vec3 fragNormal;
// 전달받은 세계 공간에서의 위치
in vec3 fragWorldPos;

// 레이아웃 번호는 G 버퍼의 출력에 해당
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

// 분산 색상 텍스처 샘플러
uniform sampler2D uTexture;

void main() {
	// 분산 색상은 텍스처로부터 얻는다.
	outDiffuse = texture(uTexture, fragTexCoord).xyz;
	// 법선과 세계 좌표는 바로 G 버퍼에 전달
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}
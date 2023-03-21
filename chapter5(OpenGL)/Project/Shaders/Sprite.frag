// GLSL 3.3 버전 요청
#version 330

// vertex shader에서 받은 텍스처 좌표
in vec2 fragTexCoord;

// 출력 색상 저장을 위한 변수
// 색상 버퍼로 출력되는 색상
out vec4 outColor;

// 텍스처 좌표로 색상을 얻기위한 텍스처 샘플러
uniform sampler2D uTexture;


void main() {
	// 푸른색으로 설정
	outColor = texture(uTexture, fragTexCoord);

}
// GLSL 3.3 버전 요청
#version 330

// vertex shader에서 받은 텍스처 좌표
in vec2 fragTexCoord;
// 전달받은 세계 공간에서의 법선
in vec3 fragNormal;
// 전달받은 세계 공간에서의 위치
in vec3 fragWorldPos;

// 출력 색상 저장을 위한 변수
// 색상 버퍼로 출력되는 색상
out vec4 outColor;

// 텍스처 좌표로 색상을 얻기위한 텍스처 샘플러
uniform sampler2D uTexture;

// 방향광을 위한 구조체 정의
struct DirectionalLight
{
	// 방향광
	vec3 mDirection;
	// 난반사 색상
	vec3 mDiffuseColor;
	// 정반사 색상
	vec3 mSpecColor;
};

// 조명을 위한 uniform
// 세계 공간에서의 카메라 위치
uniform vec3 uCameraPos;
// 주변광
uniform vec3 uAmbientLight;
// 표면에 대한 정반사 지수
uniform float uSpecPower;
// 방향광(지금은 오직 하나)
uniform DirectionalLight uDirLight;

void main() {
	// 표면 법선
	vec3 N = normalize(fragNormal);
	// 표면에서 광원으로의 벡터
	vec3 L = normalize(-uDirLight.mDirection);
	// 표면에서 카메라로 향하는 벡터
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// N에 대한 -L의 반사
	vec3 R = normalize(reflect(-L, N));

	// 퐁 반사 계산
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong = uAmbientLight + Diffuse + Specular;
	}

	// 최종 색은 텍스처 색상 곱하기 퐁 광원 (알파값 = 1)
	outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}
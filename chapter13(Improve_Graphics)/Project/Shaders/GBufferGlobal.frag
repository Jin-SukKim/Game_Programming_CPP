// GLSL 3.3 버전 요청
#version 330

// vertex shader에서 받은 텍스처 좌표
in vec2 fragTexCoord;

// 출력 색상 저장을 위한 변수
// 색상 버퍼로 출력되는 색상
layout(location = 0) out vec4 outColor;

// G 버퍼의 다양한 입력 
// 분산광
uniform sampler2D uGDiffuse;
// 법선
uniform sampler2D uGNormal;		
// 세계 위치
uniform sampler2D uGWorldPos;	

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
// 방향광(지금은 오직 하나)
uniform DirectionalLight uDirLight;

void main() {
	// 분산 색상과 법선, 세계 위치를 G 버퍼로부터 샘플링
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	// 표면 법선
	vec3 N = normalize(gbufferNorm);
	// 표면에서 광원으로의 벡터
	vec3 L = normalize(-uDirLight.mDirection);
	// 표면에서 카메라로 향하는 벡터
	vec3 V = normalize(uCameraPos - gbufferWorldPos);
	// N에 대한 -L의 반사
	vec3 R = normalize(reflect(-L, N));

	// 퐁 반사 계산
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		Phong += Diffuse;
	}
	// Clamp light between 0-1 RGB value
	Phong = clamp(Phong, 0.0, 1.0);

	// 최종 색은 분산 색상 곱하기 퐁 광원 (알파값 = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
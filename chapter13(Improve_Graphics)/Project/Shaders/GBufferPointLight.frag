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

// 점광을 위한 구조체
struct PointLight
{
	// 광원의 위치
	vec3 mWorldPos;
	// 분산 색상
	vec3 mDiffuseColor;
	// 광원의 반지름
	float mInnerRadius;
	float mOuterRadius;
};

uniform PointLight uPointLight;
// 화면의 너비/높이 저장
uniform vec2 uScreenDimensions;

void main() {
	// G 버퍼에서 올바른 데이터를 샘플링할 수 있도록 좌표를 계산
	// gl_FragCoord는 화면 공간 위치 정보가 있는 내장 변수다.
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;

	// 분산 색상과 법선, 세계 위치를 G 버퍼로부터 샘플링
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;

	// 표면 법선
	vec3 N = normalize(gbufferNorm);
	// 표면에서 광원으로의 벡터
	vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

	// 광원에 대한 퐁 반사 요소 계산
	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		// 광원과 세계 위치와의 거리 계산
		float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);
		// 값을 [0, 1] 범위로 계산하기 위해 smoothstep 사용
		// 파라미터는 내부/외부 반지름
		float intensity = smoothstep(uPointLight.mInnerRadius,
			uPointLight.mOuterRadius, dist);
		// 광원의 분산 색상은 빛의 세기에 의존
		vec3 DiffuseColor = mix(uPointLight.mDiffuseColor,
			vec3(0.0, 0.0, 0.0), intensity);
		Phong = DiffuseColor * NdotL;
	}

	// 최종 색은 분산 색상 곱하기 퐁 광원 (알파값 = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
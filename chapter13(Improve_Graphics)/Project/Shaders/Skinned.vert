// GLSL 언어 3.3 버전
#version 330

// 세계 변환 행렬과 뷰-투영 행렬을 위한 Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
// 행렬 Palette, 인덱스에 해당하는 본의 인버스 바인드 포즈 * 현재 포즈 결과가 있다.
uniform mat4 uMatrixPalette[96];

// 복수개의 속성이 있으므로 layout을 활용(vertex 속성)
// 3D inPosition 선언(3개의 float)
layout(location = 0) in vec3 inPosition;
// 법선 벡터(3개의 float)
layout(location = 1) in vec3 inNormal;
// vertex에 영향을 미치는 본(4개의 unsigned int)
layout(location = 2) in uvec4 inSkinBones;
// 본 영향의 가중치(4개의 float)
layout(location = 3) in vec4 inSkinWeights;
// 텍스처 좌표(2개의 float)
layout(location = 4) in vec2 inTexCoord;

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

	// 위치를 스키닝 (각 vertex는 4개의 다른 본의 영향을 받는다.)
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// 위치를 세계 공간상의 위치로 변환
	skinnedPos = skinnedPos * uWorldTransform;
	// 세계 공간상의 위치 저장
	fragWorldPos = skinnedPos.xyz;
	// 위치를 클립 공간 좌표로 변환
	gl_Position = skinnedPos * uViewProj;

	// 법선 벡터 스키닝 (조명을 올바르게 적용하기 위함)
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		+ (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		+ (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		+ (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// 법선을 세계 공간상의 법선으로 변환 (동차 좌표로 변환해 계산을 쉽게함, w = 0)
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// 텍스처 좌푤르 프래그먼트 셰이더로 전달
	fragTexCoord = inTexCoord;
}

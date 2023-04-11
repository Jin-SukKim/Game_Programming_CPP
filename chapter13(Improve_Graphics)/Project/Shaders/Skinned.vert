// GLSL ��� 3.3 ����
#version 330

// ���� ��ȯ ��İ� ��-���� ����� ���� Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
// ��� Palette, �ε����� �ش��ϴ� ���� �ι��� ���ε� ���� * ���� ���� ����� �ִ�.
uniform mat4 uMatrixPalette[96];

// �������� �Ӽ��� �����Ƿ� layout�� Ȱ��(vertex �Ӽ�)
// 3D inPosition ����(3���� float)
layout(location = 0) in vec3 inPosition;
// ���� ����(3���� float)
layout(location = 1) in vec3 inNormal;
// vertex�� ������ ��ġ�� ��(4���� unsigned int)
layout(location = 2) in uvec4 inSkinBones;
// �� ������ ����ġ(4���� float)
layout(location = 3) in vec4 inSkinWeights;
// �ؽ�ó ��ǥ(2���� float)
layout(location = 4) in vec2 inTexCoord;

// �ؽ�ó ��ǥ�� fragment shader�� ����
out vec2 fragTexCoord;
// ���� ���������� ����
out vec3 fragNormal;
// ���� ���������� ��ġ
out vec3 fragWorldPos;

// GLSL�� ���� �Լ��� �Ķ���͸� ���� �ʴ´�.
// ��� inPositionó�� ���� ������ �޾ƾ��Ѵ�.
void main()
{
	// ��ġ�� ���� ��ǥ�� ��ȯ
	vec4 pos = vec4(inPosition, 1.0);

	// ��ġ�� ��Ű�� (�� vertex�� 4���� �ٸ� ���� ������ �޴´�.)
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// ��ġ�� ���� �������� ��ġ�� ��ȯ
	skinnedPos = skinnedPos * uWorldTransform;
	// ���� �������� ��ġ ����
	fragWorldPos = skinnedPos.xyz;
	// ��ġ�� Ŭ�� ���� ��ǥ�� ��ȯ
	gl_Position = skinnedPos * uViewProj;

	// ���� ���� ��Ű�� (������ �ùٸ��� �����ϱ� ����)
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x
		+ (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y
		+ (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z
		+ (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// ������ ���� �������� �������� ��ȯ (���� ��ǥ�� ��ȯ�� ����� ������, w = 0)
	fragNormal = (skinnedNormal * uWorldTransform).xyz;

	// �ؽ�ó ��ǧ�� �����׸�Ʈ ���̴��� ����
	fragTexCoord = inTexCoord;
}

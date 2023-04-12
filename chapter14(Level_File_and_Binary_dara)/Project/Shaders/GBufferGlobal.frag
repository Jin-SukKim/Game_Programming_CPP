// GLSL 3.3 ���� ��û
#version 330

// vertex shader���� ���� �ؽ�ó ��ǥ
in vec2 fragTexCoord;

// ��� ���� ������ ���� ����
// ���� ���۷� ��µǴ� ����
layout(location = 0) out vec4 outColor;

// G ������ �پ��� �Է� 
// �л걤
uniform sampler2D uGDiffuse;
// ����
uniform sampler2D uGNormal;		
// ���� ��ġ
uniform sampler2D uGWorldPos;	

// ���Ɽ�� ���� ����ü ����
struct DirectionalLight
{
	// ���Ɽ
	vec3 mDirection;
	// ���ݻ� ����
	vec3 mDiffuseColor;
	// ���ݻ� ����
	vec3 mSpecColor;
};

// ������ ���� uniform
// ���� ���������� ī�޶� ��ġ
uniform vec3 uCameraPos;
// �ֺ���
uniform vec3 uAmbientLight;
// ���Ɽ(������ ���� �ϳ�)
uniform DirectionalLight uDirLight;

void main() {
	// �л� ����� ����, ���� ��ġ�� G ���۷κ��� ���ø�
	vec3 gbufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, fragTexCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	// ǥ�� ����
	vec3 N = normalize(gbufferNorm);
	// ǥ�鿡�� ���������� ����
	vec3 L = normalize(-uDirLight.mDirection);
	// ǥ�鿡�� ī�޶�� ���ϴ� ����
	vec3 V = normalize(uCameraPos - gbufferWorldPos);
	// N�� ���� -L�� �ݻ�
	vec3 R = normalize(reflect(-L, N));

	// �� �ݻ� ���
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		Phong += Diffuse;
	}
	// Clamp light between 0-1 RGB value
	Phong = clamp(Phong, 0.0, 1.0);

	// ���� ���� �л� ���� ���ϱ� �� ���� (���İ� = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
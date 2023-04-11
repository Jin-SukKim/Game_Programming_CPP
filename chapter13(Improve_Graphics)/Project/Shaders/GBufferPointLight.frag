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

// ������ ���� ����ü
struct PointLight
{
	// ������ ��ġ
	vec3 mWorldPos;
	// �л� ����
	vec3 mDiffuseColor;
	// ������ ������
	float mInnerRadius;
	float mOuterRadius;
};

uniform PointLight uPointLight;
// ȭ���� �ʺ�/���� ����
uniform vec2 uScreenDimensions;

void main() {
	// G ���ۿ��� �ùٸ� �����͸� ���ø��� �� �ֵ��� ��ǥ�� ���
	// gl_FragCoord�� ȭ�� ���� ��ġ ������ �ִ� ���� ������.
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;

	// �л� ����� ����, ���� ��ġ�� G ���۷κ��� ���ø�
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;

	// ǥ�� ����
	vec3 N = normalize(gbufferNorm);
	// ǥ�鿡�� ���������� ����
	vec3 L = normalize(uPointLight.mWorldPos - gbufferWorldPos);

	// ������ ���� �� �ݻ� ��� ���
	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		// ������ ���� ��ġ���� �Ÿ� ���
		float dist = distance(uPointLight.mWorldPos, gbufferWorldPos);
		// ���� [0, 1] ������ ����ϱ� ���� smoothstep ���
		// �Ķ���ʹ� ����/�ܺ� ������
		float intensity = smoothstep(uPointLight.mInnerRadius,
			uPointLight.mOuterRadius, dist);
		// ������ �л� ������ ���� ���⿡ ����
		vec3 DiffuseColor = mix(uPointLight.mDiffuseColor,
			vec3(0.0, 0.0, 0.0), intensity);
		Phong = DiffuseColor * NdotL;
	}

	// ���� ���� �л� ���� ���ϱ� �� ���� (���İ� = 1)
	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}
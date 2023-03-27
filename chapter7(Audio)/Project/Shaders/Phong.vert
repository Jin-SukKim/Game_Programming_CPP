// GLSL ��� 3.3 ����
#version 330

// ���� ��ȯ ��İ� ��-���� ����� ���� Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// �������� �Ӽ��� �����Ƿ� layout�� Ȱ��
// 3D inPosition ����, vertex �Ӽ�
layout(location=0) in vec3 inPosition;
// ���� ����
layout(location=1) in vec3 inNormal;
// �ؽ�ó ��ǥ
layout(location=2) in vec2 inTexCoord;

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
	// ��ġ�� ���� �������� ��ġ�� ��ȯ
	pos = pos * uWorldTransform;
	// ���� �������� ��ġ ����(�� �ݻ� ����� ����)
	fragWorldPos = pos.xyz;
	// ��ġ�� Ŭ�� ���� ��ǥ�� ��ȯ
	gl_Position = pos * uViewProj;

	// ������ ���� �������� �������� ��ȯ (���� ��ǥ�� ��ȯ�� ����� ������, w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// �ؽ�ó ��ǧ�� �����׸�Ʈ ���̴��� ����
	fragTexCoord = inTexCoord;
}

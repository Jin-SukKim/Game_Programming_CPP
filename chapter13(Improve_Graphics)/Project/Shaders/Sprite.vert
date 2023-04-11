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

// GLSL�� ���� �Լ��� �Ķ���͸� ���� �ʴ´�.
// ��� inPositionó�� ���� ������ �޾ƾ��Ѵ�.
void main()
{
	// ���̴� �ڵ� �ۼ�

	// ���� ��ǥ�� ����
	vec4 pos = vec4(inPosition, 1.0);
	// model space -> world space -> clip space ��ǥ�� ��ȯ
	gl_Position = pos * uWorldTransform * uViewProj;

	// fragment shader�� �ѱ� ���� �� ����
	fragTexCoord = inTexCoord;
}

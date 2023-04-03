// GLSL ��� 3.3 ����
#version 330

// ���� ��ȯ ��İ� ��-���� ����� ���� Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 3D inPosition ����, vertex �Ӽ�
in vec3 inPosition;

// GLSL�� ���� �Լ��� �Ķ���͸� ���� �ʴ´�.
// ��� inPositionó�� ���� ������ �޾ƾ��Ѵ�.
void main()
{
	// ���̴� �ڵ� �ۼ�

	// ���� ��ǥ�� ����
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProj;
}

// GLSL ��� 3.3 ����
#version 330

// Vertex�� �Ӽ�
in vec3 inPosition;

// GLSL�� ���� �Լ��� �Ķ���͸� ���� �ʴ´�.
// ��� inPositionó�� ���� ������ �޾ƾ��Ѵ�.
void main()
{
	// ���̴� �ڵ� �ۼ�
	gl_Position = vec4(inPosition, 1.0);
}

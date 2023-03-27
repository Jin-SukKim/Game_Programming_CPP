// GLSL 3.3 ���� ��û
#version 330

// vertex shader���� ���� �ؽ�ó ��ǥ
in vec2 fragTexCoord;

// ��� ���� ������ ���� ����
// ���� ���۷� ��µǴ� ����
out vec4 outColor;

// �ؽ�ó ��ǥ�� ������ ������� �ؽ�ó ���÷�
uniform sampler2D uTexture;


void main() {
	// Ǫ�������� ����
	outColor = texture(uTexture, fragTexCoord);

}
// GLSL 3.3 ���� ��û
#version 330

// vertex shader���� ���� �ؽ�ó ��ǥ
in vec2 fragTexCoord;
// ���޹��� ���� ���������� ����
in vec3 fragNormal;
// ���޹��� ���� ���������� ��ġ
in vec3 fragWorldPos;

// ���̾ƿ� ��ȣ�� G ������ ��¿� �ش�
layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;

// �л� ���� �ؽ�ó ���÷�
uniform sampler2D uTexture;

void main() {
	// �л� ������ �ؽ�ó�κ��� ��´�.
	outDiffuse = texture(uTexture, fragTexCoord).xyz;
	// ������ ���� ��ǥ�� �ٷ� G ���ۿ� ����
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
}
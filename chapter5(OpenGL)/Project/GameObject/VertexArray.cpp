#include "PreCompiled.hpp"

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts), mNumIndices(numIndices)
{
	// VertexArray �������� ������ �����ϴ�

	// ���� �迭 ��ü�� ������ �� ��� ������ ID�� �����Ѵ�.
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// ���� �迭 ��ü�� ������ �� ���� ���۸� �����Ѵ�.
	glGenBuffers(1, &mVertexBuffer);
	// ù��° ���ڴ� mVertexBuffer�� ���۷� ����ϰڴٴ� �ǹ�
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	// �����迭 �����ڷ� ���޵� ���� �����͸� ���� ���۷� �����Ѵ�.
	// ��ü ID�� �������� �ʰ� ��� ���� ���ε��� ������ Ÿ���� �����Ѵ�.
	glBufferData(
		// �����͸� �� ������ ���� Ÿ��(�� ������ ���۸� ����Ѵٴ� �ǹ�)
		GL_ARRAY_BUFFER,
		// ������ ����Ʈ ũ��(x,y,z 3���� float�� ����) + UV ��ǥ
		numVerts * 5 * sizeof(float),
		// ������ �ҽ�(������)
		verts,				
		// �����͸� ��� �������(�����͸� ���� �� ���� �ε�, ���� �׷����� ��� ���Ǵ� �ɼ�)
		GL_STATIC_DRAW
	);

	// �ε��� ���� ����
	// ���� Ÿ���� �ε��� ���۷� �����ϴ� ���� �����ϰ� ���� ���� ������ ����ϴ�.
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	// �ε��� �����͸� �ε��� ���۷� �����Ѵ�.
	glBufferData(
		// �ε��� ����
		GL_ELEMENT_ARRAY_BUFFER,
		// �������� ũ��
		numIndices * sizeof(unsigned int),
		indices,
		GL_STATIC_DRAW
	);

	// ���� �Ӽ����� �Ҹ��� ���� ���̾ƿ�(vertex layout)�� ����
	// ���� ���̾ƿ��� 3���� float���� ���� ��ġ�̴�.
	// ù��° ���� �Ӽ�(�Ӽ� 0)�� Ȱ��ȭ
	glEnableVertexAttribArray(0);
	// ũ��� Ÿ��, �Ӽ��� ������ ����
	glVertexAttribPointer(
		// �Ӽ� �ε���(ù ��° ���� �Ӽ� �ε����� 0)
		0,
		// ����� ��(�̹� ���ô� 3, x,y,z) ���� ��ġ ��ǥ�� ����
		3,
		// ����� Ÿ��
		GL_FLOAT,
		// ������ Ÿ�Կ����� ���ȴ�.
		GL_FALSE,
		// ���� (�Ϲ������� �� ������ ũ��), ������ ���� ������ byte ������
		// ���� ���ۿ��� �е�(padding)�� ���ٸ� ������ ũ�Ⱑ �����̵ȴ�.
		sizeof(float) * 5,
		// ������ ���ۿ��� �� �Ӽ������� ������
		// ��ġ �Ӽ��� ������ ���� ��ġ�� �����ϱ⿡ 0�̴�.
		// (�߰� �Ӽ��� ���ؼ��� 0�� �ƴ� ���� �����ؾ� �Ѵ�)
		0
	); // �̰� ��ġ ���� �Ӽ��� �����Ѵ�.

	// �ι�° ���� �Ӽ��� �ؽ�ó ��ǥ (�Ӽ� 1 Ȱ��ȭ)
	glEnableVertexAttribArray(1);
	// �ؽ�ó ��ǥ ���� ����
	glVertexAttribPointer(
		1,	// ���ؽ� �Ӽ� �ε���
		2,	// ������Ʈ�� �� (UV, 2)
		GL_FLOAT,	// ������Ʈ Ÿ��
		GL_FALSE,	// float Ÿ���̹Ƿ� false
		sizeof(float) * 5,	// ���� (�׻� ���ؽ��� ũ��, ��ġ��ǥ + �ؽ�ó��ǥ)
		// (void*) Ÿ��
		reinterpret_cast<void*>(sizeof(float) * 3)	// ������ ������(�տ� ��ġ��ǥ�� �����Ƿ�)
	);
	

}

// ���� ����, �ε��� ����, ���� �迭 ��ü�� �����Ѵ�.
VertexArray::~VertexArray() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

// ���� ����� ���� �迭�� �����ϴ� �Լ��� ȣ��
void VertexArray::SetActive() {
	glBindVertexArray(mVertexArray);
}
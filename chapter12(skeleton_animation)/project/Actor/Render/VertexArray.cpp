#include "..\..\PreCompiled.hpp"

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts), mNumIndices(numIndices)
{
	// VertexArray �������� ������ �����ϴ�

	// ���� �迭 ��ü�� ������ �� ��� ������ ID�� �����Ѵ�.
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 8 * sizeof(float);
	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}

	// ���� �迭 ��ü�� ������ �� ���� ���۸� �����Ѵ�.
	glGenBuffers(1, &mVertexBuffer);
	// ù��° ���ڴ� mVertexBuffer�� ���۷� ����ϰڴٴ� �ǹ�
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	// �����迭 �����ڷ� ���޵� ���� �����͸� ���� ���۷� �����Ѵ�.
	// ��ü ID�� �������� �ʰ� ��� ���� ���ε��� ������ Ÿ���� �����Ѵ�.
	glBufferData(
		// �����͸� �� ������ ���� Ÿ��(�� ������ ���۸� ����Ѵٴ� �ǹ�)
		GL_ARRAY_BUFFER,
		// ������ ����Ʈ ũ��(x,y,z 3���� float�� ����) + UV ��ǥ + ���� ����
		numVerts * vertexSize,
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

	// vertex �Ӽ�
	if (layout == PosNormTex)
	{
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
			// ������ Ÿ�Կ����� ���ȴ�. (���� ����ȭ�ؾ��ϴ� �� ����)
			GL_FALSE,
			// ���� (�Ϲ������� �� ������ ũ��), ������ ���� ������ byte ������
			// ���� ���ۿ��� �е�(padding)�� ���ٸ� ������ ũ�Ⱑ �����̵ȴ�.
			vertexSize,
			// ������ ���ۿ��� �� �Ӽ������� ������
			// ��ġ �Ӽ��� ������ ���� ��ġ�� �����ϱ⿡ 0�̴�.
			// (�߰� �Ӽ��� ���ؼ��� 0�� �ƴ� ���� �����ؾ� �Ѵ�)
			0
		); // �̰� ��ġ ���� �Ӽ��� �����Ѵ�.

		// �ι�° ���� �Ӽ��� ���� ���� (�Ӽ� 1 Ȱ��ȭ)
		glEnableVertexAttribArray(1);
		// ���� ���� ���� ����
		glVertexAttribPointer(
			1,	// ���ؽ� �Ӽ� �ε���
			3,	// ������Ʈ�� �� (���� ����, 3)
			GL_FLOAT,	// ������Ʈ Ÿ��
			GL_FALSE,	// float Ÿ���̹Ƿ� false
			vertexSize,	// ���� (�׻� ���ؽ��� ũ��, ��ġ��ǥ + �������� + �ؽ�ó��ǥ )
			// (void*) Ÿ��
			reinterpret_cast<void*>(sizeof(float) * 3)	// ������ ������(�տ� ��ġ��ǥ�� �����Ƿ�)
		);

		// ����° ���� �Ӽ��� �ؽ�ó ��ǥ (�Ӽ� 2 Ȱ��ȭ)
		glEnableVertexAttribArray(2);
		// �ؽ�ó ��ǥ ���� ����
		glVertexAttribPointer(
			2,	// �Ӽ� �ε���
			2,	// uv ��ǥ
			GL_FLOAT,
			GL_FALSE,
			vertexSize, // vertex�� ����
			reinterpret_cast<void*>(sizeof(float) * 6) // ������(��ġ+����)
		);
	}
	// ���� �����ϴ� ���̴�
	else if (layout == PosNormSkinTex)
	{
		// ��ġ�� 3���� float
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// ������ 3���� float
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// ��Ű�� ��(unsigned int)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// ��Ű�� ����ġ (float�� ��ȯ)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

		// �ؽ�ó ��ǥ
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
	

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
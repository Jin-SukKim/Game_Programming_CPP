#include "..\..\PreCompiled.hpp"

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts), mNumIndices(numIndices)
{
	// VertexArray 생성자의 구현은 복잡하다

	// 정점 배열 개체를 생성한 뒤 멤버 변수에 ID를 저장한다.
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	unsigned vertexSize = 8 * sizeof(float);
	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}

	// 정정 배열 개체를 생성한 후 정점 버퍼를 생성한다.
	glGenBuffers(1, &mVertexBuffer);
	// 첫번째 인자는 mVertexBuffer를 버퍼로 사용하겠다는 의미
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	// 정점배열 생성자로 전달된 정점 데이터를 정점 버퍼로 복사한다.
	// 개체 ID를 전달하지 않고 대신 현재 바인딩될 버퍼의 타입을 지정한다.
	glBufferData(
		// 데이터를 쓸 버퍼의 버퍼 타입(막 생성한 버퍼를 사용한다는 의미)
		GL_ARRAY_BUFFER,
		// 복사할 바이트 크기(x,y,z 3개의 float을 포함) + UV 좌표 + 번선 벡터
		numVerts * vertexSize,
		// 복사할 소스(포인터)
		verts,				
		// 데이터를 어떻게 사용할지(데이터를 오직 한 번만 로드, 자주 그려지는 경우 사용되는 옵션)
		GL_STATIC_DRAW
	);

	// 인덱스 버퍼 생성
	// 버퍼 타입을 인덱스 버퍼로 지정하는 것을 제외하고 정점 버퍼 생성과 비슷하다.
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	// 인덱스 데이터를 인덱스 버퍼로 복사한다.
	glBufferData(
		// 인덱스 버퍼
		GL_ELEMENT_ARRAY_BUFFER,
		// 데이터의 크기
		numIndices * sizeof(unsigned int),
		indices,
		GL_STATIC_DRAW
	);

	// vertex 속성
	if (layout == PosNormTex)
	{
		// 정점 속성으로 불리는 정점 레이아웃(vertex layout)을 지정
		// 현재 레이아웃은 3개의 float값을 가진 위치이다.
		// 첫번째 정점 속성(속성 0)을 활성화
		glEnableVertexAttribArray(0);
		// 크기와 타입, 속성의 포맷을 지정
		glVertexAttribPointer(
			// 속성 인덱스(첫 번째 정점 속성 인덱스는 0)
			0,
			// 요소의 수(이번 예시는 3, x,y,z) 현재 위치 좌표만 지정
			3,
			// 요소의 타입
			GL_FLOAT,
			// 정수형 타입에서만 사용된다. (값을 정규화해야하는 지 여부)
			GL_FALSE,
			// 간격 (일반적으로 각 정점의 크기), 연속한 정점 사이의 byte 오프셋
			// 정점 버퍼에서 패딩(padding)이 없다면 정점의 크기가 간격이된다.
			vertexSize,
			// 정점의 시작에서 이 속성까지의 오프셋
			// 위치 속성은 정점의 시작 위치와 동일하기에 0이다.
			// (추가 속성에 대해서는 0이 아닌 값을 전달해야 한다)
			0
		); // 이건 위치 정점 속성만 수정한다.

		// 두번째 정점 속성인 법선 벡터 (속성 1 활성화)
		glEnableVertexAttribArray(1);
		// 벅선 벡터 포맷 지정
		glVertexAttribPointer(
			1,	// 버텍스 속성 인덱스
			3,	// 컴포넌트의 수 (법선 벡터, 3)
			GL_FLOAT,	// 컴포넌트 타입
			GL_FALSE,	// float 타입이므로 false
			vertexSize,	// 간격 (항상 버텍스의 크기, 위치좌표 + 법선벡터 + 텍스처좌표 )
			// (void*) 타입
			reinterpret_cast<void*>(sizeof(float) * 3)	// 오프셋 포인터(앞에 위치좌표가 있으므로)
		);

		// 세번째 정점 속성인 텍스처 좌표 (속성 2 활성화)
		glEnableVertexAttribArray(2);
		// 텍스처 좌표 포맷 지정
		glVertexAttribPointer(
			2,	// 속성 인덱스
			2,	// uv 좌표
			GL_FLOAT,
			GL_FALSE,
			vertexSize, // vertex의 간격
			reinterpret_cast<void*>(sizeof(float) * 6) // 오프셋(위치+법선)
		);
	}
	// 본을 지원하는 셰이더
	else if (layout == PosNormSkinTex)
	{
		// 위치는 3개의 float
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
		// 법선은 3개의 float
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// 스키닝 본(unsigned int)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// 스키닝 가중치 (float로 변환)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

		// 텍스처 좌표
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
	

}

// 정점 버퍼, 인덱스 버퍼, 정점 배열 객체를 해제한다.
VertexArray::~VertexArray() {
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

// 현재 사용할 정점 배열을 지정하는 함수를 호출
void VertexArray::SetActive() {
	glBindVertexArray(mVertexArray);
}

unsigned int VertexArray::GetVertexSize(VertexArray::Layout layout)
{
	unsigned vertexSize = 8 * sizeof(float);
	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	return vertexSize;
}

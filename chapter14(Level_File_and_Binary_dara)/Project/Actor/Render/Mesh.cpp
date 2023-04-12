#include "..\..\PreCompiled.hpp"

namespace
{
	union Vertex
	{
		float f;
		uint8_t b[4];
	};

	const int BinaryVersion = 1;
	struct MeshBinHeader
	{
		// ���� Ÿ�� �ñ״�ó
		char mSignature[4] = { 'G', 'M', 'S', 'H' };
		// Version
		uint32_t mVersion = BinaryVersion;
		// Vertex layout Ÿ��
		VertexArray::Layout mLayout = VertexArray::PosNormTex;
		// �ؽ�ó, vertex, �ε��� ���� ���� ����
		uint32_t mNumTextures = 0;
		uint32_t mNumVerts = 0;
		uint32_t mNumIndices = 0;
		// �浹�� ���Ǵ� �ڽ�/������
		AABB mBox{ Vector3d::Zero, Vector3d::Zero };
		float mRadius = 0.0f;
		float mSpecPower = 100.0f;
	};
}

Mesh::Mesh() :mBox(Vector3d::Infinity, Vector3d::NegInfinity),
mVertexArray(nullptr), mRadius(0.f), mSpecPower(100.f) {}

Mesh::~Mesh() {}

bool Mesh::Load(const std::string& fileName, Renderer* renderer) {
	mFileName = fileName;

	// ���̳ʸ� ���� �����
	if (LoadBinary(fileName + ".bin", renderer))
	{
		return true;
	}

	std::ifstream file(fileName);
	if (!file.is_open()) {
		SDL_Log("File not found: Mesh %s", fileName.c_str());
		return false;
	}

	std::stringstream filesStream;
	filesStream << file.rdbuf();
	std::string contents = filesStream.str();

	// JSON Parsing
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	// JSON string�� document(DOM)���� Parsing
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Mesh %s is not valid json", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	// ���� Ȯ��
	if (ver != 1) {
		SDL_Log("Mesh %s not version 1", fileName.c_str());
		return false;
	}

	mShaderName = doc["shader"].GetString();

	// vertex layout/size�� ���� ���˿� ���� ����
	VertexArray::Layout layout = VertexArray::PosNormTex;
	size_t vertSize = 8;

	std::string vertexFormat = doc["vertexformat"].GetString();
	if (vertexFormat == "PosNormSkinTex")
	{
		layout = VertexArray::PosNormSkinTex;
		// number of "Vertex" union, which is 8 + 2 (for skinning)
		vertSize = 10;
	}

	// �ؽ�ó Load
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1) {
		SDL_Log("Mesh %s has no textues, ther should be at least one", fileName.c_str());
		return false;
	}
	
	// ���ݻ� ��
	mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

	for (rapidjson::SizeType i = 0; i < textures.Size(); i++) {
		// �ؽ�ó�� �̹� �ε����� Ȯ��
		std::string texName = textures[i].GetString();
		Texture* t = renderer->GetTexture(texName);
		if (t == nullptr) {
			// �ؽ�ó �ε�
			t = renderer->GetTexture(texName);
			if (t == nullptr) {
				// ���� ������ ���ٸ� default �ؽ�ó�� ����
				t = renderer->GetTexture("Assets/Default.png");
			}
		}
		mTextures.emplace_back(t);
	}

	// vertex buffer load
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
		SDL_Log("Mesh %s has no vertices", fileName.c_str());
		return false;
	}

	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// ������ �Ӽ��� ������ �� 8���� �����Ѵ�
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray())
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3d pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = MathUtils::Max(mRadius, pos.LengthSq());
		mBox.UpdateMinMax(pos);

		if (layout == VertexArray::PosNormTex)
		{
			Vertex v;
			// floats�� �־��ش�
			for (rapidjson::SizeType j = 0; j < vert.Size(); j++) {
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}
		}
		// �� �ִ� ���̴�
		else
		{
			Vertex v;
			// ��ġ/���� ���� ���ϱ�
			for (rapidjson::SizeType j = 0; j < 6; j++)
			{
				v.f = static_cast<float>(vert[j].GetDouble());
				vertices.emplace_back(v);
			}

			// skin ����
			for (rapidjson::SizeType j = 6; j < 14; j += 4)
			{
				v.b[0] = vert[j].GetUint();
				v.b[1] = vert[j + 1].GetUint();
				v.b[2] = vert[j + 2].GetUint();
				v.b[3] = vert[j + 3].GetUint();
				vertices.emplace_back(v);
			}

			// �ؽ�ó ��ǥ
			for (rapidjson::SizeType j = 14; j < vert.Size(); j++)
			{
				v.f = vert[j].GetDouble();
				vertices.emplace_back(v);
			}
		}
	}

	// ������ �������� ������ ���س���.
	mRadius = MathUtils::Sqrt(mRadius);

	// index buffer load
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1) {
		SDL_Log("Mesh %s has no indices", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	// �ﰢ���̹Ƿ� ���� int*3�� ���������.
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3) {
			SDL_Log("Invalid indices for %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// vertex �迭 ����
	mVertexArray = new VertexArray(vertices.data(),
		static_cast<unsigned>(vertices.size()) / vertSize,
		layout, indices.data(), static_cast<unsigned>(indices.size()));

	return true;
}

void Mesh::UnLoad() {
	delete mVertexArray;
	mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(size_t index) {
	// �ؽ�ó�� ������ �ؽ�ó return
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}


void Mesh::SaveBinary(const std::string& fileName, const void* verts,
	uint32_t numVerts, VertexArray::Layout layout,
	const uint32_t* indices, uint32_t numIndices,
	const std::vector<std::string>& textureNames,
	const AABB& box, float radius,
	float specPower)
{
	// ��� ����ü ����
	MeshBinHeader header;
	header.mLayout = layout;
	header.mNumTextures =
		static_cast<unsigned>(textureNames.size());
	header.mNumVerts = numVerts;
	header.mNumIndices = numIndices;
	header.mBox = box;
	header.mRadius = radius;

	// ���Ͽ� ���� ���� ���̳ʸ� ���� ����
	std::ofstream outFile(fileName, std::ios::out
		| std::ios::binary);
	if (outFile.is_open())
	{
		// ����� ���Ͽ� ����.
		outFile.write(reinterpret_cast<char*>(&header), sizeof(header));

		// �ؽ�ó�� ���ؼ��� �̸� ���ڿ��� ���̸� �� �ʿ䰡 �ִ�.
		// ���⿡ null ���� ���� 1�� ���Ѵ�.
		for (const auto& tex : textureNames)
		{
			// (Assume file names won't have more than 32k characters)
			uint16_t nameSize = static_cast<uint16_t>(tex.length()) + 1;
			outFile.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
			outFile.write(tex.c_str(), nameSize - 1);
			outFile.write("\0", 1);
		}

		// ���̾ƿ��� �м��ؼ� ���ؽ��� ����Ʈ ���� �˾Ƴ���.
		unsigned vertexSize = VertexArray::GetVertexSize(layout);
		// ���ؽ� �����͸� ���Ͽ� ����.
		outFile.write(reinterpret_cast<const char*>(verts),
			numVerts * vertexSize);
		// �ε��� �����͸� ���Ͽ� ����.
		outFile.write(reinterpret_cast<const char*>(indices),
			numIndices * sizeof(uint32_t));
	}
}

bool Mesh::LoadBinary(const std::string& fileName, Renderer* renderer)
{
	std::ifstream inFile(fileName, std::ios::in |
		std::ios::binary);
	if (inFile.is_open())
	{
		// Read in header
		MeshBinHeader header;
		inFile.read(reinterpret_cast<char*>(&header), sizeof(header));

		// ��� �ñ״�ó�� ���������� ��ȿ�� ����
		char* sig = header.mSignature;
		if (sig[0] != 'G' || sig[1] != 'M' || sig[2] != 'S' ||
			sig[3] != 'H' || header.mVersion != BinaryVersion)
		{
			return false;
		}

		// �ؽ�ó ���� �̸��� �д´�.
		for (uint32_t i = 0; i < header.mNumTextures; i++)
		{
			// Get the file name size
			uint16_t nameSize = 0;
			inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

			// Make a buffer of this size
			char* texName = new char[nameSize];
			// Read in the texture name
			inFile.read(texName, nameSize);

			// Get this texture
			Texture* t = renderer->GetTexture(texName);
			if (t == nullptr)
			{
				// If it's null, use the default texture
				t = renderer->GetTexture("Assets/Default.png");
			}
			mTextures.emplace_back(t);

			delete[] texName;
		}

		// vertx�� index �����͸� �о���δ�.
		unsigned vertexSize = VertexArray::GetVertexSize(header.mLayout);
		char* verts = new char[header.mNumVerts * vertexSize];
		inFile.read(verts, header.mNumVerts * vertexSize);

		// Now read in the indices
		uint32_t* indices = new uint32_t[header.mNumIndices];
		inFile.read(reinterpret_cast<char*>(indices),
			header.mNumIndices * sizeof(uint32_t));

		// ���ؽ� �迭�� �����Ѵ�.
		mVertexArray = new VertexArray(verts, header.mNumVerts,
			header.mLayout, indices, header.mNumIndices);

		// ���ؽ� ���ۿ� �ε��� ���۸� �����Ѵ�.
		delete[] verts;
		delete[] indices;

		// Set mBox/mRadius/specular from header
		mBox = header.mBox;
		mRadius = header.mRadius;
		mSpecPower = header.mSpecPower;

		return true;
	}
	return false;
}

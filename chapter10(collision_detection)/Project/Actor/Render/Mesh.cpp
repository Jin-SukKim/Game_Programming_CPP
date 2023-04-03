#include "..\..\PreCompiled.hpp"

Mesh::Mesh() :mBox(Vector3d::Infinity, Vector3d::NegInfinity),
mVertexArray(nullptr), mRadius(0.f), mSpecPower(100.f) {}

Mesh::~Mesh() {}

bool Mesh::Load(const std::string& fileName, Renderer* renderer) {
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

	// ������ vertex format/shader�� ��ŵ�Ѵ�
	// ���߿� �ڼ��� �ٷ��.
	size_t vertSize = 8;

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

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	mRadius = 0.f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// ������ �Ӽ��� ������ �� 8���� �����Ѵ�
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			SDL_Log("Unexpected vertex format for %s", fileName.c_str());
			return false;
		}

		Vector3d pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = MathUtils::Max(mRadius, pos.LengthSq());
		mBox.UpdateMinMax(pos);

		// floats�� �־��ش�
		for (rapidjson::SizeType i = 0; i < vert.Size(); i++) {
			vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
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
	mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size()) / vertSize,
		indices.data(), static_cast<unsigned>(indices.size()));
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
#include "PreCompiled.hpp"

// ������ ����
const int LevelVersion = 1;

std::unordered_map<std::string, ActorFunc> LevelLoader::sActorFactoryMap
{
	{ "Actor", &Actor::Create<Actor> },
	{ "BallActor", &Actor::Create<BallActor> },
	{ "FollowActor", &Actor::Create<FollowActor> },
	{ "PlaneActor", &Actor::Create<PlaneActor> },
	{ "TargetActor", &Actor::Create<TargetActor> },
};

std::unordered_map<std::string, std::pair<int, ComponentFunc>> LevelLoader::sComponentFactoryMap
{
	{ "AudioComponent", { Component::TAudioComponent, &Component::Create<AudioComponent>} },
	{ "BallMove", { Component::TBallMove, &Component::Create<BallMove> } },
	{ "BoxComponent", { Component::TBoxComponent, &Component::Create<BoxComponent> } },
	{ "CameraComponent", { Component::TCameraComponent, &Component::Create<CameraComponent> } },
	{ "FollowCamera", { Component::TFollowCamera, &Component::Create<FollowCamera> } },
	{ "MeshComponent", { Component::TMeshComponent, &Component::Create<MeshComponent> } },
	{ "MoveComponent", { Component::TMoveComponent, &Component::Create<MoveComponent> } },
	{ "SkeletalMeshComponent", { Component::TSkeletalMeshComponent, &Component::Create<SkeletalMeshComponent> } },
	{ "SpriteComponent", { Component::TSpriteComponent, &Component::Create<SpriteComponent> } },
	{ "MirrorCamera", { Component::TMirrorCamera, &Component::Create<MirrorCamera> } },
	{ "PointLightComponent", { Component::TPointLightComponent, &Component::Create<PointLightComponent> }},
	{ "TargetComponent",{ Component::TTargetComponent, &Component::Create<TargetComponent> } },
};


bool LevelLoader::LoadLevel(Game* game, const std::string& fileName)
{
	rapidjson::Document doc;
	// JSON ������ Document�� Parsing
	if (!LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load level %s", fileName.c_str());
		return false;
	}


	// �ε��� ������ ������ ��밪�� �������� ����
	int version = 0;
	if (!JsonHelper::GetInt(doc, "version", version) ||
		version != LevelVersion)
	{
		SDL_Log("Incorrect level file version for %s", fileName.c_str());
		return false;
	}

	// ���� �Ӽ��� �ٷ��.
	const rapidjson::Value& globals = doc["globalProperties"];
	if (globals.IsObject())
	{
		LoadGlobalProperties(game, globals);
	}

	// ���͸� �ε��Ѵ�.
	const rapidjson::Value& actors = doc["actors"];
	if (actors.IsArray())
	{
		LoadActors(game, actors);
	}
	return true;
}

bool LevelLoader::LoadJSON(const std::string& fileName,
	rapidjson::Document& outDoc)
{
	// ���� ���� ifstream�� ��ũ ������ �ε��ϰ�
	// ��Ʈ�� ������ ��ġ�� ���� ���������� �����Ѵ�(std::ios::ate �÷���)
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		SDL_Log("File %s not found", fileName.c_str());
		return false;
	}

	// ������ ũ�⸦ ��´�.
	// tellg�� ���� ���� ��Ʈ���� ���� ��ġ�� ��µ�
	// ���� ������ ���� �����Ƿ� ������� ��ü ������ ũ���.
	std::ifstream::pos_type fileSize = file.tellg();
	// ��Ʈ�� ���۰� ���� ó�� �κ��� ����Ű���� �ǵ�����.
	file.seekg(0, std::ios::beg);

	// ���� ������ + 1(null ����) ũ���� ���ͷ� �����Ѵ�.
	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	// ������ �����͸� ���ͷ� �о���δ�.
	file.read(bytes.data(), static_cast<size_t>(fileSize));

	// RapidJSON�� Document ��ü�� �����͸� �Ѱܼ� parsing�Ѵ�.
	outDoc.Parse(bytes.data());
	if (!outDoc.IsObject())
	{
		SDL_Log("File %s is not valid JSON", fileName.c_str());
		return false;
	}

	return true;
}

void LevelLoader::LoadGlobalProperties(Game* game, const rapidjson::Value& inObject)
{
	// �ֺ����� ��´�.
	Vector3d ambient;
	if (JsonHelper::GetVector3(inObject, "ambientLight", ambient))
	{
		game->GetRenderer()->SetAmbientLight(ambient);
	}


	// ���Ɽ�� ��´�.
	const rapidjson::Value& dirObj = inObject["directionalLight"];
	if (dirObj.IsObject())
	{
		DirectionalLight& light = game->GetRenderer()->GetDirectionalLight();

		// ���Ɽ �Ӽ�, driection/color�� �����ϸ� ���� �д´�.
		JsonHelper::GetVector3(dirObj, "direction", light.mDirection);
		JsonHelper::GetVector3(dirObj, "color", light.mDiffuseColor);
	}
}

void LevelLoader::LoadActors(Game* game, const rapidjson::Value& inArray)
{
	// ���� �迭�� ��ȸ�Ѵ�.
	for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& actorObj = inArray[i];
		if (actorObj.IsObject())
		{
			// Ÿ���� ��´�.
			std::string type;
			if (JsonHelper::GetString(actorObj, "type", type))
			{
				// �ش� Ÿ���� �ʿ� �����ϴ°�?
				auto itr = sActorFactoryMap.find(type);
				if (itr != sActorFactoryMap.end())
				{
					// �ʿ� ����� �Լ��� ���� ����
					Actor* actor = itr->second(game, actorObj["properties"]);
					// ������ ������Ʈ�� ��´�.
					if (actorObj.HasMember("components"))
					{
						const rapidjson::Value& components = actorObj["components"];
						if (components.IsArray())
						{
							LoadComponents(actor, components);
						}
					}
				}
				else
				{
					SDL_Log("Unkown actor type %s", type.c_str());
				}
			}
		}
	}
}

void LevelLoader::LoadComponents(Actor* actor, const rapidjson::Value& inArray)
{
	// ������Ʈ �迭�� ��ȸ�Ѵ�.
	for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& compObj = inArray[i];
		if (compObj.IsObject())
		{
			// ������Ʈ�� Ÿ���� ��´�.
			std::string type;
			if (JsonHelper::GetString(compObj, "type", type))
			{
				auto itr = sComponentFactoryMap.find(type);
				if (itr != sComponentFactoryMap.end())
				{
					// ������Ʈ�� Ÿ�� ID�� ��´�.
					Component::TypeID tid = static_cast<Component::TypeID>
						(itr->second.first);
					// ���Ͱ� �̹� ������Ʈ�� ������ �ִ��� Ȯ��
					Component* comp = actor->GetComponentOfType(tid);
					if (comp == nullptr)
					{
						// ���ٸ� �� ������Ʈ�̹Ƿ� �ʿ� ����� ������Ʈ ���� �Լ� ȣ��
						comp = itr->second.second(actor, compObj["properties"]);
					}
					else
					{
						// �̹� ������Ʈ�� �ִٸ� �ٷ� �Ӽ����� �ε�
						comp->LoadProperties(compObj["properties"]);
					}
				}
				else
				{
					SDL_Log("Unkown component type %s", type.c_str());
				}
			}
		}
	}
}

void LevelLoader::SaveLevel(Game* game, const std::string& fileName)
{
	// Document�� �����ϰ� ��Ʈ ������Ʈ�� ����
	rapidjson::Document doc;
	doc.SetObject();

	// ������ ����Ѵ�.
	JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

	// ���� �Ӽ�
	rapidjson::Value globals(rapidjson::kObjectType);
	SaveGlobalProperties(doc.GetAllocator(), game, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());

	// ����
	rapidjson::Value actors(rapidjson::kArrayType);
	SaveActors(doc.GetAllocator(), game, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());

	// JSON�� ���ڿ� ���ۿ� ����
	rapidjson::StringBuffer buffer;
	// �������� ���̱� ���� PrettyWriter ���
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	const char* output = buffer.GetString();

	// ���۸� ���Ͽ� ����.
	std::ofstream outFile(fileName);
	if (outFile.is_open())
	{
		outFile << output;
	}

}


void LevelLoader::SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc,
	Game* game, rapidjson::Value& inObject)
{
	// Ambient light
	JsonHelper::AddVector3(alloc, inObject, "ambientLight",
		game->GetRenderer()->GetAmbientLight());

	// Directional light
	DirectionalLight& dirLight = game->GetRenderer()->GetDirectionalLight();
	rapidjson::Value dirObj(rapidjson::kObjectType);
	JsonHelper::AddVector3(alloc, dirObj, "direction", dirLight.mDirection);
	JsonHelper::AddVector3(alloc, dirObj, "color", dirLight.mDiffuseColor);
	inObject.AddMember("directionalLight", dirObj, alloc);
}

void LevelLoader::SaveActors(rapidjson::Document::AllocatorType& alloc,
	Game* game, rapidjson::Value& inArray)
{
	const auto& actors = game->GetActors();
	for (const Actor* actor : actors)
	{
		// JSON ������Ʈ�� �����.
		rapidjson::Value obj(rapidjson::kObjectType);
		// Ÿ���� �߰��Ѵ�.
		JsonHelper::AddString(alloc, obj, "type", Actor::TypeNames[actor->GetType()]);

		// �Ӽ� ������Ʈ�� �����.
		rapidjson::Value props(rapidjson::kObjectType);
		// �Ӽ��� �����Ѵ�.
		actor->SaveProperties(alloc, props);
		// JSON ������Ʈ�� �Ӽ��� �߰��Ѵ�.
		obj.AddMember("properties", props, alloc);

		// ������Ʈ�� �����Ѵ�.
		rapidjson::Value components(rapidjson::kArrayType);
		SaveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);

		// ���͸� inArray�� �߰��Ѵ�.
		inArray.PushBack(obj, alloc);
	}
}

void LevelLoader::SaveComponents(rapidjson::Document::AllocatorType& alloc,
	const Actor* actor, rapidjson::Value& inArray)
{
	const auto& components = actor->GetComponents();
	for (const Component* comp : components)
	{
		// Make a JSON object
		rapidjson::Value obj(rapidjson::kObjectType);
		// Add type
		JsonHelper::AddString(alloc, obj, "type", Component::TypeNames[comp->GetType()]);

		// Make an object for properties
		rapidjson::Value props(rapidjson::kObjectType);
		// Save rest of properties
		comp->SaveProperties(alloc, props);
		// Add the member
		obj.AddMember("properties", props, alloc);

		// Add component to array
		inArray.PushBack(obj, alloc);
	}
}
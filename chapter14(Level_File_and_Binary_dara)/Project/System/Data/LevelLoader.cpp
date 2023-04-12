#include "PreCompiled.hpp"

// 파일의 버전
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
	// JSON 파일을 Document로 Parsing
	if (!LoadJSON(fileName, doc))
	{
		SDL_Log("Failed to load level %s", fileName.c_str());
		return false;
	}


	// 로드한 파일의 버전이 기대값과 동일한지 검증
	int version = 0;
	if (!JsonHelper::GetInt(doc, "version", version) ||
		version != LevelVersion)
	{
		SDL_Log("Incorrect level file version for %s", fileName.c_str());
		return false;
	}

	// 전역 속성을 다룬다.
	const rapidjson::Value& globals = doc["globalProperties"];
	if (globals.IsObject())
	{
		LoadGlobalProperties(game, globals);
	}

	// 액터를 로드한다.
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
	// 이진 모드로 ifstream에 디스크 파일을 로드하고
	// 스트림 버퍼의 위치를 파일 마지막으로 설정한다(std::ios::ate 플래그)
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		SDL_Log("File %s not found", fileName.c_str());
		return false;
	}

	// 파일의 크기를 얻는다.
	// tellg를 통해 파일 스트림의 현재 위치를 얻는데
	// 현재 파일의 끝에 있으므로 결과값은 전체 파일의 크기다.
	std::ifstream::pos_type fileSize = file.tellg();
	// 스트림 버퍼가 파일 처음 부분을 가리키도록 되돌린다.
	file.seekg(0, std::ios::beg);

	// 파일 사이즈 + 1(null 문자) 크기의 벡터로 선언한다.
	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	// 파일의 데이터를 벡터로 읽어들인다.
	file.read(bytes.data(), static_cast<size_t>(fileSize));

	// RapidJSON의 Document 객체로 데이터를 넘겨서 parsing한다.
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
	// 주변광을 얻는다.
	Vector3d ambient;
	if (JsonHelper::GetVector3(inObject, "ambientLight", ambient))
	{
		game->GetRenderer()->SetAmbientLight(ambient);
	}


	// 방향광을 얻는다.
	const rapidjson::Value& dirObj = inObject["directionalLight"];
	if (dirObj.IsObject())
	{
		DirectionalLight& light = game->GetRenderer()->GetDirectionalLight();

		// 방향광 속성, driection/color가 존재하면 값을 읽는다.
		JsonHelper::GetVector3(dirObj, "direction", light.mDirection);
		JsonHelper::GetVector3(dirObj, "color", light.mDiffuseColor);
	}
}

void LevelLoader::LoadActors(Game* game, const rapidjson::Value& inArray)
{
	// 액터 배열을 조회한다.
	for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& actorObj = inArray[i];
		if (actorObj.IsObject())
		{
			// 타입을 얻는다.
			std::string type;
			if (JsonHelper::GetString(actorObj, "type", type))
			{
				// 해당 타입이 맵에 존재하는가?
				auto itr = sActorFactoryMap.find(type);
				if (itr != sActorFactoryMap.end())
				{
					// 맵에 저장된 함수로 액터 생성
					Actor* actor = itr->second(game, actorObj["properties"]);
					// 액터의 컴포넌트를 얻는다.
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
	// 컴포넌트 배열을 조회한다.
	for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
	{
		const rapidjson::Value& compObj = inArray[i];
		if (compObj.IsObject())
		{
			// 컴포넌트의 타입을 얻는다.
			std::string type;
			if (JsonHelper::GetString(compObj, "type", type))
			{
				auto itr = sComponentFactoryMap.find(type);
				if (itr != sComponentFactoryMap.end())
				{
					// 컴포넌트의 타입 ID를 얻는다.
					Component::TypeID tid = static_cast<Component::TypeID>
						(itr->second.first);
					// 액터가 이미 컴포넌트를 가지고 있는지 확인
					Component* comp = actor->GetComponentOfType(tid);
					if (comp == nullptr)
					{
						// 없다면 새 컴포넌트이므로 맵에 저장된 컴포넌트 생성 함수 호출
						comp = itr->second.second(actor, compObj["properties"]);
					}
					else
					{
						// 이미 컴포넌트가 있다면 바로 속성값을 로드
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
	// Document를 생성하고 루트 오브젝트로 지정
	rapidjson::Document doc;
	doc.SetObject();

	// 버전을 기록한다.
	JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

	// 전역 속성
	rapidjson::Value globals(rapidjson::kObjectType);
	SaveGlobalProperties(doc.GetAllocator(), game, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());

	// 액터
	rapidjson::Value actors(rapidjson::kArrayType);
	SaveActors(doc.GetAllocator(), game, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());

	// JSON을 문자열 버퍼에 저장
	rapidjson::StringBuffer buffer;
	// 가독성을 높이기 위해 PrettyWriter 사용
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	const char* output = buffer.GetString();

	// 버퍼를 파일에 쓴다.
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
		// JSON 오브젝트를 만든다.
		rapidjson::Value obj(rapidjson::kObjectType);
		// 타입을 추가한다.
		JsonHelper::AddString(alloc, obj, "type", Actor::TypeNames[actor->GetType()]);

		// 속성 오브젝트를 만든다.
		rapidjson::Value props(rapidjson::kObjectType);
		// 속성을 저장한다.
		actor->SaveProperties(alloc, props);
		// JSON 오브젝트에 속성을 추가한다.
		obj.AddMember("properties", props, alloc);

		// 컴포넌트를 저장한다.
		rapidjson::Value components(rapidjson::kArrayType);
		SaveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);

		// 액터를 inArray에 추가한다.
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
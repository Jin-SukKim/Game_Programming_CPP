using ActorFunc = std::function<class Actor* (class Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<
	class Component* (class Actor*, const rapidjson::Value&)>;

class LevelLoader
{
public:
	// 레벨을 로드한다 - 성공하면 true 반환
	static bool LoadLevel(class Game* game, const std::string& fileName);
	// JSON 파일을 RapidJSON document로 로드하는 헬퍼함수
	static bool LoadJSON(const std::string& fileName, rapidjson::Document& outDoc);
	// Level을 JSON 형태 파일로 저장
	static void SaveLevel(class Game* game, const std::string& fileName);
protected:
	// 전역 속성을 로드한다.
	static void LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
	// 생성할 여러 액터로 구성된 맵
	static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
	// 액터를 로드한다.
	static void LoadActors(class Game* game, const rapidjson::Value& inArray);
	// 컴포넌트를 로드한다.
	static void LoadComponents(class Actor* actor, const rapidjson::Value& inArray);
	static std::unordered_map<std::string, std::pair<int, ComponentFunc>> sComponentFactoryMap;
	// 전역 속성 저장
	static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc,
		class Game* game, rapidjson::Value& inObject);
	// 액터 저장
	static void SaveActors(rapidjson::Document::AllocatorType& alloc,
		class Game* game, rapidjson::Value& inArray);
	// H컴포넌트 저장
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc,
		const class Actor* actor, rapidjson::Value& inArray);
};
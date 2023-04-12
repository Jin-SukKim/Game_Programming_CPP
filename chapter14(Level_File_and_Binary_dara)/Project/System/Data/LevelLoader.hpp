using ActorFunc = std::function<class Actor* (class Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<
	class Component* (class Actor*, const rapidjson::Value&)>;

class LevelLoader
{
public:
	// ������ �ε��Ѵ� - �����ϸ� true ��ȯ
	static bool LoadLevel(class Game* game, const std::string& fileName);
	// JSON ������ RapidJSON document�� �ε��ϴ� �����Լ�
	static bool LoadJSON(const std::string& fileName, rapidjson::Document& outDoc);
	// Level�� JSON ���� ���Ϸ� ����
	static void SaveLevel(class Game* game, const std::string& fileName);
protected:
	// ���� �Ӽ��� �ε��Ѵ�.
	static void LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
	// ������ ���� ���ͷ� ������ ��
	static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
	// ���͸� �ε��Ѵ�.
	static void LoadActors(class Game* game, const rapidjson::Value& inArray);
	// ������Ʈ�� �ε��Ѵ�.
	static void LoadComponents(class Actor* actor, const rapidjson::Value& inArray);
	static std::unordered_map<std::string, std::pair<int, ComponentFunc>> sComponentFactoryMap;
	// ���� �Ӽ� ����
	static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc,
		class Game* game, rapidjson::Value& inObject);
	// ���� ����
	static void SaveActors(rapidjson::Document::AllocatorType& alloc,
		class Game* game, rapidjson::Value& inArray);
	// H������Ʈ ����
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc,
		const class Actor* actor, rapidjson::Value& inArray);
};
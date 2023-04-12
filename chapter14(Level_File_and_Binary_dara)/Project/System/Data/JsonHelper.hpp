class JsonHelper
{
public:
	// 속성을 찾은 뒤 그 속성이 정수 타입과 일치하는지 검증한다.
	static bool GetInt(const rapidjson::Value& inObject,
		const char* inProperty, int& outInt);
	static bool GetFloat(const rapidjson::Value& inObject,
		const char* inProperty, float& outFloat);
	static bool GetString(const rapidjson::Value& inObject,
		const char* inProperty, std::string& outString);
	static bool GetBool(const rapidjson::Value& inObject,
		const char* inProperty, bool& outBool);
	static bool GetVector3(const rapidjson::Value& inObject,
		const char* inProperty, Vector3d& outVector);
	static bool GetQuaternion(const rapidjson::Value& inObject,
		const char* inProperty, Quaternion& outQuat);

	// 파일 저장을 위한 세팅 함수들
	static void AddInt(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, int value);
	static void AddFloat(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, float value);
	static void AddString(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const std::string& value);
	static void AddBool(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, bool value);
	static void AddVector3(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const Vector3d& value);
	static void AddQuaternion(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObject, const char* name, const Quaternion& value);
};
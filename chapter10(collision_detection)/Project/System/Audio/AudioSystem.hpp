// Forward declarations
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};


class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	void Update(float deltaTime);

	// load 뱅크
	void LoadBank(const std::string& name);

	// unload 뱅크
	void UnloadBank(const std::string& name);
	// 모든 뱅크를 언로드하며 mEvents와 mBanks를 정리한다.
	void UnloadAllBanks();

	// 이벤트 재생
	SoundEvent PlayEvent(const std::string& name);

	// 리스너 설정(1인칭이라 카메라의 세계 위치
	void SetListener(const Matrix4x4& viewMatrix);

	// 버스 조작
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	class Game* mGame;

	// FMOD 스튜디오 시스템
	FMOD::Studio::System* mSystem;
	// FMOD 저수준 시스템
	FMOD::System* mLowLevelSystem;

	// 로드된 뱅크를 관리하는 맵
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	// 이벤트 이름과 EventDescription 맵
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;

	// 이벤트를 관리하기 위한 맵
	std::unordered_map<unsigned int,
		FMOD::Studio::EventInstance*> mEventInstances;
	// 다음 ID 추적
	static unsigned int sNextID;

	// 버스 맵
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};
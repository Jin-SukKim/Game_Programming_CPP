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

	// load ��ũ
	void LoadBank(const std::string& name);

	// unload ��ũ
	void UnloadBank(const std::string& name);
	// ��� ��ũ�� ��ε��ϸ� mEvents�� mBanks�� �����Ѵ�.
	void UnloadAllBanks();

	// �̺�Ʈ ���
	SoundEvent PlayEvent(const std::string& name);

	// ������ ����(1��Ī�̶� ī�޶��� ���� ��ġ
	void SetListener(const Matrix4x4& viewMatrix);

	// ���� ����
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	class Game* mGame;

	// FMOD ��Ʃ��� �ý���
	FMOD::Studio::System* mSystem;
	// FMOD ������ �ý���
	FMOD::System* mLowLevelSystem;

	// �ε�� ��ũ�� �����ϴ� ��
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	// �̺�Ʈ �̸��� EventDescription ��
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;

	// �̺�Ʈ�� �����ϱ� ���� ��
	std::unordered_map<unsigned int,
		FMOD::Studio::EventInstance*> mEventInstances;
	// ���� ID ����
	static unsigned int sNextID;

	// ���� ��
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};
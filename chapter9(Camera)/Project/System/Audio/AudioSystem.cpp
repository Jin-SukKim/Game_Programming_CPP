#include "..\PreCompiled.hpp"


unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	: mGame(game), mSystem(nullptr), mLowLevelSystem(nullptr) {}

AudioSystem::~AudioSystem() {}

bool AudioSystem::Initialize() {

	// FMOD �ʱ�ȭ
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // ������ ��쿡�� �α׸� �����.
		FMOD_DEBUG_MODE_TTY		// stdout���� �α� ���
	);

	// FMOD ��Ʃ��� �ý����� �ν��Ͻ� ����
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s",
			FMOD_ErrorString(result));
		return false;
	}

	// Initialize FMOD system
	result = mSystem->initialize(
		512,		// ���ÿ� ����� �� �ִ� ������ �ִ� ����
		FMOD_STUDIO_INIT_NORMAL,	// �⺻ ���� ���
		FMOD_INIT_NORMAL,			// �⺻ ����
		nullptr						// ��κ� nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	// low-level �ý��� pointer ����
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	// Load Master banks
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}


void AudioSystem::Shutdown()
{
	// ��� ��ũ ��ε�
	UnloadAllBanks();
	// FMOD system ����
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::Update(float deltaTime)
{
	// ������ �̺�Ʈ�� ã�´�.
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		// �̺�Ʈ�� ���°��� ��´�
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			// �̺�Ʈ�� �����ϰ� id�� done �ʿ� �߰��Ѵ�.
			e->release();
			done.emplace_back(iter.first);
		}
	}

	// �Ϸ�� �̺�Ʈ �ν��Ͻ��� �ʿ��� ����
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	// Update FMOD
	mSystem->update();
}

void AudioSystem::LoadBank(const std::string& name) {
	// �� �� �ε����� �ʰ� �Ѵ�.
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	// load ��ũ
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),	// ��ũ�� ���� �̸�
		FMOD_STUDIO_LOAD_BANK_NORMAL,	// �Ϲ����� ������� �ε�
		&bank			// ��ũ ������ ����
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		// ��ũ�� �ʿ� �߰�
		mBanks.emplace(name, bank);
		// ��Ʈ���� ������ �ƴ� ��� ���� �����͸� �ε�
		bank->loadSampleData();
		// ��ũ�� �̺�Ʈ�� ���� ��´�.
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			// ��ũ���� event description ����Ʈ�� ��´�.
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				// event:/Explosion2D ���� �̺�Ʈ�� ��θ� ��´�
				e->getPath(eventName, maxPathLength, nullptr);
				// �̺�Ʈ�� �ʿ� �߰��Ѵ�.
				mEvents.emplace(eventName, e);
			}
		}

		// ���� ���ڸ� ��ũ���� ���
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// ���� ����Ʈ
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// ���� ��ġ (ex: bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// �����ʿ� �߰�
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	// �ε� �ȵȰ� �н�
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// ��� �̺�Ʈ�� �����Ѵ�
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		// event description ��������
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// �̺�Ʈ�� ����Ʈ ��������
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// �̺�Ʈ ��� ���
			e->getPath(eventName, 512, nullptr);
			// �̺�Ʈ ����
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	// ��ũ���� ���� ���� ��������
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		// ���� ����Ʈ
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			// ���� ���
			bus->getPath(busName, 512, nullptr);
			// ���� ����
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	// ���õ����� ��ε�
	bank->unloadSampleData();
	// ��ũ ��ε�
	bank->unload();
	// ��ũ�� �ʿ��� ����
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		// ���� ������ ��ε�
		iter.second->unloadSampleData();
		// ��ũ ��ε�
		iter.second->unload();
	}

	mBanks.clear();
	// ��ũ�� ���ٸ� �̺�Ʈ�� ����
	mEvents.clear();
}


SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	// �̺�Ʈ�� �����ϴ��� Ȯ��
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		// �̺�Ʈ�� �ν��Ͻ��� �����Ѵ�.
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			// �̺�Ʈ �ν��Ͻ��� �����Ѵ�.
			event->start();
			// �� ���̵� ��� �ʿ� �߰��Ѵ�.
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}

	return SoundEvent(this, retID);
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3d& in)
	{
		// ��ġ(+x ��, +y ������, +z ��)�� 
		// FMOD ��ġ(+z ��, +x ������, +y ��)�� ��ȯ
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}


void AudioSystem::SetListener(const Matrix4x4& viewMatrix)
{
	// �� ����� ������� ���� �ʿ��� ���� ������ ���͸� ��´�.
	Matrix4x4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// ��ġ�� ���� ����, ���� ���͸� ����
	listener.position = VecToFMOD(invView.GetTranslation());
	// �� ����� ����Ŀ��� ����° ���� ���� ����
	listener.forward = VecToFMOD(invView.GetZAxis());
	// �ι�° ���� ���� ����
	listener.up = VecToFMOD(invView.GetYAxis());
	// �ӵ��� 0���� ����(���÷� ȿ�� ����� �� ����)
	listener.velocity = { 0.f, 0.f, 0.f };
	// FMOD�� ������ (0 = �����ʴ� �ϳ�)
	mSystem->setListenerAttributes(0, &listener);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}



float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}
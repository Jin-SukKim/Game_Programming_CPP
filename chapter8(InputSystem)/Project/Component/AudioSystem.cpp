#include "..\PreCompiled.hpp"


unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	: mGame(game), mSystem(nullptr), mLowLevelSystem(nullptr) {}

AudioSystem::~AudioSystem() {}

bool AudioSystem::Initialize() {

	// FMOD 초기화
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // 에러일 경우에만 로그를 남긴다.
		FMOD_DEBUG_MODE_TTY		// stdout으로 로그 출력
	);

	// FMOD 스튜디오 시스템의 인스턴스 생성
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
		512,		// 동시에 출력할 수 있는 사운드의 최대 갯수
		FMOD_STUDIO_INIT_NORMAL,	// 기본 설정 사용
		FMOD_INIT_NORMAL,			// 기본 설정
		nullptr						// 대부분 nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	// low-level 시스템 pointer 저장
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	// Load Master banks
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}


void AudioSystem::Shutdown()
{
	// 모든 뱅크 언로드
	UnloadAllBanks();
	// FMOD system 종료
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::Update(float deltaTime)
{
	// 정지된 이벤트를 찾는다.
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		// 이벤트의 상태값을 얻는다
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			// 이벤트를 해제하고 id를 done 맵에 추가한다.
			e->release();
			done.emplace_back(iter.first);
		}
	}

	// 완료된 이벤트 인스턴스를 맵에서 제거
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	// Update FMOD
	mSystem->update();
}

void AudioSystem::LoadBank(const std::string& name) {
	// 두 번 로딩되지 않게 한다.
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	// load 뱅크
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),	// 뱅크의 파일 이름
		FMOD_STUDIO_LOAD_BANK_NORMAL,	// 일반적인 방식으로 로딩
		&bank			// 뱅크 포인터 저장
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		// 뱅크를 맵에 추가
		mBanks.emplace(name, bank);
		// 스트리밍 형식이 아닌 모든 샘플 데이터를 로드
		bank->loadSampleData();
		// 뱅크의 이벤트의 수를 얻는다.
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			// 뱅크에서 event description 리스트를 얻는다.
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				// event:/Explosion2D 같은 이벤트의 경로를 얻는다
				e->getPath(eventName, maxPathLength, nullptr);
				// 이벤트를 맵에 추가한다.
				mEvents.emplace(eventName, e);
			}
		}

		// 버스 숫자를 뱅크에서 얻기
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// 버스 리스트
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// 버스 위치 (ex: bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// 버스맵에 추가
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	// 로드 안된건 패스
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// 모든 이벤트를 삭제한다
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		// event description 가져오기
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// 이벤트의 리스트 가져오기
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// 이벤트 경로 얻기
			e->getPath(eventName, 512, nullptr);
			// 이벤트 삭제
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	// 뱅크에서 버스 숫자 가져오기
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		// 버스 리스트
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			// 버스 경로
			bus->getPath(busName, 512, nullptr);
			// 버스 삭제
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	// 샐플데이터 언로드
	bank->unloadSampleData();
	// 뱅크 언로드
	bank->unload();
	// 뱅크를 맵에서 삭제
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		// 샘플 데이터 언로드
		iter.second->unloadSampleData();
		// 뱅크 언로드
		iter.second->unload();
	}

	mBanks.clear();
	// 뱅크가 없다면 이벤트도 없다
	mEvents.clear();
}


SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	// 이벤트가 존재하는지 확인
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		// 이벤트의 인스턴스를 생성한다.
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			// 이벤트 인스턴스를 시작한다.
			event->start();
			// 새 아이디를 얻어 맵에 추가한다.
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
		// 위치(+x 앞, +y 오른쪽, +z 위)를 
		// FMOD 위치(+z 앞, +x 오른쪽, +y 위)로 변환
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}


void AudioSystem::SetListener(const Matrix4x4& viewMatrix)
{
	// 뷰 행렬의 역행렬을 구해 필요한 세계 공간의 벡터를 얻는다.
	Matrix4x4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// 위치와 전방 벡터, 상향 벡터를 설정
	listener.position = VecToFMOD(invView.GetTranslation());
	// 뷰 행렬의 역행렬에서 세번째 행은 전방 벡터
	listener.forward = VecToFMOD(invView.GetZAxis());
	// 두번째 행을 상향 벡터
	listener.up = VecToFMOD(invView.GetYAxis());
	// 속도를 0으로 설정(도플러 효과 사용할 시 수정)
	listener.velocity = { 0.f, 0.f, 0.f };
	// FMOD로 보낸다 (0 = 리스너는 하나)
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
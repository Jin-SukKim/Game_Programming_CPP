#include "..\PreCompiled.hpp"

AudioComponent::AudioComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder) {}

AudioComponent::~AudioComponent()
{
	StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
	Component::Update(deltaTime);

	// 더이상 유효하지 않은 2D 이벤트 삭제
	auto iter = mEvents2D.begin();
	while (iter != mEvents2D.end())
	{
		if (!iter->IsValid()) {
			iter = mEvents2D.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	// 더이상 유효하지 않은 3D 이벤트 삭제
	iter = mEvents3D.begin();
	while (iter != mEvents3D.end())
	{
		if (!iter->IsValid()) {
			iter = mEvents3D.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void AudioComponent::OnUpdateWorldTransform()
{
	// 3D 이벤트 세계 위치 갱신
	Matrix4x4 world = mOwner->GetWorldTransform();
	for (auto& event : mEvents3D)
	{
		if (event.IsValid())
		{
			event.Set3DAttributes(world);
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	SoundEvent e = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);
	// 2D인지 3D인지 확인
	if (e.Is3D())
	{
		mEvents3D.emplace_back(e);
		// 초기 3D 속성 설정
		e.Set3DAttributes(mOwner->GetWorldTransform());
	}
	else
	{
		mEvents2D.emplace_back(e);
	}

	return e;
}

void AudioComponent::StopAllEvents()
{
	// 모든 이벤트 정지
	for (auto& e : mEvents2D)
	{
		e.Stop();
	}
	for (auto& e : mEvents3D)
	{
		e.Stop();
	}

	// vector 클리어
	mEvents2D.clear();
	mEvents3D.clear();
}
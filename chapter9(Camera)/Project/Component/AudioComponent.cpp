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

	// ���̻� ��ȿ���� ���� 2D �̺�Ʈ ����
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
	// ���̻� ��ȿ���� ���� 3D �̺�Ʈ ����
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
	// 3D �̺�Ʈ ���� ��ġ ����
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
	// 2D���� 3D���� Ȯ��
	if (e.Is3D())
	{
		mEvents3D.emplace_back(e);
		// �ʱ� 3D �Ӽ� ����
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
	// ��� �̺�Ʈ ����
	for (auto& e : mEvents2D)
	{
		e.Stop();
	}
	for (auto& e : mEvents3D)
	{
		e.Stop();
	}

	// vector Ŭ����
	mEvents2D.clear();
	mEvents3D.clear();
}
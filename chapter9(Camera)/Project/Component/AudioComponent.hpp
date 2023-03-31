class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();

	// 더 이상 유효하지 않는 이벤트를 제거한다.
	void Update(float deltaTime) override;
	// 세계 변환이 변경될 때마다 모든 3D 이벤트의 속성을 갱신
	void OnUpdateWorldTransform() override;
	SoundEvent PlayEvent(const std::string& name);
	// 두 벡터에 있는 모든 이벤트의 중단을 호출하고 벡터를 클리어
	void StopAllEvents();

private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};
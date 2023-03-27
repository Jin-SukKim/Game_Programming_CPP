class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder = 200);
	~AudioComponent();

	// �� �̻� ��ȿ���� �ʴ� �̺�Ʈ�� �����Ѵ�.
	void Update(float deltaTime) override;
	// ���� ��ȯ�� ����� ������ ��� 3D �̺�Ʈ�� �Ӽ��� ����
	void OnUpdateWorldTransform() override;
	SoundEvent PlayEvent(const std::string& name);
	// �� ���Ϳ� �ִ� ��� �̺�Ʈ�� �ߴ��� ȣ���ϰ� ���͸� Ŭ����
	void StopAllEvents();

private:
	std::vector<SoundEvent> mEvents2D;
	std::vector<SoundEvent> mEvents3D;
};
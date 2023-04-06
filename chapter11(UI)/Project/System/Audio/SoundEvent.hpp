class SoundEvent
{
public:
	SoundEvent();
	// ������ FMOD �̺�Ʈ�� �����ϸ� true return
	bool IsValid();
	// �̺�Ʈ �����
	void Restart();
	// �̺�Ʈ ����
	void Stop(bool allowFadeOut = true);
	// Setters
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);

	// Getters
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);

	// ��ġ
	bool Is3D() const;
	void Set3DAttributes(const Matrix4x4& worldTrans);

protected:
	// friend�� ������ ���� AudioSystem���� �� �����ڿ� ������ �� �ִ�.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};
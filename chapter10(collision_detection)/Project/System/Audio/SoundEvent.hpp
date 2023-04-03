class SoundEvent
{
public:
	SoundEvent();
	// 연관된 FMOD 이벤트가 존재하면 true return
	bool IsValid();
	// 이벤트 재시작
	void Restart();
	// 이벤트 정지
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

	// 위치
	bool Is3D() const;
	void Set3DAttributes(const Matrix4x4& worldTrans);

protected:
	// friend로 설정해 오직 AudioSystem만이 이 생성자에 접근할 수 있다.
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};
class CameraActor : public Actor
{
public:
	CameraActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	// 발소리
	void SetFootstepSurface(float value);

private:
	class MoveComponent* mMoveComp;

	// 사운드
	class AudioComponent* mAudioComp;
	class SoundEvent mFootstep;
	float mLastFootstep;
};
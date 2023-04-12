class CameraActor : public Actor
{
public:
	CameraActor(class Game* game);

	void LoadProperties(const rapidjson::Value& inObj) override;

	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& state) override;

	// �߼Ҹ�
	void SetFootstepSurface(float value);

private:
	class MoveComponent* mMoveComp;

	// ����
	class AudioComponent* mAudioComp;
	class SoundEvent mFootstep;
	float mLastFootstep;
};
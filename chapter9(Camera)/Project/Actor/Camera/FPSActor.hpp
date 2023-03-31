class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& state) override;

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class Actor* mFPSModel;
	SoundEvent mFootstep;
	float mLastFootstep;
};
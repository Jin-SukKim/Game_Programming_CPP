class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& state) override;

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

	// �浹����
	void FixCollisions();

	void Shoot();

	void LoadProperties(const rapidjson::Value& inObj) override;
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Actor* mFPSModel;
	SoundEvent mFootstep;
	float mLastFootstep;
};
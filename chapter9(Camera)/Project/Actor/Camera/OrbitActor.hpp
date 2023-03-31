class OrbitActor : public Actor
{
public:
	OrbitActor(class Game* game);

	void ActorInput(const struct InputState& state) override;
	void SetVisible(bool visible);

private:
	class OrbitCamera* mCameraComp;
	class MeshComponent* mMeshComp;
};
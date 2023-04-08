class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);

	void ActorInput(const struct InputState& state) override;
	void SetVisible(bool visible);
private:
	class MoveComponent* mMoveComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	bool mMoving;
};
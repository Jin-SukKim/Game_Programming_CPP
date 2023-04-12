class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);

	void ActorInput(const struct InputState& state) override;
	void SetVisible(bool visible);

	TypeID GetType() const override { return TFollowActor; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;
private:
	class MoveComponent* mMoveComp;
	class FollowCamera* mCameraComp;
	class SkeletalMeshComponent* mMeshComp;
	bool mMoving;
};
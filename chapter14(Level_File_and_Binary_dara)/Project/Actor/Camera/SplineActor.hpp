class SplineActor : public Actor
{
public:
	SplineActor(class Game* game);

	void ActorInput(const struct InputState& state) override;
	void RestartSpline();

	void LoadProperties(const rapidjson::Value& inObj) override;
private:
	class SplineCamera* mCameraComp;
	bool pause;
};
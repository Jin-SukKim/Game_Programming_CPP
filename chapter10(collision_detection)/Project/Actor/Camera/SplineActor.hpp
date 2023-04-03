class SplineActor : public Actor
{
public:
	SplineActor(class Game* game);

	void ActorInput(const struct InputState& state) override;
	void RestartSpline();

private:
	class SplineCamera* mCameraComp;
	bool pause;
};
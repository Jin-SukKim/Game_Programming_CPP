#include "PreCompiled.hpp"

SplineActor::SplineActor(Game* game) : Actor(game)
{
	mCameraComp = new SplineCamera(this);

	// 스플라인 생성
	Spline path;
	path.mControlPoints.emplace_back(Vector3d::Zero);
	for (int i = 0; i < 5; i++)
	{
		if (i % 2 == 0)
		{
			path.mControlPoints.emplace_back(Vector3d(300.f * (i + 1), 300.f, 300.f));
		}
		else
		{
			path.mControlPoints.emplace_back(Vector3d(300.f * (i + 1), 0.f, 0.f));
		}
	}

	mCameraComp->SetSpline(path);
	pause = false;
	mCameraComp->SetPaused(pause);
}

void SplineActor::ActorInput(const InputState& state) {
	if (state.Keyboard.GetKeyState(SDL_SCANCODE_SPACE) == EPressed)
	{
		mCameraComp->SetPaused(!pause);
	}
}

void SplineActor::RestartSpline()
{
	mCameraComp->Restart();
}
#include "PreCompiled.hpp"

HUD::HUD(Game* game) : UIScreen(game), mRadarRange(2000.f), mRadarRadius(92.f)
, mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBar = r->GetTexture("Assets/HealthBar.png");
	mRadar = r->GetTexture("Assets/Radar.png");
	mCrosshair = r->GetTexture("Assets/Crosshair.png");
	mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	mBlipTex = r->GetTexture("Assets/Blip.png");
	mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD() {}

void HUD::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader)
{
	// Crosshair
	Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
	DrawTexture(shader, cross, Vector2d::Zero, 2.f);

	// Rader
	const Vector2d cRadarpos(-390.f, 275.f);
	DrawTexture(shader, mRadar, cRadarpos, 1.f);

	// Blips(신호들)
	for (Vector2d& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarpos + blip, 1.f);
	}
	// rader 화살표
	DrawTexture(shader, mRadarArrow, cRadarpos);
}

void HUD::AddTargetComponent(TargetComponent* tc)
{
	mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc)
{
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(), tc);
	mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(float deltaTime)
{
	// 일반 커서로 리셋
	mTargetEnemy = false;
	// 선분을 만든다.
	const float cAimDist = 5000.f;
	Vector3d start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);

	// 선분 캐스트
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// SegmentCast를 통해 얻는 액터가 타깃 컴포넌트의 소유자와 일치하는지 체크
		for (auto tc : mTargetComps)
		{
			if (tc->GetOwner() == info.mActor)
			{
				mTargetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateRadar(float deltaTime)
{
	// 신호 오프셋 벡터 클리어
	mBlips.clear();

	// 플레이어 위치를 레이더 좌표로 변환(x축은 전방 벡터, z축은 상향 벡터)
	Vector3d playerPos = mGame->GetPlayer()->GetPosition();
	Vector2d playerPos2D(playerPos.y, playerPos.x);
	// 레이더의 전방은 플레이어의 전방과 같다
	Vector3d playerForward = mGame->GetPlayer()->GetForward();
	Vector2d playerForward2D(playerForward.x, playerForward.y);

	// 레이더를 회전시키기 위해 atan2 함수 사용
	float angle = MathUtils::Atan2(playerForward2D.y, playerForward2D.x);
	// 2D 회전 행렬을 만든다.
	Matrix3x3 rotMat = Matrix3x3::CreateRotation(angle);

	// 오브젝트 신호의 위치를 얻는다.
	for (auto tc : mTargetComps)
	{
		Vector3d targetPos = tc->GetOwner()->GetPosition();
		Vector2d actorPos2D(targetPos.y, targetPos.x);

		// 플레이어와 타깃 사이의 벡터를 계산
		Vector2d playerToTarget = actorPos2D - playerPos2D;

		// 타깃이 범위 안에 있는지 확인
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			// playerToTarget 좌표를 레이더 화면 중심으로부터의 오프셋으로 변환
			Vector2d blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			// 신호를 회전시켜 레이더 공간의 최종 좌표로 변환
			blipPos = Vector2d::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}

}
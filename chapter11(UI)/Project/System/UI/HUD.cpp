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

	// Blips(��ȣ��)
	for (Vector2d& blip : mBlips)
	{
		DrawTexture(shader, mBlipTex, cRadarpos + blip, 1.f);
	}
	// rader ȭ��ǥ
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
	// �Ϲ� Ŀ���� ����
	mTargetEnemy = false;
	// ������ �����.
	const float cAimDist = 5000.f;
	Vector3d start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);

	// ���� ĳ��Ʈ
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info))
	{
		// SegmentCast�� ���� ��� ���Ͱ� Ÿ�� ������Ʈ�� �����ڿ� ��ġ�ϴ��� üũ
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
	// ��ȣ ������ ���� Ŭ����
	mBlips.clear();

	// �÷��̾� ��ġ�� ���̴� ��ǥ�� ��ȯ(x���� ���� ����, z���� ���� ����)
	Vector3d playerPos = mGame->GetPlayer()->GetPosition();
	Vector2d playerPos2D(playerPos.y, playerPos.x);
	// ���̴��� ������ �÷��̾��� ����� ����
	Vector3d playerForward = mGame->GetPlayer()->GetForward();
	Vector2d playerForward2D(playerForward.x, playerForward.y);

	// ���̴��� ȸ����Ű�� ���� atan2 �Լ� ���
	float angle = MathUtils::Atan2(playerForward2D.y, playerForward2D.x);
	// 2D ȸ�� ����� �����.
	Matrix3x3 rotMat = Matrix3x3::CreateRotation(angle);

	// ������Ʈ ��ȣ�� ��ġ�� ��´�.
	for (auto tc : mTargetComps)
	{
		Vector3d targetPos = tc->GetOwner()->GetPosition();
		Vector2d actorPos2D(targetPos.y, targetPos.x);

		// �÷��̾�� Ÿ�� ������ ���͸� ���
		Vector2d playerToTarget = actorPos2D - playerPos2D;

		// Ÿ���� ���� �ȿ� �ִ��� Ȯ��
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange))
		{
			// playerToTarget ��ǥ�� ���̴� ȭ�� �߽����κ����� ���������� ��ȯ
			Vector2d blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;

			// ��ȣ�� ȸ������ ���̴� ������ ���� ��ǥ�� ��ȯ
			blipPos = Vector2d::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}

}
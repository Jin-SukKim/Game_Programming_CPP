#include "PreCompiled.hpp"

DialogBox::DialogBox(Game* game, const std::string& text, std::function<void()> onOK)
	:UIScreen(game)
{
	// ���̾�α� �ڽ� ��ġ�� ����
	mBGPos = Vector2d(0.f, 0.f);
	mTitlePos = Vector2d(0.f, 100.f);
	mNextButtonPos = Vector2d(0.f, 0.f);
	// ��� �ؽ�ó ����
	mBackground = mGame->GetRenderer()->GetTexture("Assets/DialogBG.png");
	SetTitle(text, Vector3d::Zero, 30);
	// ��ư �߰�
	AddButton("OKButton", [onOK]() { onOK(); });
	AddButton("CancelButton", [this]() { Close(); });
}

DialogBox::~DialogBox() {}
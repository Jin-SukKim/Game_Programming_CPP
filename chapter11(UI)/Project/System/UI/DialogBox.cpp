#include "PreCompiled.hpp"

DialogBox::DialogBox(Game* game, const std::string& text, std::function<void()> onOK)
	:UIScreen(game)
{
	// 다이얼로그 박스 위치를 조정
	mBGPos = Vector2d(0.f, 0.f);
	mTitlePos = Vector2d(0.f, 100.f);
	mNextButtonPos = Vector2d(0.f, 0.f);
	// 배경 텍스처 설정
	mBackground = mGame->GetRenderer()->GetTexture("Assets/DialogBG.png");
	SetTitle(text, Vector3d::Zero, 30);
	// 버튼 추가
	AddButton("OKButton", [onOK]() { onOK(); });
	AddButton("CancelButton", [this]() { Close(); });
}

DialogBox::~DialogBox() {}
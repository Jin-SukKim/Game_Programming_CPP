#include "PreCompiled.hpp"

PauseMenu::PauseMenu(Game* game) : UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	// 제목 텍스트 설정
	SetTitle("PauseTitle");
	AddButton("ResumeButton", [this]() { Close(); });
	AddButton("QuitButton", [this]() {
		new DialogBox(mGame, "QuitText",
		[this]() {
				mGame->SetState(Game::EQuit);
			});
	});
		
}


// 게임 플레이로 돌아가도록 EGameplay로 지정
PauseMenu::~PauseMenu()
{
	SetRelativeMouseMode(true);
	mGame->SetState(Game::EGameplay);
}

void PauseMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}
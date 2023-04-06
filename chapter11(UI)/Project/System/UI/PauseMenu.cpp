#include "PreCompiled.hpp"

PauseMenu::PauseMenu(Game* game) : UIScreen(game)
{
	mGame->SetState(Game::EPaused);
	SetRelativeMouseMode(false);
	// ���� �ؽ�Ʈ ����
	SetTitle("PauseTitle");
	AddButton("ResumeButton", [this]() { Close(); });
	AddButton("QuitButton", [this]() {
		new DialogBox(mGame, "QuitText",
		[this]() {
				mGame->SetState(Game::EQuit);
			});
	});
		
}


// ���� �÷��̷� ���ư����� EGameplay�� ����
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
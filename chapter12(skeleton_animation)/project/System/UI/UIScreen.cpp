#include "..\..\PreCompiled.hpp"

UIScreen::UIScreen(Game* game)
	:mGame(game)
	, mTitle(nullptr)
	, mBackground(nullptr)
	, mTitlePos(0.0f, 300.0f)
	, mNextButtonPos(0.0f, 200.0f)
	, mBGPos(0.0f, 250.0f)
	, mState(EActive)
{
	// UI stack에 추가
	mGame->PushUI(this);
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mButtonOn = mGame->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
	mButtonOff = mGame->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

UIScreen::~UIScreen()
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}

	
	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
	
}

void UIScreen::Update(float deltaTime) {}

void UIScreen::Draw(Shader* shader)
{
	// 배경이 있으면 그려준다
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// 타이틀 그리기
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}

	// 버튼 그리기
	for (auto b : mButtons)
	{
		// 버튼의 배경 텍스처를 그린다.
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());

		// 버튼의 텍스트를 그린다.
		DrawTexture(shader, b->GetNameTex(), b->GetPosition());
	}
}

void UIScreen::ProcessInput(const struct InputState& state)
{
	// 버튼을 가졌는지 확인
	if (!mButtons.empty())
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		// 화면 중심이 (0, 0)인 좌표로 변환 (1024x768 해상도 가정)
		Vector2d mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

		// 버튼 하이라이트
		for (auto b : mButtons)
		{
			if (b->ContainsPoint(mousePos))
			{
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}
		}
	}
}

void UIScreen::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty())
		{
			for (auto b : mButtons)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

void UIScreen::Close()
{
	mState = EClosing;
}

void UIScreen::SetTitle(const std::string& text,
	const Vector3d& color, int pointSize)
{
	// 이전 타이틀 초기화
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}

	mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Vector2d dims(static_cast<float>(mButtonOn->GetWidth()),
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims);
	mButtons.emplace_back(b);

	// 다음 버튼의 위치를 갱신한다.
	// 버튼의 높이값으로 위칙밧을 감소시킨 후 패딩값을 더한다.
	mNextButtonPos.y -= mButtonOff->GetHeight() + 20.f;

}

void UIScreen::DrawTexture(Shader* shader, Texture* texture,
	const Vector2d& offset, float scale)
{
	Vector2d off = offset;
	// 텍스처 크기 행렬
	Matrix4x4 scaleMat = Matrix4x4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		static_cast<float>(texture->GetHeight()) * scale,
		1.0f);

	// 이동 행렬
	Matrix4x4 transMat = Matrix4x4::CreateTranslation(
		Vector3d(offset.x, offset.y, 0.0f));
	// 세계 변환
	Matrix4x4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// 현재 텍스처
	texture->SetActive();
	// 그리기
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// 초기화
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}
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
	// UI stack�� �߰�
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
	// ����� ������ �׷��ش�
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// Ÿ��Ʋ �׸���
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}

	// ��ư �׸���
	for (auto b : mButtons)
	{
		// ��ư�� ��� �ؽ�ó�� �׸���.
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());

		// ��ư�� �ؽ�Ʈ�� �׸���.
		DrawTexture(shader, b->GetNameTex(), b->GetPosition());
	}
}

void UIScreen::ProcessInput(const struct InputState& state)
{
	// ��ư�� �������� Ȯ��
	if (!mButtons.empty())
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		// ȭ�� �߽��� (0, 0)�� ��ǥ�� ��ȯ (1024x768 �ػ� ����)
		Vector2d mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

		// ��ư ���̶���Ʈ
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
	// ���� Ÿ��Ʋ �ʱ�ȭ
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

	// ���� ��ư�� ��ġ�� �����Ѵ�.
	// ��ư�� ���̰����� ��Ģ���� ���ҽ�Ų �� �е����� ���Ѵ�.
	mNextButtonPos.y -= mButtonOff->GetHeight() + 20.f;

}

void UIScreen::DrawTexture(Shader* shader, Texture* texture,
	const Vector2d& offset, float scale)
{
	Vector2d off = offset;
	// �ؽ�ó ũ�� ���
	Matrix4x4 scaleMat = Matrix4x4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		static_cast<float>(texture->GetHeight()) * scale,
		1.0f);

	// �̵� ���
	Matrix4x4 transMat = Matrix4x4::CreateTranslation(
		Vector3d(offset.x, offset.y, 0.0f));
	// ���� ��ȯ
	Matrix4x4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// ���� �ؽ�ó
	texture->SetActive();
	// �׸���
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// �ʱ�ȭ
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}
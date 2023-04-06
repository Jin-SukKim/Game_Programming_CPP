#include "PreCompiled.hpp"

Button::Button(const std::string& name, Font* font,
	std::function<void()> onClick, const Vector2d& pos, const Vector2d& dims)
	:mOnClick(onClick), mNameTex(nullptr), mFont(font), mPosition(pos)
	, mDimensions(dims), mHighlighted(false)
{
	SetName(name);
}

Button::~Button()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}

void Button::SetName(const std::string& name)
{
	mName = name;

	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
		mNameTex = nullptr;
	}
	mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2d pt) const
{
	bool no =
		pt.x < (mPosition.x - mDimensions.x / 2.f) ||
		pt.x > (mPosition.x + mDimensions.x / 2.f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.f) ||
		pt.y > (mPosition.y + mDimensions.y / 2.f);
	return !no;
}

void Button::OnClick()
{
	// mOnLick 핸들러가 존재하면 이 핸들러를 호출한다.
	if (mOnClick)
	{
		mOnClick();
	}
}
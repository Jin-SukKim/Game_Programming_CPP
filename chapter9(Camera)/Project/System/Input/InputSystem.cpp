#include "..\PreCompiled.hpp"

bool InputSystem::Initialize() 
{
	// Ű����
	// ���� ������ �����͸� �����Ѵ�.
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// ���� ���� �޸𸮸� 0���� �ʱ�ȭ�Ѵ�.
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

	// ���콺
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// ��Ʈ�ѷ� 
	// ��Ʈ�ѷ� �Ѱ� �����ϱ�
	mController = SDL_GameControllerOpen(0);
	// ��Ʈ�ѷ� ������� Ȯ��
	mState.Controller.mIsConnected = (mController != nullptr);
	// �޸𸮰� �ʱ�ȭ
	memset(mState.Controller.mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Shutdown() {}

void InputSystem::PrepareForUpdate()
{
	// ���� �����͸� ���� ���۷� �����Ѵ�.
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);
	

	// ���� ��ư ���¸� ���� ���¿� �����Ѵ�.
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	// ��� ���콺 ���
	mState.Mouse.mIsRelative = false;
	// ���콺 �� ����
	mState.Mouse.mScrollWheel = Vector2d::Zero;

	// ���� ��Ʈ�ѷ� ��ư ���¸� ���� ��ư ���·� ����
	memcpy(mState.Controller.mPrevButtons,
		mState.Controller.mCurrButtons,
		SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update()
{
	// ���콺 ��� ����
	int x = 0, y = 0;
	// ���콺 ��ǥ

	// ������� ���콺 ��ġ
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons =
			SDL_GetRelativeMouseState(&x, &y);
	}
	// ���� ���콺 ��ġ
	else
	{
		mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
	}
	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// ���� ��Ʈ�ѷ� ��ư �ݺ��ϸ� ���� ��ȸ
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController,
				SDL_GameControllerButton(i));
	}

	// Ʈ���� �� �б�
	mState.Controller.mLeftTrigger = Filter1D(SDL_GameControllerGetAxis(
		mController, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger = Filter1D(SDL_GameControllerGetAxis(
		mController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

	// �Ƴ��α� ��ƽ �� ���
	x = SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_LEFTX);
	// y�� ���� ���� (SDL�� +y�� �Ʒ��� ������ ���� ��ǥ��� �ݴ�)
	y = -SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_LEFTY);
	mState.Controller.mLeftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_RIGHTX);
	// y�� ���� ���� (SDL�� +y�� �Ʒ��� ������ ���� ��ǥ��� �ݴ�)
	y = -SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_RIGHTY);
	mState.Controller.mLeftStick = Filter2D(x, y);
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2d(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}
}

float InputSystem::Filter1D(int input)
{
	// ���� ���� ������ ������ 0%�� �ؼ��Ѵ�.
	const int deadZone = 250;
	// ���� �ִ񰪺��� ũ�� 100%�� �ؼ��Ѵ�.
	const int maxValue =  30000;

	float retVal = 0.f;

	// �Է��� ������ ��´�.
	int absValue = input > 0 ? input : -input;
	// ���� �� ���� �Է°��� �����Ѵ�.
	if (absValue > deadZone)
	{
		// ���� ���� �ִ� ������ �м����� ���
		retVal = static_cast<float>(absValue - deadZone) /
			(maxValue - deadZone);
		
		// �������� ��ȣȭ ��ġ��Ų��.
		retVal = input > 0 ? retVal : -1.f * retVal;

		// ���� -1.f�� 1.f ������ ����� �ʰ� �Ѵ�
		retVal = MathUtils::Clamp(retVal, -1.f, 1.f);
	}

	return retVal;
}

Vector2d InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.f;
	const float maxValue = 30000.f;

	// 2D ���͸� �����.
	Vector2d dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// ���̰� ���� ������ �۴ٸ� �Է� �������� ó��
	if (length < deadZone)
	{
		dir = Vector2d::Zero;
	}
	else
	{
		// ���� ���� �ִ� ���ɿ� ������ �м����� ���
		float f = (length - deadZone) / (maxValue - deadZone);
		// f ���� 0.f�� 1.f ���̷� ����
		f = MathUtils::Clamp(f, 0.f, 1.f);
		// ���͸� ����ȭ�ѵ� �м������� ���͸� ������
		dir *= f / length;
	}

	return dir;
}
#include "..\PreCompiled.hpp"

bool InputSystem::Initialize() 
{
	// 키보드
	// 현재 상태의 포인터를 설정한다.
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// 이전 상태 메모리를 0으로 초기화한다.
	memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

	// 마우스
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// 컨트롤러 
	// 컨트롤러 한개 연결하기
	mController = SDL_GameControllerOpen(0);
	// 컨트롤러 연결상태 확인
	mState.Controller.mIsConnected = (mController != nullptr);
	// 메모리값 초기화
	memset(mState.Controller.mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
	memset(mState.Controller.mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

	return true;
}

void InputSystem::Shutdown() {}

void InputSystem::PrepareForUpdate()
{
	// 현재 데이터를 이전 버퍼로 복사한다.
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);
	

	// 현재 버튼 상태를 이전 상태에 복사한다.
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	// 상대 마우스 모드
	mState.Mouse.mIsRelative = false;
	// 마우스 휠 리셋
	mState.Mouse.mScrollWheel = Vector2d::Zero;

	// 현재 컨트롤러 버튼 상태를 이전 버튼 상태로 복사
	memcpy(mState.Controller.mPrevButtons,
		mState.Controller.mCurrButtons,
		SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update()
{
	// 마우스 멤버 갱신
	int x = 0, y = 0;
	// 마우스 좌표

	// 상대적인 마우스 위치
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons =
			SDL_GetRelativeMouseState(&x, &y);
	}
	// 현재 마우스 위치
	else
	{
		mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
	}
	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// 현재 컨트롤러 버튼 반복하며 상태 조회
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
	{
		mState.Controller.mCurrButtons[i] =
			SDL_GameControllerGetButton(mController,
				SDL_GameControllerButton(i));
	}

	// 트리거 값 읽기
	mState.Controller.mLeftTrigger = Filter1D(SDL_GameControllerGetAxis(
		mController, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	mState.Controller.mRightTrigger = Filter1D(SDL_GameControllerGetAxis(
		mController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

	// 아날로그 스틱 값 얻기
	x = SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_LEFTX);
	// y축 값을 반전 (SDL이 +y가 아래쪽 하지만 게임 좌표계는 반대)
	y = -SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_LEFTY);
	mState.Controller.mLeftStick = Filter2D(x, y);

	x = SDL_GameControllerGetAxis(mController,
		SDL_CONTROLLER_AXIS_RIGHTX);
	// y축 값을 반전 (SDL이 +y가 아래쪽 하지만 게임 좌표계는 반대)
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
	// 값이 데드 존보다 작으면 0%로 해석한다.
	const int deadZone = 250;
	// 값이 최댓값보다 크면 100%로 해석한다.
	const int maxValue =  30000;

	float retVal = 0.f;

	// 입력의 절댓값을 얻는다.
	int absValue = input > 0 ? input : -input;
	// 데드 존 내의 입력값은 무시한다.
	if (absValue > deadZone)
	{
		// 데드 존과 최댓값 사이의 분숫값을 계산
		retVal = static_cast<float>(absValue - deadZone) /
			(maxValue - deadZone);
		
		// 원래값의 부호화 일치시킨다.
		retVal = input > 0 ? retVal : -1.f * retVal;

		// 값이 -1.f와 1.f 범위를 벗어나지 않게 한다
		retVal = MathUtils::Clamp(retVal, -1.f, 1.f);
	}

	return retVal;
}

Vector2d InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.f;
	const float maxValue = 30000.f;

	// 2D 벡터를 만든다.
	Vector2d dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// 길이가 데드 존보다 작다면 입력 없음으로 처리
	if (length < deadZone)
	{
		dir = Vector2d::Zero;
	}
	else
	{
		// 데드 존과 최댓값 동심원 사이의 분수값을 계산
		float f = (length - deadZone) / (maxValue - deadZone);
		// f 값을 0.f와 1.f 사이로 한정
		f = MathUtils::Clamp(f, 0.f, 1.f);
		// 벡터를 정규화한뒤 분수값으로 벡터를 스케일
		dir *= f / length;
	}

	return dir;
}
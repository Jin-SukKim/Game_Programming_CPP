/*
    오디오 시스템 생성

    오디오를 다루는 클래스를 따로 만들어 줘 FMOD API 호출을 담당하도록 한다.

    멤버 변수는 저수준 API 시스템뿐만 아닌 FMOD 스튜디오 시스템에 대한 포인터를 포함한다.
    대부분 FMOD 스튜디오를 사용하지만 low-level도 포함한다.

    fmod_studio.hpp 헤더파일은 FMOD 스튜디오 API 타입을 정의한다.
    하지만 AudioSystem.hpp 파일에 포함시키지 않기 위해 FMOD 타입을 전방 선언한다.
    
    FMOD 초기화

    1. 에러에 대한 로깅 설정을 위해 Debug_Initialize 함수 호출
        - 첫번째 파라미터는 로깅 메시지의 로깅 레벨 수준을 제어한다.
        - 두번째 파라미터는 로그 메시지를 쓸 위치를 지정한다.
        커스텀 디버그 코드가 있는 게임에서는 로그 메시지 출력을 커스텀 콜백 함수에서
        처리하도록 선언하는 것도 ㄱ능하다.

    FMOD::Studio::System::create(&mSystem); 으로 FMOD 시스템의 인스턴스를 생성한다.
    return 값으로 FMOD_RESULT를 받는다.
    
    FMOD 함수는 결과값을 항상 반환해서 호출자에세 FMOD 함수 호출 결과를 알려준다.

    FMOD 시스템 생성 후 FMOD 시스템의 initialize 함수를 호출한다.
    // Initialize FMOD system
	result = mSystem->initialize(
		512,		// 동시에 출력할 수 있는 사운드의 최대 갯수
		FMOD_STUDIO_INIT_NORMAL,	// 기본 설정 사용
		FMOD_INIT_NORMAL,			// 기본 설정
		nullptr						// 대부분 nullptr
	);
    첫번째 파라미터는 최대 채널수를 지정하고 다음 두 파라미터는 
    FMOD 스튜디오 FMOD 저수준 리벨 API의 동작을 조정한다.
    추가적인 드라이버 데이터를 사용하려는 경우 마지막 파라미터를 사용할 수 있지만,
    일반적으로는 사용되지 않기에 nullptr로 지정했따.

    초기화 완료 후 저수준 시스템 포인터를 얻어서 저장한다.

    AudioSystem의 Shutdown은 mSystem->release()함수를 호출하고,
    Update 함수는 mSystem->update()를 호출한다. (mSystem = FMOD 시스템)

    FMOD는 프레임마다 한번씩 update 함수 호출을 요구한다.
    이 함수는 3D 오디오 계산 갱신과 같은 작업을 수행한다.
    
*/
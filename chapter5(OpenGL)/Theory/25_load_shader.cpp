/*
    셰이더 로딩(shader 클래스)

    셰이더 파일 작성 후 C++ 코드에서 셰이더를 로드해 OpenGL이 셰이더를
    인식할 수 있도록 해준다.

    고수준 레벨에서의 수행 단계

    1. vertex shader를 로드하고 컴파일한다.
    2. fragment shader를 로드하고 컴파일한다.
    3. 2개의 셰이더를 'shader program'에 서로 연결시킨다.

    셰이더를 로드하는 단계

    3개의 helper 함수를 private에 선언했다.
    
    CompileShader()는 3개의 파라미터를 전달 받는다
    
    1. 컴파일할 셰이더 파일의 이름
    2. 셰이더 타입
    3. 셰이더의 ID를 저장할 참조 파라미터.

    return 값은 함수 호출이 성공했는지 아닌지를 나타내는 bool이더ㅏ.

    구현

    1. 파일 로드를 위해 ifstream을 생성
    2. 문자열 스트림을 사용해 파일의 전체 내용을 단일 문자열로 로드하고 
       c_str 함수를 사용해 c스타일 문자열 포인터를 얻는다.
    3. glCreateShader 함수 호출로 셰이더에 해당하는 OpenGL 셰이더 오브젝트 생성
    4. ID를 outShader에 저장.
        shaderType 파라미터는 vertex shader, fragment shader 또는 다른 셰이더 타입일 수도있다.
    5. glShaderSource 함수 호출해 셰이더 소스 코드를 포함한 스트링을 지정
    6. glCompileShader 함수로 코드 컴파일
    7. IsCompiled 헬퍼 함수로 유효성 체크

    IsCompiled 함수

    셰이더 오브젝트가 컴파일됐는지 확인한다.
    컴파일되지 못하면에러 메시지 출력.

    glGetShaderiv 함수는 셰이더 캄파일 상태를 질의하며 함수는 정수값으로 상태 코드를 반환한다.
    GL_TRUE가 아니면 에러가 있다는 의미다.
    glGetShaderInfoLog로 컴파일 에러 메시지를 얻는다.

    Load() 함수

    vertex와 fragment shader의 파일 이름을 인자로받은 뒤 이 셰이더들을 컴파일하고 서로 연결시킨다.

    CompileShader를 사용해 vertext/fragment 셰이더를 컴파일 한 뒤 셰이더 오브젝트 ID를 저장한다.
    실패하면 false를 return한다.
    셰이더들을 컴파일하고 난 후 셰이더 프로그램(glCreateProgram)이란 오브젝트(glAttachShader)르 사용해 두 셰이더를 연결(glLinkProgram)한다.
    
    오브젝트가 그릴 준비가 되면 OpenGL은 삼각형을 렌더링하고자 현재 활성화된 셰이더 프로그램을 사용한다.
    
    IsValidProgram 함수

    IsCompiled 코드와 매우 유사하다.
    단 glGetShaderiv를 호출하는  것 대신 glGetProgramiv를 호출한다.
    그리고 glGetShaderInfoLog 대신 glGetprogramInfoLog를 호출한다.

*/
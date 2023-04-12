/*
    전역 속성 로딩(global attibutes loading)

    게임 세계에 존재하는 유일한 전역 속성은 광원 속성이다.
    주변광이나 전역 방향광이 이에 해당한다.(속성 수가 적고 제한적이다)

    // 전역 광원 속성을 기술한 레벨(Level0.gplevel)
    {
        "version":1,
        "globalProperties": {
            "ambientLight": [0.2, 0.2, 0.2],
            "directionalLight": {
                "direction": [0.0, -0.707, -0.707],
                "color": [0.78, 0.88, 1.0]
            }
        }
    }

    레벨 파일의 핵심 부분인 JSON Document는 JSON 오브젝트인 키/값 속성(또는 쌍)의 딕셔너리다.
    키 이름은 ""로 묶고 콜론(:) 뒤에 값(여러 타입 가능)을 쓴다.
    기본 타입은 문자열, 숫자, 이진값이다.
    복합 타입은 배열과 JSON 오브젝트가 있따.

    위 파일의 경우 globalProperties 값은 JSON 오브젝트에 해당한다.
    이 오브젝트는 2개의 키를 가지고 있다.(주변광요 키, 방향광용 키)
    ambiendtLight는 3개의 숫자로 구성된 배열에 해당한다.

    JSON 오브젝트와 속성의 중첩된 형태는 parsing 코드를 구현하는 데 아이디어를 준다.
    특히 오브젝트와 키 이름이 주어지면 이에 해당하는 값을 읽는 일반적인 함수 작성이 가능하다.

    이 전역 속성을 parsing 하기 위해 LevelLoader 클래스를 생성한다.
    파일에서 레벨을 로딩하면 게임 상태에 영향을 미치지만, 레벨 로더 그 자체는 영향이
    없으므로 LoadLevel 함수는 정적 함수로 선언한다.

    class LevelLoader
    {
    public:
        // 레벨을 로드한다(Game 객체는 오브젝트를 생성하거나 수정할 때 접근해야 하므로 필요하다.)
        static bool LoadLevel(class Game* game, const std::string& fileName);
    };

    LoadLevel 함수에서 수행하는 첫 단계는 레벨 파일을 rapidjson::Document로 로드해 구문 분석한다.
    먼저 전체 파일을 메모리에 로드하고 난 뒤 이 버퍼를 Document의 parsing 멤버 함수로 전달한다.
    JSON 파일을 하나의 document에 로딩하는 것은 일반적인 형태이므로 헬퍼 함수를 만들면 좋다.
    헬퍼 함수를 구현하며 JSON 파일 포맷인 gpmesh, gpanim 및 여러 JSON 에셋 타입을 로드할 때에도
    이 함수를 재사용하는 것이 가능해진다.

    정적 함수로 LoadJSON 함수를 구현한다.
    LoadJSON 함수는 파일 이름과 Document의 찹조값을 파라미터로 받는다.
    파일을 읽어들여서 Document 객체로 parsing한다.

    JSON 오브젝트를 얻었으면 키를 읽은 뒤 키에 해당하는 값을 추출해야 된다.
    주어진 키가 항상 JSON 오브젝트에 존재할 거라고 추정해서는 안 되기 때문에
    우선 키가 존재하는지 그리고 예상되는 타입인지를 검증해야 한다.
    검증 됐다면 그때 값을 읽는다.

    JsonHelper 클래스를 생성한다.
    GetInt 함수를 만드는데 이 함수는 속성을 찾은 뒤 그 속성이 기댓값의 타입과
    일치하는지 검증한 다음 문제없으면 true를 반환한다.(GetInt는 정수타입인지 확인)

    그 후 LoadLevel에서 GetInt 함수를 사용해 로드한 파일의 버전이 기대값과 동일한지 검증한다.

    여기서 사용한 JSON 오브젝트는 전체 Document(루트 JSON 오브젝트)다.
    먼저 GetInt 함수가 값을 제대로 반환하는지 확인하고, 그럴 경우 값이 기대값(LevelVersion 상수)과
    일치하는지 확인한다.

    다른 기본 타입의 값을 추출하기 위해 JsonHelper에 GetFloat, GetBool, GetString 등
    유사한 함수 추가도 해준다.
    하지만 값 추출은 비기본 타입도 지원할 때 보다 강력해진다.
    특히 Vector3d 타입(ambientLight 같은)은 매우 많이 쓰여서 GetVector3 함수를 제작해두면 매우 유용하다.
    GetVector3 함수의 전반적인 구조는 속성이 float인 3개의 멤버를 가진 배열인지를
    검증하는 부분을 제외하고는 동일하다.
    비슷하게 GetQuaternion 함수를 선언하는 것도 가능하다.

    주변광과 방향광

    헬퍼 함수를 사용해 전역 속성을 로드하는 함수를 만든다.
    전역 속성은 여러 가지가 존재할 수 있고 어떤 속성은 게임에서 필요없을 수 있기에
    헬퍼 함수는 개발자가 필요로 하는 특정 속성을 수동으로 로드 가능하도록 구현한다.

    LoadGlobalProperies 함수는 주변광과 방향광 속성을 로드하는 방법을 보여준다.
    이 함수 구현 대부분은 이 속성들을 로드하기 위해 제작한 헬퍼함수의 호출이 주를 이룬다.

    연산자 []를 사용해 rapidjson::Value& 속성에 직접 접근한다.
    dirObj["directionalLight"]은 directionalLight 키를 사용해 값을 얻는다.
    그리고 IsObject() 호출을 통해 값의 타입이 JSON 오브젝트인지 확인한다.

    방향광 관련 또다른 패턴은 값을 설정하려는 변수에 직접 접근하는 부분이다.
    여기서는 GetVector3 호출 시 상태 체크 코드를 추가하지 않아도 된다.
    요청된 속성이 존재하지 않으면 Get함수는 변수값을 변경하지 않기 때문이다.


    LoadLevel의 레벨 파일 버전 유효성 점증 코드 다음에 LoadGlobalProperties 호출을 추가한다.
    그리고 Game::LoadData에 Level0.gplevel 파일을 로드하는 LoadLevel 호출을 추가한다.

    이제 레벨 파일에서 광원 속성을 로딩하므로 LoadData에 하드코딩된 주변광고 방향광 코드를 제거해도된다.

*/
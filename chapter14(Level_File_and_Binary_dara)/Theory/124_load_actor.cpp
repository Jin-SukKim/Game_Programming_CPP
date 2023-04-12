/*
    액터 로딩

    Level1.gplevel이 액터 정보를 가지고 있는 레벨 파일이다.

    액터를 로딩할 때는 JSON 파일이 액터 데이터가 배열로 구성됐으며, 각각의 액터는
    해당 액터의 속성 정보가 있다고 가정한다.
    액터는 서브클래스이기에 액터를 로딩하려면 액터의 타입을 지정하기 위한 방법이 필요하다.
    또한 레벨 로딩 코드에서는 할당하려는 액터의 서브클래스를 결정하기 위한
    긴 조건 체크 코드를 가능한 피하는게 좋다.

    Level1.gplevel에서는 TargetActor타입의 액터만 보여주지만, 타입은 다른 모든
    액터 서브클래스도쉽게 지정 가능하다.
    또한 액터에는 타입 외에도 해당 액터에 지정해야 할 여러 속성이 있다.
    여기서는 속성 세트가 위치와 회전뿐이지만 액터가 가지는 다른 속성도 가능하다.

    특정 타입의 액터를 생성할 수 있는 함수가 존재하고 액터에 대한 속성을 로드해야 할 때
    가장 간단한 접근법은 기본 Actor 클래스에 가상 함수 LoadProperties를 만드는 것이다.

    그 후 Actor의 서브클래스에 필요에 따라 추가 속성을 로드하는 LoadProperties함수를 재정의한다.

    void SomeActor::LoadProperties(const rapidjson::Value& inObj)
    {
        // 액터의 기본 속성을 로드한다.
        Actor::LoadProperties(inObj);
        // 커스텀 속성들을 로드한다.
    }

    이제 속성을 로드하는것이 가능하다.
    다음은 올바른 타입의 액터를 생성하는 것이다.
    한가지 방법은 키가 액터 타입의 문자열 이름이고 값은 해당 타입의 액터를 동적으로 할당하는
    함수로 구성된 맵을 정의한다.

    Actor 서브클래스의 각각에 대한 별도의 함수 선언을 피하기 위해 기본 Actor 클래스에 다음과
    같은 템플릿 함수를 만든다.

    template <typename T>
    static Actor* Create(class Game* game, const rapidjson::Value& inObj)
    {
        // 타입 T인 액터를 동적으로 할당
        T* t = new T(game);
        // 새로운 액터의 LoadProperties 호출(액터 타입에 따른 추가 파라미터를 로드하기 위함)
        t->LoadProperties(inObj);
        return t;
    }

    이제 LevelLoader로 돌아와 맵을 만들어야 한다
    키의 타입은 std::string이고 값은 Actor::Create 함수와 선언이 일치하는 함수가 필요하다.
    std::function 헬퍼 클래스를 다시 사용해 함수 선언을 정의한다.

    별칭 선언(alias declation, typedef와 유사)을 해서 ActorFunc 타입 지정자를 만든다.

    using ActorFunc = std::function<class Actor*(class Game*, const rapidjson::Value&)>;

    std::function의 템플릿 파라미터는 함수가 Actor*를 반환하고 2개의 파라미터 Game*와rapidjson::Value&를
    받도록 지정한다.

    그 후 LevelLoader에는 정적 변수인맵을 선언한다.

    static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;

    그리고 LevelLoader.cpp에서는 생성할 여러 액터로 구성된 sActorFactoryMap을 완성한다.

    std::unordered_map<std::string, ActorFunc> LevelLoader::sActorFactoryMap
    {
        { "Actor", &Actor::Create<Actor> },
        { "BallActor", &Actor::Create<BallActor> },
        { "FollowActor", &Actor::Create<FollowActor> },
        { "PlaneActor", &Actor::Create<PlaneActor> },
        { "TargetActor", &Actor::Create<TargetActor> },
    };
    
    문자열 이름을 키로 사용하고 Actor::Create 함수의 주소를 값으로 사용해 맵에 엔트리를 설정한다.
    Actor::Create 함수는 Actor의 서브클래스로 지정된타입을 생성하기 위해 템플릿화했따.
    여기서는 함수의 메모리 주솔르 얻은 뒤 추후 사용할 수 있도록 주소를 저장한다.

    맵 구성을 마치면 LoadActors 함수를 생성한다.
    이 함수는 JSON 파일에서 액터 배열을 순회하며 액터에 대한 타입 문자열을 얻는다.
    이 타입 문자열을 sActorFactorymap에서 조회한다.
    타입이 발견되면 맵에서 값으로 저장된 Actor::Create의 올바른 함수 버전을 호출한다.

    그런 다음 LoadLevel 내부의 전역 속성을 로딩하는 부분 다음에 LoadActors 함수 호출을 추가한다.

    그러면 LevelLoader를 통해 액터를 로딩하고 속성을 설정하는 것이 가능해졌다.
    하지만 컴포넌트를 추가하지 않았으니 컴포넌트 추가가 필요하다.

*/
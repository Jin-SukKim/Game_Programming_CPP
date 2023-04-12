/*
    레벨 파일 저장하기

    레벨파일 저장하는 것은 파일에서 데이터를 로딩하는 것보다 더 간단하다.

    먼저 레벨을 위한 전역 변수를 작성한다.
    그리고 게임상의 모든 액터와 액터에 붙어있는 모든 컴포넌트를 순회한다.
    액터와 컴포넌트를 순회할 때는 관련 속성을 작성해야 한다.

    RapidJSON 인터페이스는 파일을 읽는 것보다 생성할 때복잡하다.
    하지만 전반적으로 로딩할 떄 사용한 기법을 사용할 수 있따.

    JsonHelper에 Add 함수를 만들면 존재하는 Json 오브젝트에 속성을 쉽게 추가 할 수 있다.

    AddInt 함수

    void JsonHelper::AddInt(rapidjson::DOcument::AllocatorType& alloc,
        rapidjson::Value& inObject, const char* name, int value)
    {
        rapidjson::Value v(value);
        inObject.AddMember(rapidjson::StringRef(name), v, alloc);
    }

    value가 상수값이 아닌것만 제외하고 GetInt 함수의 파라미터와 비슷하다.
    첫 번째 파라미터는 메모리를 할당할 때 RapidJSON의 메모리 할당자다.
    다른 할당자의 사용도 가능하다.
    AddMemeber 함수로 inObject에 지정된이름으로 값을 추가한다.

    LevelLoader::SaveLevel 함수로 저장을 구현한다.
    먼저 RapidJSON Documnet를 생성하고 SetObject를 호출해 루트 오브젝트로 만든다.
    그리고 버전 정수값을 추가한 후 JSON 파일로 출력된 문자열이 가독성이 좋도록
    StringBuffer와 PrettyWriter를 사용한다.
    문자열을 파일에 쓰기 위해 표준 출력인 std::ofstream을 사용한다.

    전역 속성 저장

    SaveGlobalProperties 함수를 추가한다.
    이 함수는 간단히 주변광과방향광 오브젝트에 대한 속성을 추가한다.

    액터와 컴포넌트 저장

    액터와 컴포넌트를 저장하려면주어진 액터나 컴포넌트의포인터를 통해서 타입의
    문자열 이름을 얻는 방법이 필요하다.
    컴포넌트는 이미 TypeID가있으므로 컴포넌트의 타입 문자열을 얻으려면 여러 컴포넌트 일므을 가진
    상수 배열을 선언하면 된다.

    static const char* TypeNames[NUM_COMPONENT_TYPES]

    그리고 component.cpp 파일에서이 배열을 채워준다.
    TypeID 열거형에 선언된 순서를 유지하며 string값으로 채워준다.

    Component* comp = 컴포넌트 포인터
    const char* name = Component::TypeNames[comp->GetType()];
    을 통해 컴포넌트의 이름을 얻는것이 편해진다.

    Actor나 서브클래스에도 같은 작업을 하기위해 TypeID 열거형을 추가한다.

    다음으로 Actor와 Component에 SaveProperties 가상함수를 작성해야 한다.
    재정의가 필요한 모든 서브클래스에 SaveProperties 함수를 재정의한다.
    이는 LoadProperties와 매우 비슷하다.

    그 후 ScaveActors와 SaveComponents 함수를 추가한다.

    SaveActors는 먼저 게임으로부터 상수 참조형인 액터에 대한 벡터를얻는다.
    그 후모든액터를 순회하며 각 액터에 대한 JSON 오브젝트를 생성한다.
    TypeID와 TypeNames 기능을 사용해 타입에 해당하는 문자열을 추가한다.
    그리고 속성에 대한 JSON 오브젝트를 생성한 뒤 액터의 SaveProperties 함수를 호출한다.
    마지막으로 액터의 JSON 오브젝트를 JSON 액터 배열에 추가한다.

    비슷하게 SaveComponents 함수도 구현한다.
*/
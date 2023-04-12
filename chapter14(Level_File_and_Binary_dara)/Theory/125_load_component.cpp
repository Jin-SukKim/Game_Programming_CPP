/*
    컴포넌트 로딩

    컴포넌트가 있는 액터는 Level2.gplevel에 있따.

    컴포넌트의 데이터 로딩은 액터로 데이터를 로딩하는 것과 유사하다.
    중요한 차이점은 컴포넌트는 액터가 해당 컴포넌트를 가지고 있는 경우와
    그렇지 않은 경우를 다룰 수 있어야 한다.

    기본 액터 타입은 액터에 붙은 컴포넌트가 없어서 새로운 MeshComponent를 생성해줘야 하지만
    TargetActor 같은 타입은 생성자에서 MeshComponent를 이미 생성해 놓은 상태다.
    이 경우 지정된 속성은 새로운 컴포넌트를 생성하지 않고 존재하는 컴포넌트를 업데이트해야 한다.

    액터가 지정된 타입의 컴포넌트를 가지고 있는지 여부를 결정하기 위해 타입으로
    액터의 컴포넌트 벡터를 조회하는 방법이 필요하다.

    보통 게임 프로그래머는 자신만의 타입 정보를 알아내는 기능을 구축하는데
    기본 C++ 런타임 타입 정보(RTII)가 사용하는 것에 대해서만 비용을 지불하라는 규칙을
    따르지 않기 때문이다.

    간단한 접근법으로 자신만의 타입 정보를 구현하는 법은 enum을사용하는 것이다.
    Component 클래스에 TypeID 열거형을 선언한다.

    enum TypeID
    {
        TComponent = 0,
        TAudioComponent,
        ... etc
        NUM_COMPONENT_TYPES
    };

    그런 다음 컴포넌트를 기반으로 올바른 TypeID를 반환하는 GetType 가상 함수를 추가한다.
    TypeID GetType() const override {return TMeshComponent;}

    그리고 mComponents 벡터를 순회하고 타입과 일치하는 최초의 컴포넌트르 리턴하는
    GetComponenetOfType 함수를 Actor에 추가한다.

    Component* GetComponentOfType(Component::TypeID type)
	{
		Component* comp = nullptr;
		for (Component* c : mComponents)
		{
			if (c->GetType() == type)
			{
				comp = c;
				break;
			}
		}
		return comp;
	}

    이 방법의 단점은 새로운 Component 서브클래스를 생성할 때마다 TypeID 열거형에 엔트리에 추가해야
    한다는 점과 GetType 함수를 구현해야 된다는 점이다.
    매크로나 템플릿을 사용하면 이 부분을 자동화할 수 있으니 여기서는 이해를 위해 그렇게 하지 않는다.

    그리고 지금은 한 액터에서 같은 타입의 복수 컴포넌트를 가지지 않는 걸 가정한다.
    같은 타입의 복수 컴포넌트를 가지려면 GetComponentOfType은 싱글 포인터를 반환하지 않고
    컴포넌트의 컬렉션을 반환해야 한다

    또한 타입 정보는 상속관련 정보를 제공하지 않는다.
    상속 정보를 지원하려면 계층정보를 저장하는 접근법이 필요하다.

    지금까지의 기초 타입 시스템을 토대로 다음 단계를 진행한다
    Actor의 경우처럼 기본 Component 클래스에 LoadProperties 가상 함수를 만든다.
    그리고 필요에 따라 서브클래스에서 이 가상 함수를 재정의한다.

    여러 서브클래스에서의 LoadProperties 구현은 쉽지않다.
    MeshComponent의 LoadProperties의 경우 MeshCompoenentt는 그리고자 하는 vertex 데이터에 대한
    포인터인 mMesh 멤버 변수를 가지고 있따.
    JSON 파일에서 직접 vertex를 지정하는 것 대신 gpmesh 파일을 참조하면 된다.
    코드는 먼저 meshFile 속성을 체크한 후 렌더러에서 해당 메시를 얻어온다.

    다음은 Component에 템플릿화한 Create 정적 함수를 추가하는 것이다.
    이건 파라미터가 다른걸 제외하고 Actor와 같다.

    그리고 LevelLoader에 ComponentFunc이란 맵을 만든다(액터와 비슷하다)
    하나의 값만 가졌던 sActorFactoryMap과는 다르게 한 쌍의 값을 갖는다.
    첫번째 요소는 컴포넌트의 TypeID 정수값이고 두 번째 요소는 ComponentFunc이다.

    LevelLoader에서 LoadComponent 헬퍼함수를 구현한다.
    LoadActors 처럼 필요컴포넌트를 로드하기 위해 컴포넌트 배열을 얻은 후 조회한다.
    컴포넌트 타입을 찾으면 액터에 이미 해당 타입의 컴포넌트가 있는지 검사한다.
    컴포넌트를 가지고 있지 않다면 저장된 함수를사용해 컴포넌트를 생성한다.
    이미 존재한다면 컴포넌트의 LoadProperties를 호출한다.

    마지막으로 LoadActors에다 컴포넌트 속성에접근하는 코드를 추가한다.

*/
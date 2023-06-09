/*
    프로젝트

    gllevl 파일에서 로드하며 R키로 저장한다.
    또한 .gpmesh.bin 포맷으로 이진 메시 파일의 저장 및 로딩을 구현한다.

    요약

    JSON 파일 포맷을 생성하는 방법을 공부했다.
    파일 로딩을 위해몇 가지 시스템을 구축해야 한다.

    1. JSON에 게임 타입을 쉽게 쓸 수 있도록 RapidJSON 라이브러리의 기능을
       감싸는 헬퍼 함수를 제작한다.
    
    2. 전역 속성을 설정한 뒤 액터를 로드하고액터와 관련된 컴포넌트를 로드하는 코드를 추가한다.

    3. 컴포넌트 타입을 동적으로 할당할수있도록 타입의 이름과 함수를 연결시키는 매핑 작업을 하고
       컴포넌트에타입 정보를 추가한다.

    4. COmponent와 Actor 클래스 모드에 LoadProperties 가상 함수를 만든다.

    5. 게임 세계를 JSON으로 저장하기 위한 코드를 작성한다
       이 과정에 도움을 주는 헬퍼 함수를 제작한다.

    6. 파일 저장에서는 우선 모든전역 속성을 저장한 뒤 모든 액터와 컴포넌트를
       순회하며 액터와 컴포넌트의 속성을 기록한다.

    7. Component와 Actor 클래스에 SvaeProperties 가상함수를 작성한다.

    텍스츠 포맷을 사용하면 개발 시에 편리하지만 성능과 디스크 사용모든 면에서 비효율적이다.
    이진 모드로 쓰고 읽는 바이너리 파일 포맷으로 설계하면 최적화에 큰 도움이 된다.

    연습

    1. SaveLevel 코드의 이슈는 개발자가 모든 액터 및 액터의 모든 컴포넌트에
       대한 속성을 기록한다는 데 있다.
       TargetActor 같은 특정 서브클래스는 생성 후에 속성이나 컴포넌트가 변경되는 일은 거의 없다.\

       이 중복되는 데이터 저장을 피하기 위해, 레벨을 저장할 때 임시 TargetActor를 생성하고
       일반적인 쓰기 테크닉을 사용해해당 액터의 JSON 오브젝트를 작성한다.
       이 JSON 오브젝트는 TargetActor의 템플릿 역할을 한다.

       그리고 레벨에저장하기 위해서 각각 템플릿으로 제공된 JSON 오브젝트와 비교해 서로 다른 경우에만 기록한다.

       이 과정을 모든 다른 타입의 액터에도 적용한다.

    2. 메시 파일에서 사용한 바이너리파일 테크닉을 적용해 애니메이션 파일에 댛나 바이너리 파일 포맷을 만든다.
       본 변환의 모든 트랙은 크기가 같으므로 헤더를 작성한 후 전체 트랙 정보를 이요해 각 트랙의 ID를
       쓰는 포맷을 사용하면 된다
       

*/
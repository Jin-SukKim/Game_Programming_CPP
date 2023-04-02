/*
    게임 코드에 충돌 추가하기

    BoxComponent 클래스로 액터 충돌 구현을 하고
    PhysWorld 클래스는 액터간 충돌을 감지하기 위해 BoxComponent 인스턴스를 관리한다.

    BoxComponent 클래스

    Update 함수 재정의 대신 OnUpdateWorldTransform 함수를 재정의한다.
    소유자 액터는 세계 변환을 재계산할 때마다 BoxComponent의 OnUpdateWorldTransform 함수를 호출한다.

    BoxComponent 클래스의 데이터 멤버에는 2개의AABB 구조체 인스턴스가 있다
    1. 오브젝트 공간 경계를 위한 AABB
    2. 세계 공간 상의 경계 AABB

    세계 공간 경계는 소유자 액터의 세계 변환이 변경될 때마다 바뀐다.
    마지막 BoxComponenet 클래스는 BoxComponent를 세계 회전을 기반으로 회전시킬지는
    정하는 이진값을 가진다.
    액터 회전시 이 인집값에 따라 회전 유무를 선택한다.

    메시 파일의 오브젝트 공간 경계(바운딩)을 얻기 위해 Mesh 클래스에도 멤버 데이터로 AABB를 추가한다.
    gpmesh 파일을 로드할 때 Mesh는 각 vertex마다 AABB::UpdateMinMax를 호출해 최적화된
    오브젝트 공간 AABB를 산출한다.
    그러면 메시를 사용하는 액터는 메시의 오브젝트 공간 바운딩 박스를 얻어 액터의 BoxComponent로 전달한다.

    오브젝트 바운딩 박스를 세계 바운딩 박스로 변환하기 위해선 바운딩 박스에 스케일, 회전, 이동을 적용하면 된다.
    세계 변환 행렬 구축과같이 회전은 원점이 중심임으로 순서가 중요하다.
    박스의 크기는 소유자 액터의 스케일값을 min, max에 곱해서 조절한다
    박스 회전시 AABB::Rotate에 소유자 액터의 쿼터니언을 전달한다.
    mShouldRotate 값이 ture인 경우 이 회전을 사용한다.
    박스의 이동은 소유자 액터의 위치를 min과 max에 더하면 된다.


    PhysWorld 클래스

    물리 세계를 위한 별도의 클래스이다.
    Game에 Physworld 포인터를 추가하고, Initialize 함수에서 PhysWorld를 초기화한다.

    이 클래스는 BoxComponenet의 포인터 벡터와 public 함수 AddBox, RemoveBox를 가진다.
    그리고 BoxComponent의 생성자와소멸자는 PhysWorld의 addBox와 RemoveBox 함수를 호출한다.
    이를 통해 PhysWorld 클래스는렌더러가 모든 스프라이트 컴포넌트의 벡터를 가졌던 것처럼
    모든 박스 컴포넌트의 벡터를 가진다.

    PhysWorld는 게임 세계 상의 모든 박스 컴포넌트를 추적한다.
    이런 박스들의 충돌 테스트 기능 지원을 추가한다.
    
    선분과 박스의 교차 테스트를 지원하는데 선분은 잠재적으로 여러 박스와 교차하므로
    SegmentCast 함수는 가장 가까운 교차가 가장 중요한 교차라고 가정한다.
    박스 컴포넌트의 벡터는 정렬됮 ㅣ않았으므로 SegmentCast는 최초로발생한 교차를
    단순히 return해선 안되고 모든 박스를 테스트하고 난 뒤 가장 작은 t값을 교차결과로반환한다.
    가장 작은 t값은 교차점이 선분의 시작점과 가장 가깝다는 것을 의미한다.

    
*/
/*
    점광 그리기

    점광을 그리기 전 Renderer와 PointLIghtComponent에 글루 코드를 추가한다.
    먼저 mGPointLightShader라는 새로운 셰이더 멤버 변수를 추가한다.
    그 후 LoadShaders에서 로드한다.
    vertex shader는 BasicMesh.vert를 사용하고 frag shader는 GBufferPointLIght.frag를 사용한다.

    전역 조명 셰이더와 마찬가지로 개발자는 특정 G 버퍼 텍스처와 uniform을 바인딩하기 위해
    샘플러 uniform을 설정한다.
    그리고 화면의 너비와 높이를 위한 uScreenDimensions uniform을 설정한다.

    점광에 사용할 메시를 가리키는 mPointLightMesh 멤버 변수도 추가한다.
    Renderer 초기화 시 메시를 로드하고 해당 변수에 저장한다.
    해당 메시는 구체다.

    DrawFromGBuffer에 추가 코드를 작성한다.
    이 추가 코드는 전역 조명을 적용해서 전체 화면 사각형을 그린 코드 다음에 처리한다.
    첫 부분은 G 버퍼의 깊이 버퍼를 기본 프레임 버퍼의 깊이 버퍼로 복사한다.
    3D 장면을 G 버퍼로 그렸으므로 G 버퍼의 깊이 버퍼는 모든 프래그먼트의 실제 깊이 정보를 포함한다.
    점광의 구체에 깊이 테스트를 수행해야하므로 기본 깊이 버퍼로 G 버퍼의 깊이 버퍼를 복사해야한다.

    이런 후 깊이 테스트를 재활성화한다.(전역 조명을 위해 전체 화면 사각형을 그렸을 때
    깊이 테스트를 비활성화했기 때문이다).
    하지만 깊이 마스크는 비활성화하는데 각 점광 구체에 대한 프래그먼트를 그릴 때 깊이 테스트는
    필요하지만, 프래그먼트의 새로운 깊이 값을 깊이 버퍼에 쓰지 않겠다는 것을 의마한다.
    이를 통해 점광 구체 메시가 기존 깊이 버퍼값에 간섭하는 것을 막을 수 있다.
    깊이 버퍼 쓰기를 비활성화했으므로 Draw3DScrene의 시작 부분에 깊이 버퍼쓰기를 재활성화하는 호출을 추가한다.
    (깊이 버퍼 쓰기를 활성화하지 않으면 깊이 버퍼를 지울 수 없다.)

    그리고 점광 메시와 점광 셰이더를 활성화한다.
    뷰-투영 행렬을 설정하고 G 버퍼 텍스처를 각각의 슬롯에 바인딩해준다.
    
    기존 색상 버퍼로 추가 색상을 더하기 위해 블렌딩을 활성화한다.
    첫 번째, 두 번째 파라미터로 GL_ONE을 받는 blend 함수는 알파값이나
    다른 파라미터를 고려하지 않고 직접 두 색상을 더하라고 지정한다.

    마지막으로 모든 점광을 반복하며 각 점광의 Draw 함수를 호출한다.
    PointLightComponent::Draw는 다른 메시 그리기와 비슷하다.
    
    세계 변환 행렬에서는 구체 메시의 크기를 광원의 외부 반지름 값으로 변경해야 한다.
    이를 위해 소유자 액터의 스케일 값에다 메시의 반지름을 나누고 광원의 외부 반지름을
    곱해서 얻은 결괏값을 스케일 행렬을 생성하는 함수에 파라미터로 전달한다.
    
    이동은 소유자 액터로부터 얻은 광원 위치를 참조한다.

    이전 uniform을 설정했던 방법대로 특정 점광을 위한 uniform을 설정한다.
    glDrawElements 호출은 점광의 광원 기하를 그린다.
    이 광원은 구체 메시다.
    Renderer는 Draw를 호출하기 전에 vertex 배열을 활성화했으므로 따로 vertex 배열 활성화는 필요없다.

    모든 점광 메시를 그리고 추가 광원 계산도 끝나면 이 추가 광원 색상은 전역 조명 패스로부터
    얻은 기존 색상과 더해지며 최종 색상을 얻는다.

*/
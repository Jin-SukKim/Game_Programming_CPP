/*
    언프로젝선(unprojection)

    세계 공간의 점을 클립 공간으로 변환하기 위해선 뷰 행렬을 투영 행렬과 곱해야 한다.
    그런데 1인칭 슈팅 게임에서 플레이어가 조준점의 화면 위치를 토대로 발사체를 쏘는데
    이 경우 조준점의 위치는 화면 공간상의 좌표다.
    하지만 올바르게 발사체를 쏘려면 세계 공간에서의 조준점 위치가 필요하다.
    언프로젝션은 화면 공간 좌표로부터 세계 공간 좌표로 변환하는 계산이다.

    OpenGL의 화면 공간 좌표 체계는 화면의 중심이 (0, 0)이고,
    왼쪽 상단 구석이 (-512, 384)였고, 하단 오른쪽 구석이 (512, -384)이다

    언프로젝션을 계산하는 첫 단계는 화면 공간 좌표 x, y요소를 [-1, 1]의 범위값을 가진
    정규화된 장치 좌표로 변환하는 것이다.

    ndcX = screenX / 512
    ndcY = screenY / 384

    여기서의 문제점은 하나의 (x, y) 좌표는 [0, 1]의 범위를 가지는 z 좌표도 가진다.
    z 값이 0이면 가까운 평면의 점을 의미하고(카메라의 바로 앞) z값이 1이면 먼 평면의 점이다
    (카메라를 통해 볼 수 있는 최대 거리).
    그래서 언프로젝션을 올바르게 수행하기 위해 범위 [0, 1]에서의 z 요솟값이 필요하다.
    그리고 좌표는 동차 좌표로 나타낸다.

    ndc = (ndcX, ndcY, z, 1)

    그 후 언프로젝션 행렬을 생성한다.
    언프로젝션 행렬은 뷰 투영 행렬의 역행렬이다.

    Unporjection = ((View)(Projection))^-1

    언프로젝션 행렬을 NDC에 곱하면 w 요솟값이 변경된다.
    그래서 각 오숏값을 w로 나눠 w 요소를 1로 되돌릴 수 있도록 재정규화가 필요하다.
    이를 위해 다음 계산식을 필요하며 그 결과 세계 공간의 점을 얻게 된다.

    temp = (ndc)(Unprojection)
    worldPos = temp / wtemp

    // x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1) -- 0 is closer to camera, 1 is further

    Renderer 클래스는 뷰 행렬과 투영 행렬 모두 접근할 수 있는 유일한 클래스이므로
    Renderer 클래스에 언프로젝션 함수를 추가한다.
    이 코드에서 TransformWithPerspDiv 함수는 언프로젝션 행렬과 w 요소를 사용해 재정규화를 수행한다.

    Unproject 함수를 사용하면 세계 공간상의 위치 계산이 가능하다.
    이 함수를 활용해 화면 공간 점으로 향하는 벡터를 만들어두면 다른 유용한 기능을
    활용할 수 있는 기회를 얻을 수 있다.
    이 기능 중 하나는 피킹(picking)이다.
    피킹은 3D 세계상에서 오브젝트를 클릭해서 선택할 수 있게 하는 기능이다.
    
    방향 벡터를 만들려면 Unproject를 두 번 사용해야 한다.
    시작점에서 한 번 사용하고, 한 번은 끝점에서 사용한다.
    그런 다음 벡터 간에 뺄셈을 한 뒤 이 벡터를 정규화한다.
    GetScreenDirection 함수는 이것을 구현한 것이고 함수가 세계 공간에서 벡터 시작점과 방향을
    계산하는 방법에 유의해야 한다.


*/
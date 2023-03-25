/*
    3D 메시 그리기

    3D 메시를 로드한 뒤 그려야한다.
    그리기 전에 많은 일이 진행되어야 한다.

    3D 메시 그리기를 하면 코드가 복잡해 지기에 렌더링을 관련해서 코드를 캡슐화해준다.
    (렌더링 클래스)
    Game 클래스의initialize()에서 Renerer 클래스를 초기화한다.
    GenerateOutput()에서 렌더러의 Draw() 함수로 이제 화면을 그려준다.
    로드된 텍스처 맵, 메시 맵, SpriteComponents도 모두 렌더러로 옮긴다.
    즉, 렌더링 관련 모든것이 렌더러 클래스를 통해 이뤄진다.

    클립 공간으로의 변환, 재검토

    챕터 5에선 뷰-투영 행렬이 world space 좌표를 클립 공간 좌표로 변환했다.
    3D에서는 뷰-투영 행렬을 별도의 뷰 행렬과 투영 행렬로 분해해야 한다.

    뷰 행렬(view matrix)

    뷰 행렬은 세계에서 카메라 또는 눈의 위치 및 방향을 나타낸다(자세한건 나중 챕터9에서 공부)
    look-at 행렬은 카메라의 위치와 방향을 나타낸다.

    Matrix4x4의 CreateLookAt() 함수의 구성요소

    1. 눈의 높이
    2. 눈이 바라보는 타깃의 위치
    3. 위쪽 방향

    이 파라미터들을 사용해 4개의 벡터를 계산한다

    * = 내적, x = 외적
    k = (target - eye) / (||target - eye||)
    i = (up x k) / (||up x k||)
    j = (k x i) / (||k x i||)
    t = (-i * eye, 1j * eye, -k * eye)
    
    이 벡터들로 look-at 행렬의 요소들을 정의한다.
             
             | ix  jx  kx  0 |
    LookAt = | iy  jy  ky  0 |
             | iz  jz  kz  0 |
             | tx  ty  tz  1 |

    카메라를 이동시키는 빠른 방법은 카메라를 위한 Actor를 만드는 것이다.
    이 액터의 위치는 눈의 위치를 나타낸다.
    타깃 위치는 카메라 액터의 앞에 있는 어떤 점이 된다.
    위쪽 방향을 액터가 뒤집혀지지 않는다면 +z가 된다.

    코드 예시

    // 카메라 위치
    Vector3 eye = mCameraActor->GetPosition();
    // 카메라 앞의 10 유닛 떨어진 점
    Vector3 target = mCameraActor->GetPosition() + mCameraActor->GetForward() * 10.f;
    Matrix4 view = Matrix4::CreateLookAt(eye, target, vector3::UnitZ);

    투영 행렬(projetion matrix)

    투영 행렬은 3D 세계가 화면상의 2D 세계에 그려질 때 평평해지는 정도를 결정한다.
    2가지 타입의 투영 행렬이 존재한다.

    1. 직교 투영(orthographic projection)
    2. 원근 투영(perspective projection)

    직교 투영 :
        카메라에서 멀리떨어져 있는 오브젝트든 가까이 있는 오브젝트든 그 크기가 같다.
        이는 물체가 카메라로부터 가까이 잇는지 멀리 있는지 플레이어가 지각할 수 없음을 의미한다.(원근감이 없다)
        대부분의 2D 게임이 직교 투영을 사용한다.

    원근 투영 :
        카메라보다 멀리 떨어져 있는 물체는 카메라에 가까이 있는 물체보다 더 작게 보인다.
        즉, 플레이어는 화면에 깊이가 있음을 인지한다.
        대부분의 3D 게임에서 원근 투영을 사용한다.

    각각의 투영은 가까운 평면과 먼 평면을 가진다.

    가까운 평면은 일반적으로 카메라에 매우 가깝다.
    카메라와 가까운 평면 사이에 있는 모든 물체는 화면상에 보이지 않는다.
    이는 카메라가 물체에 너무 가까우면 물체가 부분적으로 사라지는 이유가 된다.
    게임은 때때로 플레이어에게 퍼포먼스 향상을 위해 '그려질 부분의 거리'를 줄이는 것을 혀용한다.
    이를 위해 종종 먼거리의 평면을 당기는 방법을 사용한다.

    직교 투영 행렬은 4개의 파라미터가 있다.

    1. 뷰의 너비
    2. 뷰의 높이
    3. 가까운 평면과의 거리
    4. 먼 평면과의 거리

    직교 투영 행렬
                    | 2/width       0              0          0 |
    Orthographic =  |   0       2/height           0          0 |
                    |   0           0       1/(far - near)    0 |
                    |   0           0      near/(near - far)  1 |

    2D의 뷰-투영 행렬과 비슷하지만 가까운 평면과 먼 평면을 기술하는 추가적인 내용이 있다.

    원근 투영은 수평 시야각(FOV, horizntal Field Of View)라는 추가 파라미터를 가진다.
    FOV는 투영을 통해 볼 수 있느 카메라의 수평 시야 각도다.
    FOV, 즉 시야를 변경하면 3D 세계가 눈에 들어오는 범위를 조정할 수 있다.

    원근 투영 행렬

    fov = 시야각도
    yScale = cot(fov/2)
    xScale = yScale * (height / width)
                    | xScale        0                   0               0 |
    Perpective   =  |   0         yScale                0               0 |
                    |   0           0           far/(far - near)        1 |
                    |   0           0       (-near * far)/(far - near)  0 |

    원근 행렬은 동차 좌표의 w 요소를 변경한다.
    그리고 원근 나누기(perspective divide)는 변환된 vertex의 각 요소를 w 요소로 나눈다.
    그래서 w 요소는 다시 1이 된다.
    w 로 나누는 연산에 의해 물체가 카메라에서 더 멀리 떨어질수록 물체의 쿠기는 더 많이 축소된다.
    OpenGL은 자동으로 장면에 대한 원근 나누기를 수행한다.

    Matrix4x4 클래스에 CreateOrtho() 함수와 CreatePerspectiveFOV() 함수로 행렬 생성을 구현했다.

    뷰-투영 계산하기

    뷰-투영 행렬은 뷰와 투영 행렬 간의 단순한 곱이다.

    ViewProjection = (View)(Projection)

*/  
/*
    빛 반사(light reflection)

    광원을 시뮬레이션 하려면 광원과 관련된 데이터가 필요할 뿐 아니라
    광원이 장면에 있는 오브젝트에 얼마나 많은 영향을 미치는지를 계산해야 한다.

    광원을 근사하는 검증도니 방법으로 양방향 
    반사도 분포 함수(BRDF, bidirectional reflectance distribution function)이 잇다.

    BRDF는 빛이 포면에서 반사되는 양을 근사하는 함수다.
    여러 유형 중 많이 쓰이는 고전적 모델 중 하나인 퐁 반사 모델이 있다.

    퐁 반사 모델(Phong reflection model)

    퐁 모델은 빛의 2차 반사를 계산하지 않으므로 제한된 광원 모델이다.
    반사 모델은 각 오브젝트를 전체 장면에서 유일한 오브젝트로 여기고 광원을 비춘다.

    퐁 모델은 빛을3 개의 요소로 구분한다.

    1. 주변 반사(ambient)
    2. 난반사(diffuse)
    3. 정반사(specular)

    3가지 요소 모두 표면에 영향을 미치는 빛의 색상뿐만 아니라 표면 색상을 고려한다.

    주변 반사 요소(ambient component)는 장면의 전반적인 조명이다.
    그래서 주변광과 직접연결하는 것이 좋다.
    주변광은 전체 장면에 균등하게 적용되므로 주변 반사 요소는 다른 광원이나 카메라에 대해 독립적이다.

    난반사 요소(diffuse component)는 표면으로부터 나오는 빛의 주 반사다.
    모든 방향광이나 점광, 또는 스포트라이트가 난반사 요소를 결정한다.
    계산에는 표면의 법선 벡터와 표면에서 광원으로의 벡터가 사용된다.
    카메라의 위치는 난반사 요소에 영향을 미치지 않는다.

    정반사 요소(specular component)는 표면의 광택을 근사한다.
    광택이 나는 금속 물체처럼 높은 반사도를 가진 오브젝트는 광이 나지 않는
    검정색으로 채색된 물체보다 더 밝은 하이라이트를 가진다.
    난반사 요소처럼 정반사 요소도 광원 벡터와 표면의 법선 벡터에 의존한다.
    그러나 반사도는카메라의 위치에 따라서도 달라진다.
    여러 다른 각도에서 광택이나는 물체를 바라보면 사람이 지각할 수 있는 반사율이 변화되서이다.

    퐁 반사 계산을 위해선 몇몇 변수를 포함한 일련의 계산이 필요하다.

    1. n = 정규화된 표면 법선 벡터
    2. l = 표면에서 광원으로의 정규화된 벡터
    3. v = 표면에서 카메라(눈) 위치로의 정규화된 벡터
    4. r = n에 대한 -l의 정규화된 반사 벡터
    5. a = 정반사 지수(오브젝트의 광택을 결정)

    또한 광원에 대한 색상이 필요하다.

    6. ka = 주변 색상
    7. kd = 분산 색상
    8. ks = 반사 색상

    퐁 반사 모델의 표면에 적용된 광원의 계산
    * = 내적
    Ambient = ka
    Diffuse = kd(n * l)
    Specular = ks(r * v)^a
    Phong = Ambient + Specular + Diffuse, if n * l > 0 else Specular + Diffuse = 0

    난반사와 정반사 요소는 장면에 있는 모든 광원을 이용해서 계산하지만 주변 요소는 하나뿐이다.
    n * l 테스트는 광원이 자신을 마주보는 표면에만 영향을 미침을 보증한다.

    이 방정식은 장면의 모든 광원에 대한 색상을산출한다.
    표면의 최종 색상은 표면의색상과 빛의 색을 곱한 값이다.
    광원색과 표면색은 RBG 값이므로 각 요소별 곱셈을 사용한다.

    보다 복잡한 구현에서는 표면 색상을 별도의 주변반사, 난반사, 정반사색상으로 분리한다.
    이 구현에서는 하나의 곱셈이 아닌 각각 별도의 색상을 곱하도록 변경된다.

    얼마나 자주 BRDF를 계싼해야 하는가는 일반적으로 3가지 방법이 존재한다.

    1. 표면마다 한 번씩(flat shading, 플랫 셰이딩)
    2. vertex마다 한 번씩(Goround shading, 고라우드 세이딩)
    3. 픽셀마다 한 번씩(Phone shading, 퐁 셰이딩)
    
    픽셀당 조명이 계산상으로 더 비싸지만 현대의 GPU는 픽셀당 조명을 쉽게 다룬다.
    어떤 게임은 미를 살리기 위해 다른 유형을 셰이딩을 선택하기도 한다.
*/
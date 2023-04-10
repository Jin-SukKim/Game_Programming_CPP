/*
    지연 셰이딩(Deferred Shading)

    퐁 조명은 메시를 그릴 떄 각 프래그먼트마다 광원 연산을 수행했다.
    이런 유형의 광원 연산에 대한 의사코드이다.

    foreach MEhs m in Scene
        foreach Pixel p to draw from m
            if p passes depth test
                foreach Light li that effects p
                    color = Compute lighting equation(li, p)
                    Wrtie color to framebuffer

    순반향 렌더링(forward rendering)이란 조명 계산 수행 방법은 적은 수의 광원에서 잘 작동한다.
    하지만 조명이 많이 필요한 야간 맵등에선 수십 개의 광원을 제대로 렌더링하지 못한다.
    조명 방정식을 O(m * p * li) 순서로 계싼해야 핮는데, 광원 여러개를 추가시 계산의 양이 크게 늘어나서이다.

    대안책은 장면에서 보이는 표면에 대한 정보를 저장하도록 G 버퍼(G-buffer)라는
    일련의 텍스처들을 생성하는 것이다.
    
    이 G 버퍼에는 분산광(알베도), 반사 지수, 그리고 장면에서 보이는 표면의 법선이 포함된다.
    두 패스로 장면을 렌더링한다.

    1. 모든 메시에 대해 G 버퍼로 메시 표면의 특성을 렌더링한다.
    2. 모든 광원을 반복하면서 이 광원과 G 버퍼에 있는 값을 토대로 조명 방정식을 계산한다.

    foreach Mesh m in Scene
        foreach Pixel pl to draw from m
            if p passes depth test
                Wrtie surface properties of p1 to G-buffer

    foreach Light li in the scene
        foreach Pixel p2 affected by li
            s = surface properties from the G-buffer at p2
            color = Compute lighting equation (l, s)
            Write color to framebuffer

    이 2개 패스 접근법의 복잡도는 O(m*p1 + li*p2)다.
    즉, 순반향 렌더링보다 훨씬 많은 광원을 지원할 수 있는것이다.
    패스가 2개이므로 화면상의 프래그먼트 셰이딩은 두 번째 패스 때까지는 일어나지 않는다.
    그래서 지연 셰이딩(deferred shading) 또는 지연 렌더링(deferred rendering)이라 불린다.

    지연 셰이딩 구현은 몇 가지 단계가 필요하다.

    1. 다중 출력 체스처를 지원하는 프레임 버퍼 개체를 설정한다.
    2. 표면 특성을 G 버퍼에 쓰는 프래그먼트 셰이더를 작성한다.
    3. 전체 화면을 다룰 수 있고 G 버퍼의 샘플을 다룰 수 있는 사각형을 그려
       전역 조명(방향광, 주변광 등)의 결과를 출력할 수 있게한다.
    4. 비전역 광원(점광, 스포트라이트 등)에 대한 조명을 계산한다.
*/
/*
    알파 블랜딩(Alpha Blending)

    알파 블렌딩은 픽셀에 투명도를 섞는 방법을 결정한다.
    (알파 채널의 값은 1보다 작다.)

    알파 블렌딩은 다음의 방정식을 사용해 픽셀의 색상을 계산한다.

    outputColor = srcFactor * sourceColor + dstFactor * destinationColor

    이 방정식에서 소스 색상(source color)은 fragment shader에서 그리려는
    새로운 소스의 색상이며, 대상 색상은 색상 버퍼에 이미 존재하는 색상이다.
    factor parameter를 지정해 알파 블렌딩 함수를 커스터마이징하는 것이 가능하다.

    원하는 투명도의 알파 블렌딩 결과를 얻으려면 픽셀의 알파에 source factor(source alpha)를
    설정하고, 대상 factor(1-source alpha)로 설정한다.

    outputColor = srcAlpha * sourceColor + (1 - srcAlpha) * detinationColor

    ex) 색상당 8비트이고 일부 픽셀의 색상 버퍼에 빨간색이 저장돼있다

        대상 색상(destinationColor) = (255, 0, 0)

        파란색 픽셀을 그린다

        소스 색상(sourceColor) = (0, 0, 255)

        소스 알파가 0이라 가정하면 픽셀이 완전히 투명하다는 뜻이다.
        이 경우 방정식은 최종 색상을 다음과 같이 평가한다

        outputColor = 0 * (0, 0, 255) + (1 - 0) * (255, 0, 0)
        outputColor = (255, 0, 0)

        이 결과 완전히 투명한 픽셀을 원한 결과다.
        알파가 0이면 소스 색상은 완전히 무시되며 색상 버퍼에 이미 있는 색상만을 사용한다.

    코드에서 알파 블렌딩 활성화를 위해 코드를 추가한다.

    // 알파 블렌딩 활성화
    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,   // srcFactor = srcAlpha
        GL_ONE_MINUS_SRC_ALPHA  // dstFactor = 1 - srcAlpha
    );
*/
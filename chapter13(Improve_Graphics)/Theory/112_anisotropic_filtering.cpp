/*
    이방성 필터링(anisotropic filtering)

    밉매핑은 샘플링 아티팩트를 크게 줄여주지만, 카메라가 비스듬한 각도에서
    텍스처를 보면 텍스처는 매우 흐릿하게 보인다.
    이 상황은 특히 바닥 텍스처에서 현저하게 나타난다.

    이방성 필터링은 비스듬한 각도에서 볼 때 텍스처에서 추가 점들을 샘플링해 이런 상황을 완화한다.
    ex) 16x 이방성 필터링은 최종 텍셀 선택을 위해 16개의 다른 텍셀을 샘플링한다.

    GPU는 수학 함수들을 사용해 이방성 계산을 수행한다.
    자세한 내용은 OpenGL Extensions Registry에서 확인가능하다.

    OpenGL의 최신 스펙은 기본 기능으로 이방성 필터링을 포함하고 있지만
    이상성 필터링은 3.3 버전의 OpenGL의 확장 기능이다.
    이 기능을 활성화하기 전 이방성 필터링을 지원하는지를 검증해야 한다.

    대부분의 경우 지난 10년간 제작된 모든 GPU는 이방성 필터링을 지원하므로 지원 여부 체크는 별로 의미가 없다.
    하지만 일반적으로 OpenGL 확장 기능 사용 가능한지는 테스트하는 편이 좋다.

    이방성 필터링 활성화를 위해 우선 텍스처가 밉매핑을 사용하도록 설정한 뒤 다음 코드를 추가한다.

    // OpenGL 확장 기능 사용 가능성 테스트
    if (GLEW_EXT_texture_filter_anisotropic)
    {
        // 이방성 최댓값을 얻는다.
        GLfloat largest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
        // 이방성 필터링을 활성화한다.
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
    }

*/
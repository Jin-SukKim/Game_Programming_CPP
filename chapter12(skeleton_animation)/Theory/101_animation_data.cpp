/*
    애니메이션 데이터

    뼈대의 바인드 포즈는 각 본의 로컬 포즈로 표현할 수 있듯이 뼈대의 임의의 포즈에 대한 표현도 가능하다.
    뼈대의 현재 포즈는 각 본의 로컬 포즈 모음이다.
    그리고 애니메이션은 시간이 지남에 따라 플레이되는 일련의 포즈다.
    개발자는 바인드 포즈를 사용해 각 본의 로컬 포즈를 전역 포즈 행렬로 변환한다.

    이 애니메이션 데이터를 본 변환으로 구성된 동적 배열에 저장한다.
    이 경우 행은 본에 해당하고 열은 애니메이션의 프레임에 해당한다.

    프레임 단위로 애니메이션을 저장하는 것의 한 가지 문제는 애니메이션의
    프레임 레이트가 게임의 프레임 레이트와 동일하지 않다는 점이다.
    (ex: 애니메이션 프레임 30fps, 게임 프레임 60fps 등)
    
    애니메이션 코드가 프레임마다 애니메이션의 재생 시간을 추적한다면 델타 시간으로
    애니메이션을 갱신할 수 있다.
    하지만 게임은 애니메이션 두 프레임 사이에서도 보간된 애니메이션 연출을 원할 수 있다.
    이를 위해 BoneTransform에 정적 함수인 Interpolate를 추가한다.

    // 애니메이션 프레임 사이의 현재 로컬 포즈를 얻기 위해 두 애니메이션 프레임의 본 변환을 보간한다.
    BoneTransform BonTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
    {
        BoneTransform retVal;
        // 구면 선형 보간
        retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
        // 선형 보간
        retVal.mTranslation = Vector3d::Lerp(a.mTranslation, b.mTranslation, f);
        return retVal;
    }

*/
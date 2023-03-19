/*
    변환 행렬을 사용하는 셰이더로 갱신(Transform.vert)

    타입 지정자 uniform으로 새로운 전역 변수를 선언한다.
    uniform은 셰이더 프로그램의 수많은 호출 사이에서도
    동일하게 유지되는 전역 변수다.
    셰이더가 실행될 때마다(vertex나 pixel마다) 매번 
    변경된s in, out 변수와는 대조된다.

    2개의 다른 행렬이 필요해 2개의 uniform 변수를 선언한다.

    3D 공간에서 동차 좌표가 필요한 4x4 행렬을 만들었따.

    Vertex shader에서도 3D inPosition을 사용하도록 vec4를 사용한다.
    이 위치는 오브젝트 공간에 있다.

    이 위치를 세계 변환 행렬로 곱해서 세계 공간 좌표로 변환해준다.
    그리고 변환된 위치값을 뷰-투영 행렬로 곱해 클립 공간의 좌표로 변환한다.

    vertex shader는 이제 변환 행렬과 뷰-투영 행렬을 위한 uniform 변수가 필요하므로
    이 변수 타입을 C++ 코드에서 설정하는 방법이 필요하다.

    OpenGL은 활설화된 셰이더 프로그램에 uniform 변수를 설정하는 함수를 제공한다.
    이런 함수의 wrapper 함수는 Shader 클래스에 추가해준다.

    // 해당 이름의 uniform을 찾는다. Uniform의 ID를 return한다.
    // 두번쨰 파라미터는 셰이더 파일에서 변수 이름이다.
    // 이 Uniform Id는 프로그램 실행 동안 변경되지 않는다.
    // 이 값을 캐싱해둬 코드 성능을 향상시킬 수 있다.
    glGetUniformLocation(glCreateProgram shaderProgram, char* name)
    // 행렬 데이터를 uniform에 전송한다.
    glUniformMatrix4fv(
        loc, // uniform ID
        1,  // 행렬의 수
        GL_TRUE, // 행 벡터 사용시 True
        matrix.GetAsGloatPtr()  // 행렬 데이터에 대한 포인터(행렬을 포인터로 변환시켜준다)
    )

    OpenGL은 Uniform Buffer Objects(UBO)라는 unfirom을 설정하는 새로운 방법도 제공한다.
    UBO는 셰이더에 여러 개의 uniform을 그룹화해 한꺼번에 보낼 수 있어 효율적이다.
    프레임당 한 번만 갱신되는 uniform들을 그룹화하거나 오브젝트별로 갱신되는 uniform을 그룹화한다.
    뷰-투영 행렬은 한 프레임에 한 번 변경될 것이고, Actor는 오브젝트별로 변경된다.

    vertex shader의 행렬 uniform 설정을 했으면 전달을 해준다.

    이번 프로젝트의 뷰-투영 행렬은 프로그램에서 변경되지 않으므로 한 번만 설정하면 충분하다.
    그러나 화면에 그려지는 각 스프라이트 컴포넌트는 컴포넌트 소유자 Actor의 세계 변환 행렬로
    그려지므로 세계 변환 행렬은 프레임마다 설정해야 한다.

    뷰-투영 행렬은 Game::LoadShader에서 해주면 간단하다.
    SpriteComponent의 세계 변환 행렬은 복잡하다.

    Actor의 세계 변환 행렬은 게임 세계에서의 위치, 크기, 방향을 나타낸다.
    스프라이트의 경우 텍스처의 크기를 토대로 사각형의 크기를 스케일해야 한다.
    (ex: Actor가 1.f의 스케일을 가지고 스프라이트의 텍스처 이미지가 128x128이라면
         단위 사각형을 128x128까지 키워줘야한다)

    먼저텍스처의 너비와 높이로 스케일을 하기위한 크기 행렬을 생성한다.
    이 행렬로 먼저 변환을 완성하고 셰이더 프로그램에 전달한다.
    그리고 glDrawElements()를 호출해 삼각형을 그린다.

*/
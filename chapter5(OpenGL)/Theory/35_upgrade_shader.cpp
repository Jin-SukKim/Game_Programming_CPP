/*
    셰이더 갱신

    vertex format이 텍스처 좌표를 사용하므로 새로운 셰이더를 작성한다.

    vertex 셰이더

    이전엔 위치 좌표만 속성으로 사용해 변수로 위치 하나만 선언했다.
    하지만 이제 복수개의 vertex 속성이 있으므로 어떤 속성 슬롯이 어떤 변수에
    해당하는지를 구체적으로 지정해야 한다.
    ex)
        layout(location=0) in vec3 inPosition;
        layout(location=1) in vec2 inTexCoord;

    layout 명령어는 속성 슬롯이 어떤 변수에 해당하는지를 지정한다.

    버텍스 속성 슬롯 0에 float 3개를 가진 3D 벡터 좌표를 지정했고
    슬롯 1에 float 2개의 2D 벡터 텍스처 좌표를 지정했다.
    glVertexAttribPointer() 함수에서 지정한 슬롯 번호에 해당한다.

    그리고 텍스처 좌표가 vertex shader로의 입력이긴 하지만
    (텍스처 좌표가 버텍스 레이아웃에 있기 때문)
    fragment shader는 픽셀의 색상을 결정하기 위해 텍스처 좌표를 알아야되서  
    프래그먼트 셰이더 또한 텍스처 좌표를 알아야한다.

    vertex shader에서 전역 out 변수를 선언하면 vertex shader에서
    fragment shader로 데이터를 전달할 수 있따.

    out vec2 frafTexCoord;

    vertex shader의 메인 함수에 vertex 입력 변수로부터 출력 변수로
    텍스처 좌표를 직접 복사하는 라인을 추가한다.

    fragTexCoord = inTexCoord;

    OpenGL이 자동적으로 삼각형의 면에 걸쳐
    vertex shader 출력을 자동으로 보간해주기에 위 코드가 잘 작동한다.

    따라서 삼각형이 오직 3개의 정점만 있다 하더라도 삼각형 면의 모든
    임의의 픽셀은 fragment shader에서 자신에 해당하는 텍스처 좌표를 알 수 있따.

    fragment 셰이더

    원칙적으로 vertex shader의 모든 out 변수들은 fragment shader에서
    이에 해당하는 in 변수를 갖고 있어야 한다.
    in 변수의 이름과 타입은 vertex shader의 out 변수와 동일한 이름과 타입을 갖고 있어야한다.

    in vec2 fragTexCoord;      

    제공된 텍스처 좌표로 색상을 얻기 위해 텍스처 샘플러 uniform을 추가한다.
    텍스처 좌표로 색상을 얻은 후 변경될 필요가 없으므로 uniform으로 설정
    
    uniform sampler2D uTexture;

    sampler2D 타입은 2D 텍스처를 샘플링할 수 있는 특별한 타입이다.
    vertex shader에서 사용된 세계 변환 행려이나 뷰-투영 uniform과 다르게
    이 샘플러 uniform은 binding이 필요없다
    지금 구조에서는 한 번에 오직 하나의 텍스처만 바인딩해서 그렇다.

    최종 픽셀에 해당하는 outColor에 텍셀을 할당한다.

    outColor = texture(uTexture, fragTexCoord);

    vertex shader로부터 넘겨받은 텍스처 좌표를 사용해 텍스처로부터 색상을 샘플링한다.
    (텍스처 좌표는 삼각형의 면에 걸쳐 보간됐다)

    지금 텍스처를 포함해 Actor를 그려보면 투명해야할 픽셀을 검정색으로 그리고있는데
    이 문제는 Alpha Blending으로 해결한다.
*/
/*
    텍스처 품질 개선하기

    텍스처가 화면상에서 커지면 이중 선형 필터링 기법이 텍스처의 시각적 품직을 향상 시킬 수 있다.
    ex) 벽에 텍스처가 있을 때 근접할 수록 커지게 되는데 필터링 미적용시 픽셀이 눈에 보이게 된다.
        필터링을 적용시 이미지는 좀 더 부드럽게 보이게 된다(하지만 다소 흐려진다)

    이미지는 단지 픽셀의 2D 격자이며 이 "텍스처 픽셀" 각각은 텍셀로 불렸다.
    확대 효과를 살펴보는 다른 방법은 벽 텍스처가 화면에서 커짐에 따라 모든
    텍셀의 크기가 화면에서 커지는 것을 보는 것이다.

    즉, 텍스처의 텍셀과 화면상의 픽셀 비율이 감소한다.
    ex) 하나의 텍셀이 화면상의 2픽셀에 해당하면 비율은 1:2다.

    텍셀의 밀도는 텍셀과 화면상의 픽셀 사이의 비율이다.
    이상적으로 텍셀 밀도가 가능한 1:1에 가까우면 좋다.
    텍셀 밀도가 감소하면 이미지 품질도 감소한다.
    
    결국 텍스처는 너무 픽셀화되거나(최근접 이웃 필터링을 사용하면)
    또는 너무 흐리게 나타난다(이중 선형 필터링을 사용하면)

    텍셀 밀도가 너무 높으면 화면상의 각 픽셀은 텍스처상의 복수의 텍셀과 매핑된다.
    ex) 10:1 텍셀 밀도는 화면상의 모든 픽셀이 10개의 텍셀과 매핑됨을 뜻한다.

    궁극적으로 이런 픽셀들 각각은 화면에 출력될 단일한 색상을선택해야 한다.
    즉, 텍스처는 화면상에서 보일 떄 텍셀을 잃어버리게 되는데 이걸 샘플링 아티팩트(sampling artifact)라 한다.
    (텍스처 이미지가 원본보다 커지거나 작아짐에 따른 원본 그래픽 품질 결함)
    이 용어는 그래픽 알고리즘의 결과에 따른 그래픽 결함을 뜻한다.

    ex) 
        1. 1:1의 텍셀 밀도는 원본 이미지 파일과 같은 비율로 텍스처가 화면에 나타난다.
        2. 1:5의 텍셀 밀도는 텍스처의 일부분을 보여주는데 가장자리가 흐릿하게 보인다
        3. 5:1의 텍셀 밀도는 텍스처가 작게보이며 가장자리가 사라지는 원인을 제공한다

    이 문제를 보안하기 위해 여러가지 텍스처 필터링 기법이 존재한다.
*/
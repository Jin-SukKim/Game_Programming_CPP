/*
    평면(plane)

    선분이 무한히 확장회는 1차원 오브젝트인 것처럼, 
    평면은 무한히 확장되는 평평한 2차원 표면이다.
    평면을 땅이나 벽면을 추상화한 형태로 표현한다.
    
    평면의 방정식

    P * n + d = 0
    P = 평면상의 임의의 점,
    n = 평면에 수직하는 법선 벡터
    d = 평면과 원점 사이의 부호있는 최소 거리값

    게임 코드는 일반적으로 점이 해당 평면상에 놓여 있는지 자주 확인한다.
    (즉 점이 평면상에 있다면 평면의 방정식을 만족한다.)
    평면을 표현하기 위한 plane 구조체의 정의는 법선 벡터와 d값만으로 충분하다.

    struct Plane
    {
        Vector3d mNormal;
        float mD;
    }


    정의상 삼각형은 단일 평면에 놓인다.
    삼각형이 주어지면 평면의 방정식 유도가 가능하다.
    삼각형을 구성하는 vertex로 2개의 벡터를 만들고, 이 두 벡터로
    외적을 해 법선 벡터를 구하면 평면의 법선을 구하는 것이다.
    평면의 임의의 한점은 알고 있으므로(삼각형 vertex 중 하나 선택) 이
    버텍스와 점으로 d값을 구할 수 있다.

    임의의 점 C와 평면 사이의최소 거리를 구하는 것은 선분에서 했던 작업과 유사하다.
    (스칼라 투영 사용했던 case 3)

    평면 n의 법선을 알고, 원점과 평면 사이의 거리 d도 안다.
    먼저 C와 법선 n의 내적을 통해 스칼라 투영을 계산한다. (거리)

    s = C * n

    그리고 d값에 이 스칼라 투영값을 빼면 그 결과는 C와 평면 간 부호 있는 거리 차가 된다.

    SignedDist = s - d = C * n - d

    음수값은 C가 평면 아래에 있다는 걸 의미하고(법선의 반대방향),
    양수값은 C가 평면 위에 있음을 의미한다.
    
*/

/*
// 평면
struct Plane
{
	Plane(const Vector3d& normal, float d);
	// 세 점으로 평면 생성
	Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
	// 부호가 있는 점과 평면의 거리 계산
	float SignedDist(const Vector3d& point) const;

	// 법선 벡터
	Vector3d mNormal;
	// 평면과 원점 사이의 부호 있는 최소 거리값
	float mD;
};

Plane::Plane(const Vector3d& normal, float d)
	:mNormal(normal), mD(d) {}

Plane::Plane(const Vector3d& a, const Vector3d& b, const Vector3d& c)
{
	// a에서 b, a에서 c로의 벡터를 계산
	Vector3d ab = b - a;
	Vector3d ac = c - a;
	// 법선 벡터를 얻기 위해 외적을 한 뒤 정규화
	mNormal = Vector3d::Cross(ab, ac);
	mNormal.Normalize();
	// d = -P dot n
	mD = -Vector3d::Dot(a, mNormal);
}

float Plane::SignedDist(const Vector3d& point) const
{
	return Vector3d::Dot(point, mNormal) - mD;
}
*/
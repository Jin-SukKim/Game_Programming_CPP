// 게임 객체 모델의 기본 클래스
class Actor {
public:
	// pure virtual function
	// actor를 갱신하기 위해 프레임마다 호출
	virtual void Update(float deltaTime);
	// Actor를 그리기 위해 프레임마다 호출
	virtual void Draw();
};
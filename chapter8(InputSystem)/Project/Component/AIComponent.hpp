class AIComponent : public Component {
public :
	AIComponent(class Actor* owner);

	// 현재 상태에 해당하는 갱신 함수를 호출
	void Update(float deltaTime) override;
	
	// 별도의 함수로 상태 기계의 전이를 다룰 수 있다.
	// 여러 갱신 함수는 이 함수를 호출해서 상태 전이를 초기화한다.
	void ChangeState(const std::string& name);

	// 새 상태를 맵에 추가한다.
	void RegisterState(class AIState* state);
private:
	// 상태의 이름과 AIState 인스턴스를 매핑한다.
	std::unordered_map<std::string, class AIState*> mStateMap;
	// AI의 현재 상태
	class AIState* mCurrentState;
};
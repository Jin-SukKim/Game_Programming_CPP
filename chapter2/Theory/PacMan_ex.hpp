#include "Actor.hpp"

// ���� ���� ����
class PacMan : public Actor {
public:
	void Update(float deltaTime) override;
	void Draw() override;
};
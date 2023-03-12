#include "Actor.hpp"

// ¸ð³î¸®½Ä °èÃþ ±¸Á¶
class PacMan : public Actor {
public:
	void Update(float deltaTime) override;
	void Draw() override;
};
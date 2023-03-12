// Component 구조 예시
#include <unordered_set>
class GameObject {
public:
	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);
private:
	std::unordered_set<Component*> mComponents;
};
// ���� ��ü ���� �⺻ Ŭ����
class Actor {
public:
	// pure virtual function
	// actor�� �����ϱ� ���� �����Ӹ��� ȣ��
	virtual void Update(float deltaTime);
	// Actor�� �׸��� ���� �����Ӹ��� ȣ��
	virtual void Draw();
};
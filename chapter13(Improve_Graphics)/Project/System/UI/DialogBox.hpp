class DialogBox : public UIScreen
{
public:
	// ������ OK�� Ŭ������ �� ����� �Լ��� �ؽ�Ʈ ���ڿ��� ���ڷ� �޴´�.
	DialogBox(class Game* gmae, const std::string& text, std::function<void()> onOK);
	~DialogBox();
};
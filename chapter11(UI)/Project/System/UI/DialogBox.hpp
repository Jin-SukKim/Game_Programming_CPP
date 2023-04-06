class DialogBox : public UIScreen
{
public:
	// 유저가 OK를 클릭했을 때 실행될 함수와 텍스트 문자열을 인자로 받는다.
	DialogBox(class Game* gmae, const std::string& text, std::function<void()> onOK);
	~DialogBox();
};
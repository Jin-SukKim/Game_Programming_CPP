class Button
{
public:
	// 생성자는 이름과 폰트, 콜백 함수, 위치/높이를 파라미터로 받는다.
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick, const Vector2d& pos,
		const Vector2d& dims);
	~Button();
	// 버튼의 이름ㅇ르 설정하고 텍스처를 생성
	void SetName(const std::string& name);

	// 점이 버튼 경계 안에 있다면 true를 반환
	bool ContainsPoint(const Vector2d pt) const;
	// 버튼을 클릭했을 경우 호출
	void OnClick();
	// Getter/Setter
	class Texture* GetNameTex() {
		return mNameTex;
	}
	const Vector2d& GetPosition() const {
		return mPosition;
	}
	void SetHighlighted(bool sel) {
		mHighlighted = sel;
	}
	bool GetHighlighted() const {
		return mHighlighted;
	}

private:
	std::function<void()> mOnClick;
	std::string mName;
	class Texture* mNameTex;
	class Font* mFont;
	Vector2d mPosition;
	Vector2d mDimensions;
	bool mHighlighted;
};
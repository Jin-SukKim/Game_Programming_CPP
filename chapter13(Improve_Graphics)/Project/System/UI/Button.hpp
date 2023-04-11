class Button
{
public:
	// �����ڴ� �̸��� ��Ʈ, �ݹ� �Լ�, ��ġ/���̸� �Ķ���ͷ� �޴´�.
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick, const Vector2d& pos,
		const Vector2d& dims);
	~Button();
	// ��ư�� �̸����� �����ϰ� �ؽ�ó�� ����
	void SetName(const std::string& name);

	// ���� ��ư ��� �ȿ� �ִٸ� true�� ��ȯ
	bool ContainsPoint(const Vector2d pt) const;
	// ��ư�� Ŭ������ ��� ȣ��
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
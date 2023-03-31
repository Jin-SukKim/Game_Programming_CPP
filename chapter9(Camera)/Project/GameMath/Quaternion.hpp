// ���� ���ʹϾ�, ���� 4d ���ͷ� ����� function�� �缱�����ش�.
class Quaternion {
public:
	// ���ʹϾ��� 4���� ���� ������.
	float x;
	float y;
	float z;
	float w;
	
	static const Quaternion Identity;

	Quaternion();

	// ���ʹϾ��� ������ ������ �����ϱ�
	explicit Quaternion(float inX, float inY, float inZ, float inW);
	// ��� �����κ��� ���ʹϾ� ���� (�� ���ʹ� �̹� ����ȭ�ߴٰ� ����)
	explicit Quaternion(const Vector3d& axis, float angle);
	
	// ���ʹϾ��� 4���� ���� �����ϱ�
	void Set(float inX, float inY, float inZ, float inW) {
		x = inX;
		y = inY;
		z = inZ;
		w = inW;
	}

	// �ӷ����Ҽ��� �� ���ʹϾ�, q^-1
	void Conjugate() {
		x *= -1.f;
		y *= -1.f;
		z *= -1.f;
	}

	// ���ʹϾ� ũ��^2
	float LengthSq() const;
	// ���ʹϾ� ũ��
	float Length() const;

	// ���ʹϾ� ����ȭ
	void Normalize();
	static Quaternion Normalize(const Quaternion& q);

	// ���� ����
	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f);
	
	// ����
	static float Dot(const Quaternion& a, const Quaternion& b);

	// ���� ���� ���� - ���� ��Ȯ�� ���� ����
	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f);

	// ���ʹϾ� ���� �ձ� (q�� ȸ���� �� p�� ȸ���� �׶󽺸� �� pq�� ���)
	static Quaternion Concatenate(const Quaternion& q, const Quaternion& p);
};
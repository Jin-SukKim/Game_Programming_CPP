class BoneTransform
{
public:
    // ���� ȸ������ �ڿ� ���� ȸ�� ������ ���� ���ʹϾ��� Ȱ��
    Quaternion mRotation;
    // ���� ��ġ�� ����
    Vector3d mTranslation;
    // ��ķ� ��ȯ, ��� �����͸� ����� ȸ�� ��İ� �̵� ����� ����� ���Ѵ�.
    Matrix4x4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& a, 
		const BoneTransform& b, float f);
};
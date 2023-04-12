class Skeleton
{
public:
	// ����� �ִ� �� ���� ����
	struct Bone
	{
		// ���� ���ε� ���� ��ȯ
		BoneTransform mLocalBindPose;
		// �� �̸�
		std::string mName;
		// �θ� �ε���
		int mParent;
	};
	// ���Ϸκ��� �ε�
	bool Load(const std::string& fileName);

	// Get �Լ���
	size_t GetNumBones() const {
		return mBones.size();
	}
	const Bone& GetBone(size_t idx) const {
		return mBones[idx];
	}
	const std::vector<Bone>& GetBones() const {
		return mBones;
	}
	const std::vector<Matrix4x4>& GetGlobalInvBindPoses() const {
		return mGlobalInvBindPoses;
	}
	const std::string& GetFileName() const { return mFileName; }
protected:
	// �� ���� ���� �ι��� ���ε� ���� ���
	// (���븦 �ε��� �� ȣ���)
	void ComputeGlobalInvBindPose();

private:
	// ���븦 �����ϴ� ��
	std::vector<Bone> mBones;
	// �� ���� ���� �ι��� ���ε� ����
	std::vector<Matrix4x4> mGlobalInvBindPoses;
	std::string mFileName;
};
class Skeleton
{
public:
	// 뼈대상에 있는 각 본의 정의
	struct Bone
	{
		// 로컬 바인드 포즈 변환
		BoneTransform mLocalBindPose;
		// 본 이름
		std::string mName;
		// 부모 인덱스
		int mParent;
	};
	// 파일로부터 로드
	bool Load(const std::string& fileName);

	// Get 함수들
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
	// 각 보의 전역 인버스 바인드 포즈 계싼
	// (뼈대를 로드할 떄 호출됨)
	void ComputeGlobalInvBindPose();

private:
	// 뼈대를 구성하는 본
	std::vector<Bone> mBones;
	// 각 본의 전역 인버스 바인드 포즈
	std::vector<Matrix4x4> mGlobalInvBindPoses;
	std::string mFileName;
};
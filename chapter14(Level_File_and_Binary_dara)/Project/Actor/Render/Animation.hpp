class Animation
{
public:
	bool Load(const std::string& fileName);

	size_t GetNumBones() const {
		return mNumBones;
	}
	size_t GetNumFrames() const {
		return mNumFrames;
	}
	float GetDuration() const {
		return mDuration;
	}
	float GetFrameDuration() const {
		return mFrameDuration;
	}

	// 애니메이션에서 특정 시간의 각 본에 대한
	// 현재 전역 포즈 행렬을 주어진 벡터에 채운다.
	void GetGlobalPoseAtTime(std::vector<Matrix4x4>& outPoses,
		const class Skeleton* inSkeleton, float inTime) const;

	const std::string& GetFileName() const { return mFileName; }

private:
	// 애니메이션을 위한 본의 수
	size_t mNumBones;
	// 애니메이션의 프레임 수
	size_t mNumFrames;
	// 애니메이션 재생 시간 (초 단위)
	float mDuration;
	// 애니메이션에서 각 프레임의 재생 시간
	float mFrameDuration;
	// 트랙의 각 프레임별 변환 정보
	// 외부 벡터의 인덱스는 본 인덱스이며, 내부 벡터의 인덱스는 프레임 인덱스다.
	std::vector<std::vector<BoneTransform>> mTracks;
	std::string mFileName;
};
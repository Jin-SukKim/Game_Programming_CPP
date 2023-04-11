#include "..\..\PreCompiled.hpp"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true), mSkeleton(nullptr) {}

void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// 세계 변환
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		// 팔렛트 행렬
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0],
			MAX_SKELETON_BONES);
		// 정반사 값 설정
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// 텍스처 활성화
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// 메쉬의 vertex array 활성화
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// 그리기
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		
		// 애니메이션 재생 시간을 초과하면 처음부터 다시 재생하도록 래핑한다.
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// 행렬 팔렛트 재계산
		ComputeMatrixPalette();
	}
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	// 전역 인버스 바인드 포즈 행렬
	const std::vector<Matrix4x4>& globalInvBindPoses = 
		mSkeleton->GetGlobalInvBindPoses();
	// 현재 전역 포즈 행렬
	std::vector<Matrix4x4> currentPoses;
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);

	// 각 본의 팔레트를 설정
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// 전역 인버스 바인드 포즈 행렬 * 현재 포즈 행렬
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)
{
	mAnimation = anim;
	mAnimTime = 0.f;
	mAnimPlayRate = playRate;

	if (!mAnimation) {
		return 0.f;
	}

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}
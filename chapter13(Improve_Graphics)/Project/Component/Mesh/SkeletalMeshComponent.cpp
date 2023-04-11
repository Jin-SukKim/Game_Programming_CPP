#include "..\..\PreCompiled.hpp"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true), mSkeleton(nullptr) {}

void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ���� ��ȯ
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		// �ȷ�Ʈ ���
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0],
			MAX_SKELETON_BONES);
		// ���ݻ� �� ����
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// �ؽ�ó Ȱ��ȭ
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// �޽��� vertex array Ȱ��ȭ
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// �׸���
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
		
		// �ִϸ��̼� ��� �ð��� �ʰ��ϸ� ó������ �ٽ� ����ϵ��� �����Ѵ�.
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// ��� �ȷ�Ʈ ����
		ComputeMatrixPalette();
	}
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	// ���� �ι��� ���ε� ���� ���
	const std::vector<Matrix4x4>& globalInvBindPoses = 
		mSkeleton->GetGlobalInvBindPoses();
	// ���� ���� ���� ���
	std::vector<Matrix4x4> currentPoses;
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);

	// �� ���� �ȷ�Ʈ�� ����
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// ���� �ι��� ���ε� ���� ��� * ���� ���� ���
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
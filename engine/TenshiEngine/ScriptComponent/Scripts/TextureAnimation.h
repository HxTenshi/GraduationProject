
#pragma once
#include "main.h"


class TextureAnimation :public IDllScriptComponent{
public:
	void Start()override;
	void Update()override;

	void ChangeAnimation(float time);
	void AlphaAnimation(float time);

private:
	SERIALIZE GameObject mAnimationAsset_1;
	SERIALIZE GameObject mAnimationAsset_2;
	SERIALIZE GameObject mAnimationAsset_3;

	//�A�j���[�V�����̐؂�ւ��p
	SERIALIZE float mChangeTime;
	float mNextTime;

	bool mIsEnd;
	//�A�j���[�V�������A���t�@�ŏ������߂̕ϐ�
	SERIALIZE float mTime;
	SERIALIZE float mWaitTime;
	SERIALIZE float mDeleteTime;
	SERIALIZE GameObject mEndAction;

	SERIALIZE bool mIsFadeAnimation;

	std::vector<GameObject> mList;
	int mIndex;
};
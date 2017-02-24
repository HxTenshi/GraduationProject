#include "TextureAnimation.h"
# include "h_standard.h"
# include "h_component.h"
# include "OutputAnimation.h"
# include "Game/Component/MaterialComponent.h"

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TextureAnimation::Start(){
	if (mAnimationAsset_1)mList.push_back(mAnimationAsset_1);
	if (mAnimationAsset_2)mList.push_back(mAnimationAsset_2);
	if (mAnimationAsset_3)mList.push_back(mAnimationAsset_3);
	mNextTime = mChangeTime;
	mIndex = 0;
}

//毎フレーム呼ばれます
void TextureAnimation::Update(){
	if (mList.size() == 0)return;

	if (mIsEnd)return;

	mTime += Hx::DeltaTime()->GetDeltaTime();
	ChangeAnimation(mTime);
	
	if (mTime < mWaitTime)return;
	AlphaAnimation(mTime);
	//終了時の処理
	if (mTime > mDeleteTime) {
		mIsEnd = true;
		
		if (!mEndAction)return;
		auto sender = OutputGimic::GetOutputGimic(mEndAction);
		if (sender)sender->OnStart(gameObject);
	}

}

void TextureAnimation::ChangeAnimation(float time)
{
	if (mTime > mNextTime) {
		for (auto i : mList) {
			i->Disable();
		}

		auto target = mList[mIndex];
		if (target) {
			target->Enable();
		}
		mIndex++;
		if (mIndex == mList.size())mIndex = 0;

		mNextTime = mTime + mChangeTime;
	}
}

void TextureAnimation::AlphaAnimation(float time)
{
	if (!mIsFadeAnimation)return;
	for (auto i : mList) {
		auto mat = i->GetComponent<MaterialComponent>();
		XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1.0f - (time / mDeleteTime));
		if(mat)mat->SetAlbedoColor(color);
	}
}

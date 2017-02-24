#include "ChildCheacker.h"
# include "OutputGimic.h"
# include "h_standard.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void ChildCheacker::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ChildCheacker::Start(){

}

//毎フレーム呼ばれます
void ChildCheacker::Update(){
	if (mIsCheack)return;
	//Hx::Debug()->Log("数 : " + std::to_string(IsChildCount()));
	if (IsChildCount() == 0) {
		mIsCheack = true;
		if (mIsDead) {
			//Hx::Debug()->Log("敵の小隊の全滅");
			Hx::DestroyObject(mDead);
		}
		if (mOutput) {
			auto sender = OutputGimic::GetOutputGimic(mOutput);
			if (sender)sender->OnStart(gameObject);
		}
	}

}

int ChildCheacker::IsChildCount()
{
	if (!mTarget) return false;
	return mTarget->mTransform->Children().size();
}

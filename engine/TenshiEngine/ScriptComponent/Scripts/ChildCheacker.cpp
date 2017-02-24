#include "ChildCheacker.h"
# include "OutputGimic.h"
# include "h_standard.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChildCheacker::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void ChildCheacker::Start(){

}

//���t���[���Ă΂�܂�
void ChildCheacker::Update(){
	if (mIsCheack)return;
	//Hx::Debug()->Log("�� : " + std::to_string(IsChildCount()));
	if (IsChildCount() == 0) {
		mIsCheack = true;
		if (mIsDead) {
			//Hx::Debug()->Log("�G�̏����̑S��");
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

#include "OutputTextUI.h"
# include "h_standard.h"
# include "PlayerController.h"
# include "UniqueObject.h"
#include "Game/Component/CharacterControllerComponent.h"
# include "Game\Component\TextComponent.h"
# include "Game\Component\MaterialComponent.h"
//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OutputTextUI::Start(){
	mTime = 0;
	mIsEnd = false;

	if (mTextUI) {
		auto text = mTextUI->GetComponent<TextComponent>();
		Hx::Debug()->Log("�o�̓e�L�X�g�ݒ�");
		if (text)text->ChangeText(mPutText);
	}
}

//���t���[���Ă΂�܂�
void OutputTextUI::Update(){
	if (mIsEnd)return;

	mTime += Hx::DeltaTime()->GetDeltaTime();
	//Hx::Debug()->Log(std::to_string(mTime));
	if (mTime < mWait)return;

	if (mTextUI) {
		auto mat = mTextUI->GetComponent<MaterialComponent>();
		//Hx::Debug()->Log(std::to_string(1.0f - mTime / mDeleteTime + mWait));
		XMFLOAT4 color = XMFLOAT4(0, 0, 0, 1.0f - (mTime / mDeleteTime));
		if(mat)mat->SetAlbedoColor(color);
		

	}

	if (mTime > mDeleteTime) {
		mIsEnd = true;
		if (!mEndAction)return;
		auto sender = OutputGimic::GetOutputGimic(mEndAction);
		if (sender)sender->OnStart(gameObject);
	}
}
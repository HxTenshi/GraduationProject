#include "WaveCounter.h"


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaveCounter::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void WaveCounter::Start(){
	mRound = 0;
}

//���t���[���Ă΂�܂�
void WaveCounter::Update(){
	mText->GetComponent<TextComponent>()->ChangeText("Wave : "+std::to_string(mRound) +" / "+std::to_string(mALLWave));
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void WaveCounter::Finish(){

}

void WaveCounter::AddCounter()
{
	mRound++;
}

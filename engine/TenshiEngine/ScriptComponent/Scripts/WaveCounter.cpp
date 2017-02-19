#include "WaveCounter.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void WaveCounter::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WaveCounter::Start(){
	mRound = 0;
}

//毎フレーム呼ばれます
void WaveCounter::Update(){
	mText->GetComponent<TextComponent>()->ChangeText("Wave : "+std::to_string(mRound) +" / "+std::to_string(mALLWave));
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WaveCounter::Finish(){

}

void WaveCounter::AddCounter()
{
	mRound++;
}

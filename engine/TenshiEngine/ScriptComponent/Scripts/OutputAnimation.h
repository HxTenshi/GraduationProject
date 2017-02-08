
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputAnimation :public OutputGimic {
public:
	void Update();
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	SERIALIZE GameObject m_Target;
	//有効化無効か
	SERIALIZE int mPlayAnimationID;
	SERIALIZE float mWeight;
	SERIALIZE float mTime;
	SERIALIZE float mTimeScale;
	SERIALIZE GameObject m_Output;

	SERIALIZE GameObject m_AnimationEndAction;
	bool mIsEnd;
};

#pragma once
#include "main.h"
#include "OutputGimic.h"

//イベント時の音再生
class OutputPlayAudio :public OutputGimic {
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	SERIALIZE GameObject m_PlayTarget;
	SERIALIZE GameObject m_OutputGimic;
};
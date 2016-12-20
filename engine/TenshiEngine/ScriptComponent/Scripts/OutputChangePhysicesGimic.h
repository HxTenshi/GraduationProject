
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputChangePhysicesGimic :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	SERIALIZE GameObject m_Target;
	//有効化無効か
	SERIALIZE bool mIsEnable;
	SERIALIZE GameObject m_Output;
};

#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputReisStartGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//メンバ変数
	SERIALIZE GameObject m_Reis;
};
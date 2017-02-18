
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputReisStartGimick :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE GameObject m_Reis;
};
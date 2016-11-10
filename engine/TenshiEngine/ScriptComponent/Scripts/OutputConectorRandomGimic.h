
#pragma once
#include "main.h"

#include "OutputGimic.h"

class OutputConectorRandomGimic :public OutputGimic {
public:

	bool OnStart(GameObject Sender);
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject m_Output_1;
	SERIALIZE GameObject m_Output_2;
	SERIALIZE GameObject m_Output_3;
	SERIALIZE GameObject m_Output_4;
	SERIALIZE GameObject m_Output_5;
	SERIALIZE GameObject m_Output_6;

	SERIALIZE float m_Output_1_Rate;
	SERIALIZE float m_Output_2_Rate;
	SERIALIZE float m_Output_3_Rate;
	SERIALIZE float m_Output_4_Rate;
	SERIALIZE float m_Output_5_Rate;
	SERIALIZE float m_Output_6_Rate;
};
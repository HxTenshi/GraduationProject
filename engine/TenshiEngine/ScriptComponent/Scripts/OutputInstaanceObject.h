
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutputInstaanceObject :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//�����o�ϐ�
	SERIALIZE PrefabAsset mInstance;
	SERIALIZE GameObject m_Output;

};
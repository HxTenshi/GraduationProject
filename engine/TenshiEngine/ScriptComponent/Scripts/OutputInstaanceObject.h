
#pragma once
#include "main.h"
# include "OutputGimic.h"

class OutputInstaanceObject :public OutputGimic {
public:
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE PrefabAsset mInstance;
	SERIALIZE GameObject m_Output;

};
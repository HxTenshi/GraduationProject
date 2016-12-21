
#pragma once
#include "main.h"

#include "OutputGimic.h"


class OutputConnectorStackGimic :public OutputGimic {
public:
	void Initialize()override;

	bool OnStart(GameObject Sender) override;
private:
	//メンバ変数


	SERIALIZE
		GameObject m_OutputGimic;

	SERIALIZE
	int m_MaxCount;
	SERIALIZE
	bool m_Loop;
	SERIALIZE
	int m_Count;
};
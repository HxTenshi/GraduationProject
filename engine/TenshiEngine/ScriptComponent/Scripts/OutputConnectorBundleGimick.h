
#pragma once
#include "main.h"
#include "OutputGimic.h"

class OutputConnectorBundleGimick :public OutputGimic {
public:
	void Initialize()override;
	bool OnStart(GameObject Sender) override;

private:
	//ƒƒ“ƒo•Ï”
};
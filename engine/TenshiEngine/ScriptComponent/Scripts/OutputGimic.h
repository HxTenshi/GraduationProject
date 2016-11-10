
#pragma once
#include "main.h"


class OutputGimic :public IDllScriptComponent{
public:
	virtual ~OutputGimic() {}
	virtual bool OnStart(GameObject Sender) { (void)Sender; return false; }
	
	static OutputGimic* GetOutputGimic(GameObject target);

private:
	//ƒƒ“ƒo•Ï”
};
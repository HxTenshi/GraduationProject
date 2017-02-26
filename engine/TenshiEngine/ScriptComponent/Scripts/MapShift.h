
#pragma once
#include "main.h"


class MapShift :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	
	static MapShift* GetMapShift(GameObject object);

private:
	//メンバ変数
};
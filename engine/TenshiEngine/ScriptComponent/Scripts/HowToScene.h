#pragma once
#include "main.h"
#include "h_standard.h"


class HowToScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//�����o�ϐ�
	SERIALIZE
	std::string configPass;
};
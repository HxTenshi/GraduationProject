
#pragma once
#include "main.h"


class GetEnemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	GameObject GetMinEnemy();

private:
	//�����o�ϐ�
	GameObject m_MinEnemy;
	GameObject m_MinEnemy_Back;
};

#pragma once
#include "main.h"
# include "WeaponContainer.h"
//Player����
class DemoScript :public IDllScriptComponent{
public:
	DemoScript();
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void TargetMove();

private:
	XMVECTOR GetThrowTarget();
	void Throw();
	GameObject SearchChildren(std::string name);
private:
	//�����o�ϐ�
	WeaponContainer* mWeaponContainer;
	GameObject mContainer;
};

#pragma once
#include "main.h"

//class DemoWeapon :public IDllScriptComponent,public Weapon {
class DemoWeapon : public IDllScriptComponent {
public:

	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void SetReleaseVector(GameObject player, XMVECTOR position);
private:
	bool mShoot;
	XMVECTOR mThrowVector;
};
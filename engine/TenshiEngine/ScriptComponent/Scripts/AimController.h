
#pragma once
#include "main.h"
#include "TPSCamera.h"

class AimController :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void ChangeAimMode(TPSCamera* camera,GameObject player, bool isFree);

	void PlayerRotate(TPSCamera* camera, GameObject player);

private:
	//メンバ変数
	SERIALIZE XMVECTOR mAimPoint;
	SERIALIZE XMVECTOR mDefoultPoint;
	SERIALIZE float mDefoultDamping;
	SERIALIZE float mDefoultStiffness;
	SERIALIZE float mDamping;
	SERIALIZE float mStiffness;

	//投げる用
	GameObject mWeaponHand;
	//UI
	GameObject mAimUISet;
	SERIALIZE
	GameObject mUIParent;
	GameObject mTargetUI;
};
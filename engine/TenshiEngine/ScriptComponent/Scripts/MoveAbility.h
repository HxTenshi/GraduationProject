
#pragma once
#include "main.h"
# include "Game/Component/CharacterControllerComponent.h"
# include "WeaponControl.h"
class PlayerController;
class MoveAbility :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void SetPoint(GameObject target, PlayerController* pc);
	void OnMove();

	void KnockBack(GameObject target);

	bool OnTargetDistance(XMVECTOR target);

private:
	//ÉÅÉìÉoïœêî
	PlayerController* mPC;
	GameObject mMovePoint;
	GameObject mMoveActor;
	bool onMove;
	float mTime;
	SERIALIZE float mSpeed;
	XMVECTOR mMoveStartPosition;
	XMVECTOR m_LastPos;
	bool m_MoveFirst;

	WeaponControl mWeaponControl;
};
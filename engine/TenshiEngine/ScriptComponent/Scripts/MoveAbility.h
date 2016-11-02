
#pragma once
#include "main.h"
# include "Game/Component/CharacterControllerComponent.h"

class MoveAbility :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void SetPoint(GameObject target, weak_ptr<CharacterControllerComponent> cc);
	void OnMove();

private:
	//ÉÅÉìÉoïœêî
	weak_ptr<CharacterControllerComponent> mCC;
	GameObject mMovePoint;
	GameObject mMoveActor;
	bool onMove;
	float mTime;
	SERIALIZE float mSpeed;
	XMVECTOR mMoveStartPosition;
};

#pragma once
#include "main.h"
#include <functional>

enum NextAttack {
	None,
	Low,
	High,
	Special,
};

struct AttackState {
	int ID;
	int NextLowID;
	int NextHighID;
	float AttackTime;
	float KoutyokuTime;
	float NextTime;
	float DamageScale;
	std::function<void(void)> AttackFunc;
};

class CharacterControllerComponent;
class PlayerController :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:

	void move();
	void moveUpdate();
	void rotate();
	void doge();
	void guard();
	void attack();
	void throwAway(GameObject target = NULL, bool isMove = false);

	//ÉÅÉìÉoïœêî

	SERIALIZE
	GameObject m_Camera;
	SERIALIZE
	float m_JumpPower;
	SERIALIZE
	float m_MoveSpeed;
	SERIALIZE
	GameObject m_WeaponHand;

	SERIALIZE GameObject mMoveAvility;

	XMVECTOR mJump;
	XMVECTOR mGravity;
	//å¸Ç≠Ç◊Ç´ï˚å¸
	XMVECTOR mVelocity;

	bool m_IsDoge;
	bool m_IsGround;

	weak_ptr<CharacterControllerComponent> m_CharacterControllerComponent;

	bool m_AttackMode;
	int m_NextAttack;
	AttackState m_CullentAttack;
	std::vector<AttackState> m_AttackStateList;
};
#pragma once
#include "main.h"
#include "Enemy.h"

class EnemyRezardMan : public Enemy{
public:
	EnemyRezardMan();
	void ChildInitialize() override;
	void SoloAction() override;
	ENEMY_TYPE GetEnemyType() override;
	float GetOnBattleRange() override;
	void Attack(GameObject player, COL_TYPE colType) override;
	bool Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_) override;
	bool DiscoveryPlayer() override;
	bool LostPlayer() override;
	void ChildFinalize() override;

private:
	enum ANIM_ID {
		ANIM_IDLE,
		ANIM_WALK_FORWARD,
		ANIM_BACKSTEP,
		ANIM_ATTACK_DOWN,
		ANIM_GUARD,
		ANIM_JUMPATTACK,
		ANIM_WINCE,
		ANIM_HITINGUARD,
		ANIM_ATTACK_MONCKEY,
		ANIM_PROVOCATION,
		ANIM_UPPERDOWN,
		ANIM_BEATDOWN,
		ANIM_UPPERDOWNAFTER,
		ANIM_BEATDOWNAFTER,
		ANIM_ATTACK_SIDE,
		ANIM_RUSH,
		ANIM_SIDESTEPLEFT,
		ANIM_SIDESTEPRIGHT,
		ANIM_THRUST,
		ANIM_THRUSTRUN,
	};
	//ÉÅÉìÉoïœêî
	SERIALIZE float m_TrackingSpeed;
	SERIALIZE float m_TrackingRange;
	SERIALIZE float m_LostRange;
	SERIALIZE float m_OnBattleRange;
	SERIALIZE float m_OffBattleRange;
	SERIALIZE float m_TrackingAngle;
	SERIALIZE float m_TrackingRotateSpeed;
	SERIALIZE float hp;
	SERIALIZE int m_AttackDamage;
	SERIALIZE GameObject m_MyWeapon;
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE GameObject m_MovePoints;
	SERIALIZE bool m_Child;
	SERIALIZE bool m_Archer;
	SERIALIZE float m_AproachRotateSpeed;
	SERIALIZE float m_CorrectionRotateSpeed;
	SERIALIZE bool m_DrawLog;
	SERIALIZE int m_HitInGuardMinCount;
	SERIALIZE int m_HitInGuardMaxCount;
	SERIALIZE int m_AbsolutelyAvoidInHitAttackProbability;
	SERIALIZE float APROACHMINTIME;
	SERIALIZE float APROACHMAXTIME;
	
	void TrackingModeInitilize();
	void TrackingModeUpdate();
	void TrackingModeFinalize();

	void BattleModeInitilize();
	void BattleModeUpdate();
	void BattleModeFinalize();

	void ParentTrackingModeInitilize();
	void ParentTrackingModeUpdate();
	void ParentTrackingModeFinalize();

	void ChildTrackingModeInitilize();
	void ChildTrackingModeUpdate();
	void ChildTrackingModeFinalize();

	void ConfrontModeInitilize();
	void ConfrontModeUpdate();
	void ConfrontModeFinalize();

	void ApproachModeInitilize();
	void ApproachModeUpdate();
	void ApproachModeFinalize();

	void AttackDownModeInitilize();
	void AttackDownModeUpdate();
	void AttackDownModeFinalize();

	void JumpAttackModeInitilize();
	void JumpAttackModeUpdate();
	void JumpAttackModeFinalize();

	void GuardModeInitilize();
	void GuardModeUpdate();
	void GuardModeFinalize();

	void ProvocationModeInitilize();
	void ProvocationModeUpdate();
	void ProvocationModeFinalize();

	void BackStepModeInitilize();
	void BackStepModeUpdate();
	void BackStepModeFinalize();

	void WinceModeInitilize();
	void WinceModeUpdate();
	void WinceModeFinalize();

	void UpperDownInitilize();
	void UpperDownUpdate();
	void UpperDownFinalize();

	void BeatDownInitilize();
	void BeatDownUpdate();
	void BeatDownFinalize();

	void DownInitilize();
	void DownUpdate();
	void DownFinalize();

	void HitInGuardModeInitilize();
	void HitInGuardModeUpdate();
	void HitInGuardModeFinalize();

	void Prowl();

	//ç°âÒÇÕâΩâÒâ‰ñùÇ∑ÇÈÇ©(m_HitInGuardMinCountÅ`m_HitInGuardMaxCount)
	int PatienceInThisTime;

	void AttackMonckeyModeInitilize();
	void AttackMonckeyModeUpdate();
	void AttackMonckeyModeFinalize();

	void DeadModeInitilize();
	void DeadModeUpdate();
	void DeadModeFinalize();

public:
	static BATTLEACTION::Enum GetChangeBattleAction(
		int guardProbability, int approachProbability,
		int backstepProbability, int attackProbability,
		int jumpAttackProbability, int provocationProbability
		);
};
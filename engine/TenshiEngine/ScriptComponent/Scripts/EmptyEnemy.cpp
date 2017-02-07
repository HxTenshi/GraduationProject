#include "EmptyEnemy.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"

EmptyEnemy::EmptyEnemy()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EmptyEnemy::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EmptyEnemy::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EmptyEnemy::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EmptyEnemy::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EmptyEnemy::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EmptyEnemy::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EmptyEnemy::TrackingMoveModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&EmptyEnemy::TrackingMoveModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EmptyEnemy::TrackingMoveModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EmptyEnemy::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EmptyEnemy::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EmptyEnemy::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EmptyEnemy::TackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK1ACTION] = std::bind(&EmptyEnemy::TackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EmptyEnemy::TackleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK3ACTION] = std::bind(&EmptyEnemy::RotateTackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK3ACTION] = std::bind(&EmptyEnemy::RotateTackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK3ACTION] = std::bind(&EmptyEnemy::RotateTackleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EmptyEnemy::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EmptyEnemy::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EmptyEnemy::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EmptyEnemy::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EmptyEnemy::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EmptyEnemy::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EmptyEnemy::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EmptyEnemy::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EmptyEnemy::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EmptyEnemy::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EmptyEnemy::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EmptyEnemy::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EmptyEnemy::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EmptyEnemy::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EmptyEnemy::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;

}
void EmptyEnemy::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
}

void EmptyEnemy::SoloAction()
{
	if (m_AccelVec.y >= 0 || m_BattleModeParam.id == BATTLEACTION::UPPERDOWNACTION || m_BattleModeParam.id == BATTLEACTION::DEADACTION) {
		m_Gravity = XMVectorSet(0, -9.8f, 0, 0);
	}
	else {
		m_Gravity = XMVectorSet(0, 0, 0, 0);
	}
	if (Input::Trigger(KeyCode::Key_1)) {
		Damage(1.0f, BATTLEACTION::WINCEACTION, XMVectorSet(0, 0, 0, 0));
	}
	if (Input::Trigger(KeyCode::Key_2)) {
		Damage(1.0f, BATTLEACTION::UPPERDOWNACTION, XMVectorSet(0, 10, 0, 0));
	}
	if (Input::Trigger(KeyCode::Key_3)) {
		Damage(1.0f, BATTLEACTION::BEATDOWNACTION, XMVectorSet(0, -10, 0, 0));
	}
}

ENEMY_TYPE EmptyEnemy::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EmptyEnemy::GetOnBattleRange()
{
	return 0.0f;
}

void EmptyEnemy::Attack(GameObject player, COL_TYPE colType)
{

}

bool EmptyEnemy::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
		return true;
	}
	return false;
}

bool EmptyEnemy::DiscoveryPlayer()
{

	return false;
}

bool EmptyEnemy::LostPlayer()
{
	return false;
}

void EmptyEnemy::ChildFinalize()
{
	if (m_Hp < 0) {
		Hx::DestroyObject(this->gameObject);
	}
}

void EmptyEnemy::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EmptyEnemy::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EmptyEnemy::TrackingModeFinalize()
{
}

void EmptyEnemy::TrackingMoveModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
}

void EmptyEnemy::TrackingMoveModeUpdate()
{
	if (m_objective_flag) {
		hp = m_MaxHp;
	}
}

void EmptyEnemy::TrackingMoveModeFinalize()
{
}

void EmptyEnemy::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EmptyEnemy::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
}

void EmptyEnemy::BattleModeFinalize()
{
}

void EmptyEnemy::ConfrontModeInitilize()
{
}

void EmptyEnemy::ConfrontModeUpdate()
{
	
}

void EmptyEnemy::ConfrontModeFinalize()
{
}

void EmptyEnemy::TackleModeInitilize()
{
	
}

void EmptyEnemy::TackleModeUpdate()
{
	
}

void EmptyEnemy::TackleModeFinalize()
{

}

void EmptyEnemy::RotateTackleModeInitilize()
{

}

void EmptyEnemy::RotateTackleModeUpdate()
{
}

void EmptyEnemy::RotateTackleModeFinalize()
{
}

void EmptyEnemy::WinceModeInitilize() {
}

void EmptyEnemy::WinceModeUpdate() {
}

void EmptyEnemy::WinceModeFinalize() {
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EmptyEnemy::UpperDownInitilize()
{
}

void EmptyEnemy::UpperDownUpdate()
{
}

void EmptyEnemy::UpperDownFinalize()
{

}

void EmptyEnemy::BeatDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += m_Accel;
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
}

void EmptyEnemy::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}

}

void EmptyEnemy::BeatDownFinalize()
{
}

void EmptyEnemy::DownInitilize()
{
}

void EmptyEnemy::DownUpdate()
{}

void EmptyEnemy::DownFinalize()
{
}

void EmptyEnemy::DeadInitilize()
{
}

void EmptyEnemy::DeadUpdate()
{
}

void EmptyEnemy::DeadFinalize()
{
}

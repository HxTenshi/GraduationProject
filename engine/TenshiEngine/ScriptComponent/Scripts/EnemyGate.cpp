#include "EnemyGate.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"
#include "OutputGimic.h"
# include "OutputAnimation.h"

EnemyGate::EnemyGate()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyGate::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyGate::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyGate::TrackingModeFinalize, this);

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;

}
void EnemyGate::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
}

void EnemyGate::SoloAction()
{
}

ENEMY_TYPE EnemyGate::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemyGate::GetOnBattleRange()
{
	return 0.0f;
}

void EnemyGate::Attack(GameObject player, COL_TYPE colType)
{

}

bool EnemyGate::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	Hx::Debug()->Log(std::to_string(damage_));
	m_Hp -= damage_;

	if (m_Hp <= 0) {
		auto scr = OutputGimic::GetOutputGimic(gameObject->mTransform->GetParent());
		if (!scr)return false;
		scr->OnStart(gameObject);
		mGate->GetScript<OutputAnimation>()->SetBreak(true);
		Hx::DestroyObject(this->gameObject);
	}
	return false;
}

bool EnemyGate::DiscoveryPlayer()
{

	return false;
}

bool EnemyGate::LostPlayer()
{
	return false;
}

void EnemyGate::ChildFinalize()
{
}

void EnemyGate::TrackingModeInitilize()
{
}

void EnemyGate::TrackingModeUpdate()
{
}

void EnemyGate::TrackingModeFinalize()
{
}
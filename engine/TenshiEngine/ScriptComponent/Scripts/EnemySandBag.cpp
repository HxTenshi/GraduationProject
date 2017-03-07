#include "EnemySandBag.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"
#include "Score.h"
#include "SoundManager.h"
#include "EnemyManager.h"
#include "OutputGimic.h"
#include "hintDraw.h"
#include "ObjectGenerator.h"
#include "UniqueObject.h"
#include "Tutrial_1_5_Program.h"

EnemySandBag::EnemySandBag()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemySandBag::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemySandBag::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemySandBag::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemySandBag::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemySandBag::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemySandBag::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemySandBag::TrackingMoveModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemySandBag::TrackingMoveModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemySandBag::TrackingMoveModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemySandBag::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemySandBag::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemySandBag::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EnemySandBag::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EnemySandBag::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EnemySandBag::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemySandBag::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemySandBag::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemySandBag::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemySandBag::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemySandBag::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemySandBag::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EnemySandBag::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EnemySandBag::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EnemySandBag::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemySandBag::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemySandBag::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemySandBag::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::BATTLEMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
	m_UpperdownNow = false;
	m_Attacked = false;
}
void EnemySandBag::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
	m_StartForward = gameObject->mTransform->Forward();
	ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
	m_StartPos = gameObject->mTransform->WorldPosition();
	m_FirstFrame = true;
}

void EnemySandBag::SoloAction()
{
	if (m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION && !m_NotHeal) {
		m_Hp = hp;
	}

	if (m_FirstFrame) {
		Score::AddCountEnemy();
		EnemyManager::EnemyPush(gameObject);
		m_FirstFrame = false;
	}
}

ENEMY_TYPE EnemySandBag::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemySandBag::GetOnBattleRange()
{
	return 0.0f;
}

void EnemySandBag::Attack(GameObject player, COL_TYPE colType)
{
	if (m_AttackHit)return;
	if (!player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
	if (m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::UPPERDOWNACTION &&
		m_BattleModeParam.id != BATTLEACTION::BEATDOWNACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION) {
		m_WinceBeforeId = m_BattleModeParam.id;
	}
	m_Attacked = true;
	if (!m_RotateEnd) {
		if (colType == COL_TYPE::NORMAL && m_BattleModeParam.id == BATTLEACTION::ATTACK1ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
		}
		else if (colType == COL_TYPE::ROTATE && m_BattleModeParam.id == BATTLEACTION::ATTACK3ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		}
	}
}

bool EnemySandBag::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	if (m_Damage > 80)m_Damage = 10000;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
		return true;
	}
	return false;
}

bool EnemySandBag::DiscoveryPlayer()
{
	return false;
}

bool EnemySandBag::LostPlayer()
{

	return false;
}

void EnemySandBag::ChildFinalize()
{
	////gameObject->Disable();
	Score::AddScore();
	EnemyManager::EnemyErase(gameObject);
	Hx::Debug()->Log(gameObject->Name());
	Hx::DestroyObject(this->gameObject);
}

void EnemySandBag::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EnemySandBag::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EnemySandBag::TrackingModeFinalize()
{
}

void EnemySandBag::TrackingMoveModeInitilize()
{
}

void EnemySandBag::TrackingMoveModeUpdate()
{
}

void EnemySandBag::TrackingMoveModeFinalize()
{
}

void EnemySandBag::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EnemySandBag::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
}

void EnemySandBag::BattleModeFinalize()
{
}

void EnemySandBag::ConfrontModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_IDLE, 10.0f);
	m_UpperdownNow = false;
}

void EnemySandBag::ConfrontModeUpdate()
{
	LookPosition(gameObject->mTransform->WorldPosition() + m_StartForward, m_RotateSpeed);
	auto playerPos = m_Player->mTransform->WorldPosition();
	auto myPos = gameObject->mTransform->WorldPosition();
	m_Forward = gameObject->mTransform->Forward();
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(playerPos - myPos)).x, -1.0f, 1.0f));
}

void EnemySandBag::ConfrontModeFinalize()
{
}

void EnemySandBag::WinceModeInitilize() {
	m_Hp -= m_Damage;
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true, true);

	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!cc->IsGround()) {
		m_AccelVec = m_Accel;
	}
	m_WinceCount = m_WinceTime;
}

void EnemySandBag::WinceModeUpdate() {
	m_WinceCount -= Hx::DeltaTime()->GetDeltaTime();
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.afterAnimId) && m_WinceCount < 0) {
		if (!m_UpperdownNow)
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
		else {
			ChangeBattleAction(BATTLEACTION::UPPERDOWNACTION);
			m_Damage = 0;
			m_Accel = XMVectorSet(0, 0, 0, 0);
		}
	};
}

void EnemySandBag::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemySandBag::UpperDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	if (!m_UpperdownNow)
		AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_AccelVec = m_Accel;
	m_UpperdownNow = true;
}

void EnemySandBag::UpperDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (cc->IsGround() && m_AccelVec.y <= 0) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
}

void EnemySandBag::UpperDownFinalize()
{

}

void EnemySandBag::BeatDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += m_Accel;
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
}

void EnemySandBag::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}

}

void EnemySandBag::BeatDownFinalize()
{
}

void EnemySandBag::DownInitilize()
{
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_TackleStartPos = gameObject->mTransform->Forward();//
	m_TackleStartPos.y = 0;
	m_TackleStartPos += gameObject->mTransform->WorldPosition();
}

void EnemySandBag::DownUpdate()
{
	LookPosition(m_TackleStartPos, m_RotateSpeed);
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		ChangeBattleAction(BATTLEACTION::APPROACHACTION);
	};
}

void EnemySandBag::DownFinalize()
{
}

void EnemySandBag::DeadInitilize()
{
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += XMVectorSet(0, -10, 0, 0);
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_DeadIsGround = false;
	gameObject->RemoveComponent<CharacterControllerComponent>();
}

void EnemySandBag::DeadUpdate()
{

	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(ANIM_ID::ANIM_DOWN)) {
		if (!m_Isend) {
			hintDraw::OnStart_(gameObject);
			auto mogitou = m_Mogitou->GetScript<ObjectGenerator>()->GetGeneratorObject();
			UniqueObject::GetPlayer()->GetScript<PlayerController>()->SetSpecial(100);
			Hx::DestroyObject(m_Mogitou);
			Hx::DestroyObject(mogitou);
			m_Tutrial_1_5_->GetScript<Tutrial_1_5_Program>()->DestroySpecialBotton();
			Hx::DestroyObject(m_Tutrial_1_5_);
		}
		m_Isend = true;
	}
}

void EnemySandBag::DeadFinalize()
{
}
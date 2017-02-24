#include "EnemyArcher.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"
#include "EnemyArrow.h"
#include "Score.h"
#include "UniqueObject.h"

EnemyArcher::EnemyArcher()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyArcher::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyArcher::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyArcher::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyArcher::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyArcher::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyArcher::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::CHILDTRACKING] = std::bind(&EnemyArcher::ChildTrackingModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::CHILDTRACKING] = std::bind(&EnemyArcher::ChildTrackingModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::CHILDTRACKING] = std::bind(&EnemyArcher::ChildTrackingModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyArcher::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyArcher::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyArcher::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyArcher::ChargeModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyArcher::ChargeModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyArcher::ChargeModeFinalize, this);

	battleActionInitilize[BATTLEACTION::SHOTACTION] = std::bind(&EnemyArcher::ShotModeInitilize, this);
	battleActionUpdate[BATTLEACTION::SHOTACTION] = std::bind(&EnemyArcher::ShotModeUpdate, this);
	battleActionFinalize[BATTLEACTION::SHOTACTION] = std::bind(&EnemyArcher::ShotModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyArcher::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EnemyArcher::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyArcher::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyArcher::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyArcher::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyArcher::BackStepModeFinalize, this);

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyArcher::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyArcher::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyArcher::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyArcher::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyArcher::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyArcher::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyArcher::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EnemyArcher::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyArcher::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyArcher::DeadModeInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyArcher::DeadModeUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyArcher::DeadModeFinalize, this);
}

void EnemyArcher::ChildInitialize()
{
	Score::AddCountEnemy();
	ModelObject = m_ModelObject;
	m_MaxHp = hp;
	m_Hp = hp;
	if (!m_Child) {
		if (!m_MovePoints)return;
	}
	ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::CHILDTRACKING);
}

void EnemyArcher::SoloAction()
{
}

void EnemyArcher::ChildFinalize()
{
	Score::AddScore();
	////gameObject->Disable();
	Hx::Debug()->Log(gameObject->Name());
	Hx::DestroyObject(this->gameObject);
}

ENEMY_TYPE EnemyArcher::GetEnemyType()
{
	ENEMY_TYPE enemy_type = ENEMY_TYPE::PARENT_ARCHER;
	if (m_Child) {
		enemy_type = ENEMY_TYPE::CHILD_ARCHER;
	}

	return enemy_type;
}

float EnemyArcher::GetOnBattleRange()
{
	return m_OnBattleRange;
}

/****************************************************捜索時の処理**********************************************************/
void EnemyArcher::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EnemyArcher::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EnemyArcher::TrackingModeFinalize()
{
	trackingActionFinalize[m_TrackingModeParam.id]();
	//m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	//ChangeActionMode(ACTIONMODE::BATTLEMODE);
}

void EnemyArcher::ChildTrackingModeInitilize()
{
}

void EnemyArcher::ChildTrackingModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
}

void EnemyArcher::ChildTrackingModeFinalize()
{
}

void EnemyArcher::ConfrontModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
	m_BattleModeParam.count = 0.0f;
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)((APROACHMAXTIME - APROACHMINTIME) * 100)) / 100.0f) + APROACHMINTIME;
}

void EnemyArcher::ConfrontModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = true;
	LookPosition(m_Player->mTransform->WorldPosition(), 400);
	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		ChangeBattleAction(BATTLEACTION::ATTACK1ACTION);
		m_BattleModeParam.count = 0.0f;
	}
}

void EnemyArcher::ConfrontModeFinalize()
{
}

void EnemyArcher::ChargeModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_CHARGE, 5.0f, false, true);
}

void EnemyArcher::ChargeModeUpdate()
{
	LookPosition(m_Player->mTransform->WorldPosition(), 400);
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(ANIM_ID::ANIM_CHARGE)) {
		ChangeBattleAction(BATTLEACTION::SHOTACTION);
	}
}

void EnemyArcher::ChargeModeFinalize()
{
}

void EnemyArcher::ShotModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_SHOT, 5.0f, false, true);
	auto arrow = Hx::Instance(ArrowPrefab);
	if (!arrow)return;
	auto aa = this->gameObject->mTransform->WorldPosition() + XMVectorSet(0, 1.5f, 0, 0) +
		XMVector3Normalize(gameObject->mTransform->Left()) * 0.5f;
	arrow->mTransform->WorldPosition(aa);
	auto arrowScript = arrow->GetScript<EnemyArrow>();
	if (!arrowScript)return;
	arrowScript->SetEnemy(this->gameObject);
	arrowScript->SetVec(XMVector3Normalize(m_Player->mTransform->WorldPosition() + XMVectorSet(0, 0.5f, 0, 0) - aa));
}

void EnemyArcher::ShotModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(ANIM_ID::ANIM_SHOT)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	};
}

void EnemyArcher::ShotModeFinalize()
{
}

void EnemyArcher::BackStepModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_BACKSTEP, 5.0f, false, true);
}

void EnemyArcher::BackStepModeUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		LookPosition(m_PlayerVec, m_CorrectionRotateSpeed);
	}

	if (GetNowAnimTime() < 12.5f) {
		m_Vec -= m_Forward * 20.0f;
	}
	else {
		ChangeBattleAction(BATTLEACTION::ATTACK1ACTION);
	}
}

void EnemyArcher::BackStepModeFinalize()
{
}

/****************************************************戦闘時の処理**********************************************************/
void EnemyArcher::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EnemyArcher::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
}

void EnemyArcher::BattleModeFinalize()
{
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	ChangeActionMode(ACTIONMODE::TRACKINGMODE);
}

void EnemyArcher::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_DrawLog)
		Hx::Debug()->Log(std::to_string(m_Damage) + "ダメージ喰らった。残りの体力は" + std::to_string(m_Hp));
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}

	m_ManyHit++;
	if (m_ManyHit >= 8) {
		m_ManyHit = 0;
		ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true,true);

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!cc->IsGround()) {
		m_AccelVec = m_Accel;
	}
}

void EnemyArcher::WinceModeUpdate() {
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		m_ManyHit = 0;
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	};
}

void EnemyArcher::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyArcher::UpperDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_DrawLog)
		Hx::Debug()->Log(std::to_string(m_Damage) + "ダメージ喰らった。残りの体力は" + std::to_string(m_Hp));
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
	m_AccelVec = m_Accel;
}

void EnemyArcher::UpperDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (cc->IsGround() && m_AccelVec.y <= 0) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
}

void EnemyArcher::UpperDownFinalize()
{

}

void EnemyArcher::BeatDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_DrawLog)
		Hx::Debug()->Log(std::to_string(m_Damage) + "ダメージ喰らった。残りの体力は" + std::to_string(m_Hp));
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += m_Accel;
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
}

void EnemyArcher::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}

}

void EnemyArcher::BeatDownFinalize()
{
}

void EnemyArcher::DownInitilize()
{
	if (m_BattleModeParam.beforeId == BATTLEACTION::UPPERDOWNACTION) {
		AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
	}
	else {
		AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
	}
}

void EnemyArcher::DownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	};
}

void EnemyArcher::DownFinalize()
{
}

void EnemyArcher::DeadModeInitilize()
{
	if (m_DrawLog) {
		Hx::Debug()->Log("死んだ");
	}

	AnimChange(ANIM_ID::ANIM_DEAD, 5.0f, false, true);
	gameObject->RemoveComponent<CharacterControllerComponent>();
}

void EnemyArcher::DeadModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(ANIM_ID::ANIM_DEAD)) {
		m_Isend = true;
	};
}

void EnemyArcher::DeadModeFinalize()
{
}

//攻撃するときに呼ぶ処理

void EnemyArcher::Attack(GameObject player, COL_TYPE colType)
{
	if (m_DrawLog)
		Hx::Debug()->Log("何かに武器が当たった");
	if (m_DrawLog)
		Hx::Debug()->Log("m_Playerに攻撃がHit");
	if (!player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
	playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
}

/****************************************************ダメージの処理********************************************************/
bool EnemyArcher::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::BACKSTEPACTION) {
		if (m_ActionModeID == ACTIONMODE::BATTLEMODE) {
			if (m_BattleModeParam.id == BATTLEACTION::GUARDACTION || m_BattleModeParam.id == BATTLEACTION::HITINGUARDACTION) {
				ChangeBattleAction(BATTLEACTION::HITINGUARDACTION);
				return false;
			}
			else {
				ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
				return true;
			}
		}
		else {
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
			m_WasAttacked = true;
			return true;
		}
	}
	return false;
}

//敵を発見したか
bool EnemyArcher::DiscoveryPlayer()
{
	if (!m_Player) {
		Hx::Debug()->Log("PlayerのGameObjectがいないよ");
		return false;
	}
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();

	m_Forward = XMVector3Normalize(gameObject->mTransform->Forward());
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	m_Forward.y = 0.0f;
	m_PlayerVec.y = 0.0f;
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x, -1.0f, 1.0f));
	if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;
	if ((XMVector3Length(m_PlayerVec).x < m_TrackingRange && m_View / 3.14f * 180.0f < m_TrackingAngle)) {
		if (m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION)
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
		return true;
	}

	return false;
}

//敵を見失ったか
bool EnemyArcher::LostPlayer()
{
	if (!m_Player) {
		if (m_DrawLog)Hx::Debug()->Log("PlayerのGameObjectがいないよ");
		return false;
	}
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(m_PlayerVec).x > m_LostRange) {
		if (m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION)
			ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::CHILDTRACKING);
		return true;
	}
	return false;
}
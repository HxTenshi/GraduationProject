#include "EnemyWoodMan.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"

EnemyWoodMan::EnemyWoodMan()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyWoodMan::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyWoodMan::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyWoodMan::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyWoodMan::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyWoodMan::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyWoodMan::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyWoodMan::TrackingMoveModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyWoodMan::TrackingMoveModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyWoodMan::TrackingMoveModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyWoodMan::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyWoodMan::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyWoodMan::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyWoodMan::RunAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyWoodMan::RunAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyWoodMan::RunAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::APPROACHACTION] = std::bind(&EnemyWoodMan::ComeBackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::APPROACHACTION] = std::bind(&EnemyWoodMan::ComeBackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::APPROACHACTION] = std::bind(&EnemyWoodMan::ComeBackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyWoodMan::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EnemyWoodMan::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyWoodMan::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyWoodMan::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyWoodMan::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyWoodMan::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyWoodMan::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyWoodMan::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyWoodMan::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyWoodMan::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EnemyWoodMan::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyWoodMan::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyWoodMan::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyWoodMan::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyWoodMan::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::BATTLEMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
	m_UpperdownNow = false;
	m_Attacked = false;
}
void EnemyWoodMan::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
	m_StartForward = gameObject->mTransform->Forward();
	m_StartPos = gameObject->mTransform->WorldPosition();
	ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
}

void EnemyWoodMan::SoloAction()
{
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

ENEMY_TYPE EnemyWoodMan::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemyWoodMan::GetOnBattleRange()
{
	return 0.0f;
}

void EnemyWoodMan::Attack(GameObject player, COL_TYPE colType)
{
	if (!player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
	if (m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::UPPERDOWNACTION &&
		m_BattleModeParam.id != BATTLEACTION::BEATDOWNACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION) {
		m_WinceBeforeId = m_BattleModeParam.id;
	}
	m_Attacked = true;
	if (!m_RotateEnd) {
		if (colType == COL_TYPE::NORMAL && m_BattleModeParam.id == BATTLEACTION::ATTACKDOWNACTION) {
			playerScript->Damage(1.0f, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
		}
		else if (colType == COL_TYPE::ROTATE && m_BattleModeParam.id == BATTLEACTION::ATTACKMONCKEYACTION) {
			playerScript->Damage(1.0f, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		}
	}
}

bool EnemyWoodMan::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
		return true;
	}
	return false;
}

bool EnemyWoodMan::DiscoveryPlayer()
{
	return false;
}

bool EnemyWoodMan::LostPlayer()
{
	
	return false;
}

void EnemyWoodMan::ChildFinalize()
{
	gameObject->RemoveComponent<CharacterControllerComponent>();
	////gameObject->Disable();
	Hx::Debug()->Log(gameObject->Name());
	Hx::DestroyObject(this->gameObject);
}

void EnemyWoodMan::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EnemyWoodMan::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EnemyWoodMan::TrackingModeFinalize()
{
}

void EnemyWoodMan::TrackingMoveModeInitilize()
{
}

void EnemyWoodMan::TrackingMoveModeUpdate()
{
}

void EnemyWoodMan::TrackingMoveModeFinalize()
{
}

void EnemyWoodMan::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EnemyWoodMan::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
}

void EnemyWoodMan::BattleModeFinalize()
{
}

void EnemyWoodMan::ConfrontModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_IDLE, 10.0f);
	m_UpperdownNow = false;
}

void EnemyWoodMan::ConfrontModeUpdate()
{	
	LookPosition(gameObject->mTransform->WorldPosition() + m_StartForward);
	auto playerPos = m_Player->mTransform->WorldPosition();
	auto myPos = gameObject->mTransform->WorldPosition();
	m_Forward = gameObject->mTransform->Forward();
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(playerPos - myPos)).x, -1.0f, 1.0f));
	if (XMVector3Length(myPos - playerPos).x < m_TrackingRange && (m_View / 3.14f * 180.0f < m_TrackingAngle)) {
		ChangeBattleAction(BATTLEACTION::ATTACKDOWNACTION);
	}
}

void EnemyWoodMan::ConfrontModeFinalize()
{
}

void EnemyWoodMan::RunAttackModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_RUNATTACK, 10.0f);
	m_Attacked = false;

}

void EnemyWoodMan::RunAttackModeUpdate()
{
	auto pPos = m_Player->mTransform->WorldPosition();
	LookPosition(pPos);
	m_Vec += XMVector3Normalize(gameObject->mTransform->Forward()) * m_TackleSpeed;

	if (XMVector3Length(gameObject->mTransform->WorldPosition() - m_StartPos).x > m_LostRange || m_Attacked) {
		ChangeBattleAction(BATTLEACTION::APPROACHACTION);
	}
}

void EnemyWoodMan::RunAttackModeFinalize()
{
}

void EnemyWoodMan::ComeBackModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
}

void EnemyWoodMan::ComeBackModeUpdate()
{
	LookPosition(m_StartPos);
	m_Vec += XMVector3Normalize(m_StartPos - gameObject->mTransform->WorldPosition()) * m_TackleSpeed;
	auto groundPos = gameObject->mTransform->WorldPosition();
	auto groundStartPos = m_StartPos;
	groundPos.y = 0;
	groundStartPos.y = 0;
	if (XMVector3Length(groundPos-groundStartPos).x < 1.0f) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	}
}

void EnemyWoodMan::ComeBackModeFinalize()
{
}

void EnemyWoodMan::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!cc->IsGround()) {
		m_AccelVec = m_Accel;
	}
	m_WinceCount = m_WinceTime;
}

void EnemyWoodMan::WinceModeUpdate() {
	m_WinceCount -= Hx::DeltaTime()->GetDeltaTime();
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.nowAnimId) && m_WinceCount < 0) {
		if (!m_UpperdownNow)
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, m_WinceBeforeId);
		else {
			ChangeBattleAction(BATTLEACTION::UPPERDOWNACTION);
			m_Damage = 0;
			m_Accel = XMVectorSet(0, 0, 0, 0);
		}
	};
}

void EnemyWoodMan::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyWoodMan::UpperDownInitilize()
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

void EnemyWoodMan::UpperDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (cc->IsGround() && m_AccelVec.y <= 0) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
}

void EnemyWoodMan::UpperDownFinalize()
{

}

void EnemyWoodMan::BeatDownInitilize()
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

void EnemyWoodMan::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}

}

void EnemyWoodMan::BeatDownFinalize()
{
}

void EnemyWoodMan::DownInitilize()
{
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_TackleStartPos = gameObject->mTransform->Forward();//
	m_TackleStartPos.y = 0;
	m_TackleStartPos += gameObject->mTransform->WorldPosition();
}

void EnemyWoodMan::DownUpdate()
{
	LookPosition(m_TackleStartPos);
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	};
}

void EnemyWoodMan::DownFinalize()
{
}

void EnemyWoodMan::DeadInitilize()
{
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += XMVectorSet(0, -10, 0, 0);
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_DeadIsGround = false;
}

void EnemyWoodMan::DeadUpdate()
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (m_DeadIsGround) {
		LookPosition(m_TackleStartPos);

		if (anim->IsAnimationEnd(ANIM_ID::ANIM_DOWN)) {
			m_Isend = true;
		}
	}
	else {
		if (cc->IsGround()) {
			AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
			m_TackleStartPos = gameObject->mTransform->Forward();//
			m_TackleStartPos.y = 0;
			m_TackleStartPos += gameObject->mTransform->WorldPosition();
			m_DeadIsGround = true;
		}
	}
}

void EnemyWoodMan::DeadFinalize()
{
}

void EnemyWoodMan::LookPosition(XMVECTOR position_)
{
	auto myPos = gameObject->mTransform->WorldPosition();
	auto moveVec = position_;
	auto naviVec = XMVector3Normalize(moveVec - myPos);
	naviVec.y = 0;
	m_Forward = gameObject->mTransform->Forward();
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
	if (m_View < 0.1f)m_View = 0.0f;
	auto trackingNowAngle = m_RotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < trackingNowAngle)
		trackingNowAngle = m_View;
	gameObject->mTransform->WorldQuaternion(
		XMQuaternionMultiply(gameObject->mTransform->WorldQuaternion(), XMQuaternionRotationAxis(cross, trackingNowAngle)));
	auto myAngle = gameObject->mTransform->Rotate();
	myAngle.z = 0;
	gameObject->mTransform->Rotate(myAngle);
}
#include "EnemyEbilEye.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"

EnemyEbilEye::EnemyEbilEye()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyEbilEye::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyEbilEye::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyEbilEye::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyEbilEye::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyEbilEye::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyEbilEye::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyEbilEye::TrackingMoveModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyEbilEye::TrackingMoveModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyEbilEye::TrackingMoveModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyEbilEye::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyEbilEye::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyEbilEye::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyEbilEye::TackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyEbilEye::TackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyEbilEye::TackleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK3ACTION] = std::bind(&EnemyEbilEye::RotateTackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK3ACTION] = std::bind(&EnemyEbilEye::RotateTackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK3ACTION] = std::bind(&EnemyEbilEye::RotateTackleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyEbilEye::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EnemyEbilEye::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyEbilEye::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyEbilEye::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyEbilEye::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyEbilEye::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyEbilEye::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyEbilEye::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyEbilEye::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyEbilEye::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EnemyEbilEye::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyEbilEye::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyEbilEye::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyEbilEye::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyEbilEye::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
	m_UpperdownNow = false;
	m_RotateOpp = false;
}
void EnemyEbilEye::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
	m_MovePositionCenter = gameObject->mTransform->WorldPosition();
}

void EnemyEbilEye::SoloAction()
{
	if (m_AccelVec.y >= 0 || m_BattleModeParam.id == BATTLEACTION::UPPERDOWNACTION || m_BattleModeParam.id == BATTLEACTION::DEADACTION) {
		m_Gravity = XMVectorSet(0, -9.8f, 0, 0);
	}
	else {
		m_Gravity = XMVectorSet(0, 0, 0, 0);
	}
	if (Input::Trigger(KeyCode::Key_1)) {
		Damage(1.0f, BATTLEACTION::WINCEACTION,XMVectorSet(0,0,0,0));
	}
	if (Input::Trigger(KeyCode::Key_2)) {
		Damage(1.0f, BATTLEACTION::UPPERDOWNACTION, XMVectorSet(0, 10, 0, 0));
	}
	if (Input::Trigger(KeyCode::Key_3)) {
		Damage(1.0f, BATTLEACTION::BEATDOWNACTION, XMVectorSet(0, -10, 0, 0));
	}
}

ENEMY_TYPE EnemyEbilEye::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemyEbilEye::GetOnBattleRange()
{
	return 0.0f;
}

void EnemyEbilEye::Attack(GameObject player, COL_TYPE colType)
{
	if (m_AttackHit)return;
	if (!player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
	if (!m_RotateEnd) {
		if (colType == COL_TYPE::NORMAL && m_BattleModeParam.id == BATTLEACTION::ATTACK1ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage[0], XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
		}
		else if (colType == COL_TYPE::ROTATE && m_BattleModeParam.id == BATTLEACTION::ATTACK3ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage[1], XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		}
	}
}

bool EnemyEbilEye::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
		return true;
	}
	return false;
}

bool EnemyEbilEye::DiscoveryPlayer()
{
	auto groundPos = gameObject->mTransform->WorldPosition();
	auto rayMyPos = groundPos;
	if (!m_Player)return false;
	auto playerPos = m_Player->mTransform->WorldPosition();
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(playerPos - groundPos)).x, -1.0f, 1.0f));
	if (XMVector3Length(groundPos - playerPos).x < m_TrackingRange && (m_View / 3.14f * 180.0f < m_TrackingAngle)) {
		playerPos.y = playerPos.y + 3;
		if (!Hx::PhysX()->Raycast(rayMyPos,
				XMVector3Normalize(playerPos - rayMyPos),
				XMVector3Length(playerPos - rayMyPos).x,
				Layer::UserTag4) && m_ActionModeID == ACTIONMODE::TRACKINGMODE) {
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE,BATTLEACTION::CONFRONTACTION);
			return true;
		}
	}

	return false;
}

bool EnemyEbilEye::LostPlayer()
{
	auto groundPos = gameObject->mTransform->WorldPosition();
	if (!m_Player)return false;
	auto playerPos = m_Player->mTransform->WorldPosition();
	groundPos.y = m_Player->mTransform->WorldPosition().y;
	auto lostVec = playerPos - groundPos;
	if (XMVector3Length(lostVec).x > m_LostRange && m_BattleModeParam.id == BATTLEACTION::CONFRONTACTION) {
		ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::PARENTTRACKING);
		return true;
	}
	return false;
}

void EnemyEbilEye::ChildFinalize()
{
	gameObject->RemoveComponent<CharacterControllerComponent>();
	////gameObject->Disable();
	Hx::Debug()->Log(gameObject->Name());
	Hx::DestroyObject(this->gameObject);
}

void EnemyEbilEye::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EnemyEbilEye::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EnemyEbilEye::TrackingModeFinalize()
{
}

void EnemyEbilEye::TrackingMoveModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
}

void EnemyEbilEye::TrackingMoveModeUpdate()
{
	SetPositionRotation(m_MovePositionCenter,m_MovePositionRadius);
}

void EnemyEbilEye::TrackingMoveModeFinalize()
{
}

void EnemyEbilEye::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EnemyEbilEye::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
}

void EnemyEbilEye::BattleModeFinalize()
{
}

void EnemyEbilEye::ConfrontModeInitilize()
{
	m_Count = (float)(rand() % (int)((m_TackleStartRandMax - m_TackleStartRandMin) * 100.0f)) / 100.0f + m_TackleStartRandMin;
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
	m_UpperdownNow = false;
}

void EnemyEbilEye::ConfrontModeUpdate()
{
	SetPositionRotation(m_MovePositionCenter, m_MovePositionRadius);
	m_PlayerVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	auto canAttack = m_MovePositionCenter - gameObject->mTransform->WorldPosition();
	if(XMVector3Length(canAttack).x < m_MovePositionRadius * 1.1f)
	m_Count -= Hx::DeltaTime()->GetDeltaTime();
	if (m_Count <= 0.0f) {
		if (rand() % 2 == 0) {
			ChangeBattleAction(BATTLEACTION::ATTACK1ACTION);
		}
		else {
			ChangeBattleAction(BATTLEACTION::ATTACK3ACTION);
		}
	}
}

void EnemyEbilEye::ConfrontModeFinalize()
{
}

void EnemyEbilEye::TackleModeInitilize()
{
	m_TackleVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	m_TackleStartPos = gameObject->mTransform->WorldPosition();
	AnimChange(ANIM_ID::ANIM_TACKLE, 10.0f);
	m_RotateEnd = false;
	m_AttackHit = false;
}

void EnemyEbilEye::TackleModeUpdate()
{
	if (!m_RotateEnd) {
		m_Vec += XMVector3Normalize(m_TackleVec) * m_TackleSpeed;
		LookPosition(gameObject->mTransform->WorldPosition() + m_Vec, m_RotateSpeed, true);
	}
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (cc->IsGround()) {
		m_RotateEnd = true;
		AnimChange(ANIM_ID::ANIM_ROTATEEYE, 10.0f, false);
	}
	auto animParam = anim->GetAnimetionParam(ANIM_ID::ANIM_ROTATEEYE);
	if (m_RotateEnd && anim->IsAnimationEnd(ANIM_ID::ANIM_ROTATEEYE)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
		m_AttackHit = false;
	}
}

void EnemyEbilEye::TackleModeFinalize()
{

}

void EnemyEbilEye::RotateTackleModeInitilize()
{
	if (!m_Player)return;
	m_BattleModeParam.battlePosition = m_Player->mTransform->WorldPosition();
	m_RotateStart = false;
	m_RotateEnd = false;
	m_Count = 0.0f; 
	if (m_BattleModeParam.battlePosition.y <= gameObject->mTransform->WorldPosition().y) {
		m_RotateDown = true;
	}
	else {
		m_RotateDown = false;
	}
	m_AttackHit = false;
}

void EnemyEbilEye::RotateTackleModeUpdate()
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!m_RotateStart) {
		bool downMove = false;
		if (m_BattleModeParam.battlePosition.y <= gameObject->mTransform->WorldPosition().y && m_RotateDown) {
			m_Vec += XMVectorSet(0, -1, 0, 0) * m_Speed;
			LookPosition(m_Player->mTransform->WorldPosition(), m_RotateSpeed, true);
			downMove = true;
		}
		else if(m_BattleModeParam.battlePosition.y > gameObject->mTransform->WorldPosition().y && !m_RotateDown){
			m_Vec += XMVectorSet(0, 1, 0, 0) * m_Speed;
			LookPosition(m_Player->mTransform->WorldPosition(), m_RotateSpeed,true);
			downMove = true;
		}

		if (!downMove || cc->IsGround()) {
			m_RotateStart = true;
			AnimChange(ANIM_ID::ANIM_ROTATEATTACK, 10.0f);
			m_TackleStartPos = gameObject->mTransform->WorldPosition();
			m_TackleVec = m_Player->mTransform->WorldPosition() - m_TackleStartPos;
		}
	}
	else {
		if (m_Count > m_TackleTime || XMVector3Length(m_TackleVec).x * 1.5f < XMVector3Length(gameObject->mTransform->WorldPosition() - m_TackleStartPos).x) {
			auto anim = m_ModelObject->GetComponent<AnimationComponent>();
			if (!anim)return;
			auto animParam = anim->GetAnimetionParam(ANIM_ID::ANIM_ROTATEATTACK);
			animParam.mLoop = false;
			anim->SetAnimetionParam(ANIM_ID::ANIM_ROTATEATTACK, animParam);
			if(anim->IsAnimationEnd(ANIM_ID::ANIM_ROTATEATTACK)) {
				m_RotateEnd = true;
				AnimChange(ANIM_ID::ANIM_ROTATEEYE, 10.0f,false);
			}
			animParam = anim->GetAnimetionParam(ANIM_ID::ANIM_ROTATEEYE);
			if (m_RotateEnd && anim->IsAnimationEnd(ANIM_ID::ANIM_ROTATEEYE)) {
				ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
				m_AttackHit = false;
			}
		}
		else {
			m_Vec += XMVector3Normalize(m_TackleVec) * m_TackleSpeed;
			m_Count += Hx::DeltaTime()->GetDeltaTime();
		}
	}
}

void EnemyEbilEye::RotateTackleModeFinalize()
{
}

void EnemyEbilEye::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false,true);

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!cc->IsGround()) {
		m_AccelVec = m_Accel;
	}
	m_WinceCount = m_WinceTime;
}

void EnemyEbilEye::WinceModeUpdate() {
	m_WinceCount -= Hx::DeltaTime()->GetDeltaTime();
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.afterAnimId) && m_WinceCount < 0){
		if (!m_UpperdownNow)
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
		else {
			ChangeBattleAction(BATTLEACTION::UPPERDOWNACTION);
			m_Damage = 0;
			m_Accel = XMVectorSet(0,0,0,0);
		}
	};
}

void EnemyEbilEye::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyEbilEye::UpperDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	if(!m_UpperdownNow)
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false,true);
	m_AccelVec = m_Accel;
	m_UpperdownNow = true;
}

void EnemyEbilEye::UpperDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (cc->IsGround() && m_AccelVec.y <= 0) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
}

void EnemyEbilEye::UpperDownFinalize()
{

}

void EnemyEbilEye::BeatDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += m_Accel;
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false,true);
}

void EnemyEbilEye::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}

}

void EnemyEbilEye::BeatDownFinalize()
{
}

void EnemyEbilEye::DownInitilize()
{
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_TackleStartPos = gameObject->mTransform->Forward();//
	m_TackleStartPos.y = 0;
	m_TackleStartPos += gameObject->mTransform->WorldPosition();
}

void EnemyEbilEye::DownUpdate()
{
	LookPosition(m_TackleStartPos, m_RotateSpeed, true);
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	};
}

void EnemyEbilEye::DownFinalize()
{
}

void EnemyEbilEye::DeadInitilize()
{
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += XMVectorSet(0,-10,0,0);
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
	m_DeadIsGround = false;
}

void EnemyEbilEye::DeadUpdate()
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (m_DeadIsGround) {
		LookPosition(m_TackleStartPos, m_RotateSpeed, true);

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

void EnemyEbilEye::DeadFinalize()
{
}

void EnemyEbilEye::SetPositionRotation(XMVECTOR pos_,float radius_)
{
	auto myPos = gameObject->mTransform->WorldPosition();
	auto moveMat = XMMatrixMultiply(XMMatrixMultiply(XMMatrixTranslation(0, 0, radius_), XMMatrixRotationY(m_RotateAngle)), XMMatrixTranslationFromVector(pos_));
	auto moveVec = XMVectorSet(moveMat.m[3][0], moveMat.m[3][1], moveMat.m[3][2], moveMat.m[3][3]);
	auto naviVec = XMVector3Normalize(moveVec - myPos);
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
	m_Forward = gameObject->mTransform->Forward();
	if (XMVector3Length(myPos - moveVec).x > 1.0f)
		m_Vec += m_Forward * m_Speed;
	//auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	//if (!cc)return;
	//cc->Teleport(moveVec);
	if (XMVector3Length(moveVec - myPos).x < 2.0f) {
		if(!m_RotateOpp)
		m_RotateAngle += m_MoveRotateSpeed * Hx::DeltaTime()->GetDeltaTime();
		else
			m_RotateAngle -= m_MoveRotateSpeed * Hx::DeltaTime()->GetDeltaTime();
	}
}
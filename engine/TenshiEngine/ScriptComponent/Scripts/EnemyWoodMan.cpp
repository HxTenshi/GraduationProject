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

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyWoodMan::TackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyWoodMan::TackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyWoodMan::TackleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyWoodMan::RotateTackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyWoodMan::RotateTackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyWoodMan::RotateTackleModeFinalize, this);

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

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
	m_UpperdownNow = false;
}
void EnemyWoodMan::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
}

void EnemyWoodMan::SoloAction()
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
	auto groundPos = gameObject->mTransform->WorldPosition();
	auto rayMyPos = groundPos;
	if (!m_Player)return false;
	auto playerPos = m_Player->mTransform->WorldPosition();
	groundPos.y = m_Player->mTransform->WorldPosition().y;
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(playerPos - groundPos)).x, -1.0f, 1.0f));
	if (XMVector3Length(groundPos - playerPos).x < m_TrackingRange && (m_View / 3.14f * 180.0f < m_TrackingAngle)) {
		playerPos.y = playerPos.y + 3;
		if (!Hx::PhysX()->Raycast(rayMyPos,
			XMVector3Normalize(playerPos - rayMyPos),
			XMVector3Length(playerPos - rayMyPos).x,
			Layer::UserTag4) && m_ActionModeID == ACTIONMODE::TRACKINGMODE) {
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
			return true;
		}
	}

	return false;
}

bool EnemyWoodMan::LostPlayer()
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
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
}

void EnemyWoodMan::TrackingMoveModeUpdate()
{
	SetPositionRotation(m_MovePositionCenter, m_MovePositionRadius);
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
	m_Count = (float)(rand() % (int)((m_TackleStartRandMax - m_TackleStartRandMin) * 100.0f)) / 100.0f + m_TackleStartRandMin;
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
	m_UpperdownNow = false;
}

void EnemyWoodMan::ConfrontModeUpdate()
{
	SetPositionRotation(m_MovePositionCenter, m_MovePositionRadius);
	m_PlayerVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	auto canAttack = m_MovePositionCenter - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(canAttack).x < m_MovePositionRadius * 1.1f)
		m_Count -= Hx::DeltaTime()->GetDeltaTime();
	if (m_Count <= 0.0f) {
		if (rand() % 2 == 0) {
			ChangeBattleAction(BATTLEACTION::ATTACKDOWNACTION);
		}
		else {
			ChangeBattleAction(BATTLEACTION::ATTACKMONCKEYACTION);
		}
	}
}

void EnemyWoodMan::ConfrontModeFinalize()
{
}

void EnemyWoodMan::TackleModeInitilize()
{
	m_TackleVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	m_TackleStartPos = gameObject->mTransform->WorldPosition();
	AnimChange(ANIM_ID::ANIM_TACKLE, 10.0f);
	m_RotateEnd = false;
}

void EnemyWoodMan::TackleModeUpdate()
{
	if (!m_RotateEnd) {
		m_Vec += XMVector3Normalize(m_TackleVec) * m_TackleSpeed;
		LookPosition(gameObject->mTransform->WorldPosition() + m_Vec);
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
	}
}

void EnemyWoodMan::TackleModeFinalize()
{

}

void EnemyWoodMan::RotateTackleModeInitilize()
{
	if (!m_Player)return;
	m_BattleModeParam.battlePosition = m_Player->mTransform->WorldPosition();
	m_RotateStart = false;
	m_RotateEnd = false;
	m_Count = 0.0f;
}

void EnemyWoodMan::RotateTackleModeUpdate()
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!m_RotateStart) {
		bool downMove = false;
		if (m_BattleModeParam.battlePosition.y <= gameObject->mTransform->WorldPosition().y) {
			m_Vec += XMVectorSet(0, -1, 0, 0) * m_Speed;
			LookPosition(m_Player->mTransform->WorldPosition());
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
			if (anim->IsAnimationEnd(ANIM_ID::ANIM_ROTATEATTACK)) {
				m_RotateEnd = true;
				AnimChange(ANIM_ID::ANIM_ROTATEEYE, 10.0f, false);
			}
			animParam = anim->GetAnimetionParam(ANIM_ID::ANIM_ROTATEEYE);
			if (m_RotateEnd && anim->IsAnimationEnd(ANIM_ID::ANIM_ROTATEEYE)) {
				ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
			}
		}
		else {
			m_Vec += XMVector3Normalize(m_TackleVec) * m_TackleSpeed;
			m_Count += Hx::DeltaTime()->GetDeltaTime();
		}
	}
}

void EnemyWoodMan::RotateTackleModeFinalize()
{
}

void EnemyWoodMan::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);

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
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
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
		AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
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
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
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
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
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

void EnemyWoodMan::SetPositionRotation(XMVECTOR pos_, float radius_)
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
	m_RotateAngle += m_MoveRotateSpeed * Hx::DeltaTime()->GetDeltaTime();
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
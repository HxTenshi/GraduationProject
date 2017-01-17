#include "EnemyEbilEye.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"

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

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyEbilEye::TackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyEbilEye::TackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyEbilEye::TackleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyEbilEye::RotateTackleModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyEbilEye::RotateTackleModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyEbilEye::RotateTackleModeFinalize, this);

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
}
void EnemyEbilEye::ChildInitialize()
{
	m_Gravity = XMVectorSet(0, 0, 0, 0);
	ModelObject = m_ModelObject;
}

void EnemyEbilEye::SoloAction()
{

}

ENEMY_TYPE EnemyEbilEye::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemyEbilEye::GetOnBattleRange()
{
	return 0.0f;
}

void EnemyEbilEye::Attack(GameObject player)
{
}

bool EnemyEbilEye::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	return false;
}

bool EnemyEbilEye::DiscoveryPlayer()
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
				Layer::UserTag4)) {
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
	m_Count = (float)(rand() % 50) / 10.0f + 5.0f;
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
}

void EnemyEbilEye::ConfrontModeUpdate()
{
	SetPositionRotation(m_MovePositionCenter, m_MovePositionRadius);
	m_PlayerVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	m_Count -= Hx::DeltaTime()->GetDeltaTime();

	if (m_Count <= 0.0f) {
		ChangeBattleAction(BATTLEACTION::ATTACKMONCKEYACTION);
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
}

void EnemyEbilEye::TackleModeUpdate()
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
}

void EnemyEbilEye::RotateTackleModeUpdate()
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
			if(anim->IsAnimationEnd(ANIM_ID::ANIM_ROTATEATTACK)) {
				m_RotateEnd = true;
				AnimChange(ANIM_ID::ANIM_ROTATEEYE, 10.0f,false);
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

void EnemyEbilEye::RotateTackleModeFinalize()
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
	m_RotateAngle += m_MoveRotateSpeed * Hx::DeltaTime()->GetDeltaTime();
}

void EnemyEbilEye::LookPosition(XMVECTOR position_)
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

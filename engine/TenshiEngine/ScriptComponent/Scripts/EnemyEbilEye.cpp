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

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
}
void EnemyEbilEye::ChildInitialize()
{
	m_Gravity = XMVectorSet(0, 0, 0, 0);
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
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(playerPos - gameObject->mTransform->WorldPosition())).x, -1.0f, 1.0f));
	groundPos.y = m_Player->mTransform->WorldPosition().y;
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
	auto lostVec = m_MovePositionCenter - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(lostVec).x > m_LostRange) {
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
}

void EnemyEbilEye::TrackingMoveModeUpdate()
{
	auto myPos = gameObject->mTransform->WorldPosition();
	auto moveMat = XMMatrixMultiply(XMMatrixMultiply(XMMatrixTranslation(0, 0, m_MovePositionRadius), XMMatrixRotationY(m_RotateAngle)), XMMatrixTranslationFromVector(m_MovePositionCenter));
	auto moveVec = XMVectorSet(moveMat.m[3][0], moveMat.m[3][1], moveMat.m[3][2], moveMat.m[3][3]);
	auto naviVec = XMVector3Normalize(moveVec - myPos);
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));

	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
	auto trackingNowAngle = m_RotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	//if (m_View < trackingNowAngle)
	trackingNowAngle = m_View;
	auto qua = gameObject->mTransform->Quaternion();
	qua = XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, m_View));
	qua.z = 0;
	gameObject->mTransform->WorldQuaternion(qua);
	m_Forward = gameObject->mTransform->Forward();
	if (XMVector3Length(myPos - moveVec).x > 1.0f)
		m_Vec += m_Forward * m_Speed;
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	cc->Teleport(moveVec);
	m_RotateAngle += m_MoveRotateSpeed * Hx::DeltaTime()->GetDeltaTime();
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

}

void EnemyEbilEye::ConfrontModeUpdate()
{
	auto playerPos = m_Player->mTransform->WorldPosition();
	auto myPos = gameObject->mTransform->WorldPosition();
	auto naviVec = XMVector3Normalize(playerPos - myPos);
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));

	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
	auto trackingNowAngle = m_RotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < m_RotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	qua.z = 0;
	gameObject->mTransform->WorldQuaternion(qua);
	m_Forward = gameObject->mTransform->Forward();
	if (XMVector3Length(myPos - playerPos).x > 1.0f)
		m_Vec += m_Forward * m_Speed;
	//auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	//if (!cc)return;
	//cc->Teleport(moveVec);
}

void EnemyEbilEye::ConfrontModeFinalize()
{

}
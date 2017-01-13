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
	return false;
}

bool EnemyEbilEye::LostPlayer()
{
	return false;
}

void EnemyEbilEye::TrackingModeInitilize()
{
}

void EnemyEbilEye::TrackingModeUpdate()
{
	auto myPos = gameObject->mTransform->WorldPosition();
	auto moveVec = XMVector2Transform(m_MovePositionCenter, XMMatrixMultiply(XMMatrixTranslation(0, 0, m_MovePositionRadius), XMMatrixRotationY(m_RotateAngle)));

	auto naviVec = XMVector3Normalize(moveVec - myPos);
	m_Forward = gameObject->mTransform->Forward();
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));

	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
	auto qua = gameObject->mTransform->Quaternion();
	qua = XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, m_View));
	qua.z = 0;
	gameObject->mTransform->WorldQuaternion(qua);
	m_Forward = gameObject->mTransform->Forward();
	if(XMVector3Length(myPos - moveVec).x > 1.0f)
	m_Vec += m_Forward * m_TrackingSpeed;

	m_RotateAngle += m_RotateSpeed * Hx::DeltaTime()->GetDeltaTime();
}

void EnemyEbilEye::TrackingModeFinalize()
{
}

void EnemyEbilEye::TrackingMoveModeInitilize()
{
}

void EnemyEbilEye::TrackingMoveModeUpdate()
{
}

void EnemyEbilEye::TrackingMoveModeFinalize()
{
}

void EnemyEbilEye::BattleModeInitilize()
{
}

void EnemyEbilEye::BattleModeUpdate()
{
}

void EnemyEbilEye::BattleModeFinalize()
{
}

void EnemyEbilEye::ConfrontModeInitilize()
{
}

void EnemyEbilEye::ConfrontModeUpdate()
{
}

void EnemyEbilEye::ConfrontModeFinalize()
{
}
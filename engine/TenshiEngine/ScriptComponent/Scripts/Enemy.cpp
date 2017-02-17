#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"
#include "EnemyRezardMan.h"
#include "EnemyArcher.h"
#include "EnemyEbilEye.h"
#include "EnemyWoodMan.h"
#include "UniqueObject.h"
#include "EnemyMinotaur.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize() {
	m_Gravity = XMVectorSet(0, -9.81f, 0, 1);
	m_MoveCount = 0;
	m_MoveCountUp = true;
	m_AccelVec = XMVectorZero();
	m_ChildTranckingSpeed = 1.0f;
	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;

	m_Isend = false;
	m_WasAttacked = false;
	ChildInitialize();
	m_Forward = gameObject->mTransform->Forward();
	m_MovieActionFlag = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）s
void Enemy::Start() {
	m_Player = UniqueObject::GetPlayer();
	m_AttackHit = false;
}

//毎フレーム呼ばれます
void Enemy::Update() {
	m_Vec = XMVectorZero();
	if (Input::Trigger(KeyCode::Key_N)){
		static_cast<EnemyRezardMan*>(this)->MoveFrontStart(3.0f);
	}
	if (m_MovieActionFlag) {
		AnimLerp();
		m_MovieAction();
		auto cc = gameObject->GetComponent<CharacterControllerComponent>();
		if (!cc)return;

		//重力
		if (!cc->IsGround()) {
			m_AccelVec += m_Gravity * Hx::DeltaTime()->GetDeltaTime();
		}
		else {
			if (m_AccelVec.y <= 0)
				m_AccelVec = m_Gravity;
		}
		m_Vec += m_AccelVec;

		cc->Move(m_Vec  * Hx::DeltaTime()->GetDeltaTime());
		return;
	}

	if (!m_Player)return;
	auto ps = m_Player->GetScript<PlayerController>();
	if (!ps)return;
	if (ps->GetPlayerState() == PlayerController::PlayerState::Movie)return;

	m_PlayerVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();

	if (gameObject->mTransform->WorldPosition().y < -100) {
		m_Isend = true;
	}

	AnimLerp();

	actionModeUpdate[m_ActionModeID]();

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	//重力
	if (!cc->IsGround()) {
		m_AccelVec += m_Gravity * Hx::DeltaTime()->GetDeltaTime();
	}
	else {
		if (m_AccelVec.y <= 0)
		m_AccelVec = m_Gravity;
	}
	m_Vec += m_AccelVec;

	cc->Move(m_Vec  * Hx::DeltaTime()->GetDeltaTime());
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish() {

}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(GameObject target) {
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)gameObject->AddComponent<NaviMeshComponent>();
	if (target->Name() == "NaviMeshColliderSegment1") {
		navi->SetBaseNaviMeshObject(UniqueObject::GetNaviMesh(1));
	}
	else if (target->Name() == "NaviMeshColliderSegment2") {
		navi->SetBaseNaviMeshObject(UniqueObject::GetNaviMesh(2));
	}
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Enemy::OnCollideEnter(GameObject target) {
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(GameObject target) {
	(void)target;
}

/**************************************************アニメーションの処理****************************************************/
void Enemy::AnimChange(int id, float speed, bool roop, bool forcingChange)
{
	if (forcingChange) {

		AnimeParam ap;
		if (!ModelObject)return;
		auto anim = ModelObject->GetComponent<AnimationComponent>();
		if (!anim)return;

		ap = anim->GetAnimetionParam(m_Animparam.beforeAnimId);
		ap.mWeight = 0.0f;
		ap.mLoop = m_Animparam.beforeAnimLoop;
		anim->SetAnimetionParam(m_Animparam.beforeAnimId, ap);

		ap = anim->GetAnimetionParam(m_Animparam.nowAnimId);
		ap.mWeight = 0.0f;
		ap.mLoop = m_Animparam.afterAnimLoop;
		anim->SetAnimetionParam(m_Animparam.nowAnimId, ap);

		m_Animparam.nowAnimId = id;
		m_Animparam.afterAnimId = id;
		m_Animparam.beforeAnimId = id;
		m_Animparam.lerpSpeed = speed;
		m_Animparam.afterAnimLoop = roop;

		m_Animparam.nowLerpTime = 0.0f;
		m_Animparam.animLerpFlag = false;
		m_Animparam.beforeAnimLoop = m_Animparam.afterAnimLoop;

		ap = anim->GetAnimetionParam(m_Animparam.nowAnimId);
		ap.mTime = 0.0f;

		ap.mWeight = 1.0f;
		ap.mLoop = m_Animparam.beforeAnimLoop;
		anim->SetAnimetionParam(m_Animparam.nowAnimId, ap);

		return;
	}
	if (id != m_Animparam.beforeAnimId) {
		if (!m_Animparam.animLerpFlag) {
			m_Animparam.nowAnimId = id;
			m_Animparam.afterAnimId = id;
			m_Animparam.animLerpFlag = true;
			m_Animparam.lerpSpeed = speed;
			m_Animparam.afterAnimLoop = roop;
			AnimeParam ap;
			if (!ModelObject)return;
			auto anim = ModelObject->GetComponent<AnimationComponent>();
			if (!anim)return;
			ap = anim->GetAnimetionParam(m_Animparam.afterAnimId);
			ap.mTime = 0.0f;
			anim->SetAnimetionParam(m_Animparam.afterAnimId, ap);
		}
	}
}

void Enemy::AnimLerp()
{
	if (m_Animparam.animLerpFlag) {
		if (!ModelObject)return;
		auto anim = ModelObject->GetComponent<AnimationComponent>();
		if (!anim)return;

		m_Animparam.nowLerpTime += m_Animparam.lerpSpeed * Hx::DeltaTime()->GetDeltaTime();

		bool endFlag = false;
		if (m_Animparam.nowLerpTime >= 1.0f) {
			m_Animparam.nowLerpTime = 1.0f;
			endFlag = true;
		}

		AnimeParam ap;
		ap = anim->GetAnimetionParam(m_Animparam.beforeAnimId);
		ap.mWeight = 1.0f - m_Animparam.nowLerpTime;
		ap.mLoop = m_Animparam.beforeAnimLoop;
		anim->SetAnimetionParam(m_Animparam.beforeAnimId, ap);

		ap = anim->GetAnimetionParam(m_Animparam.afterAnimId);
		ap.mWeight = m_Animparam.nowLerpTime;
		ap.mLoop = m_Animparam.afterAnimLoop;
		anim->SetAnimetionParam(m_Animparam.afterAnimId, ap);

		if (endFlag) {
			m_Animparam.beforeAnimId = m_Animparam.afterAnimId;
			m_Animparam.nowLerpTime = 0.0f;
			m_Animparam.animLerpFlag = false;
			m_Animparam.beforeAnimLoop = m_Animparam.afterAnimLoop;
		}
	}
}

void Enemy::LookPosition(XMVECTOR position_, float rotateSpeed, bool zReset)
{
	auto myPos = gameObject->mTransform->WorldPosition();
	auto moveVec = position_;
	auto naviVec = XMVector3Normalize(moveVec - myPos);
	naviVec.y = 0;
	m_Forward = gameObject->mTransform->Forward();
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
	if (m_View < 0.1f)m_View = 0.0f;
	auto trackingNowAngle = rotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < trackingNowAngle)
		trackingNowAngle = m_View;
	gameObject->mTransform->WorldQuaternion(
		XMQuaternionMultiply(gameObject->mTransform->WorldQuaternion(), XMQuaternionRotationAxis(cross, trackingNowAngle)));
	if (zReset) {
		auto myAngle = gameObject->mTransform->Rotate();
		myAngle.z = 0;
		gameObject->mTransform->Rotate(myAngle);
	}
}

float Enemy::GetNowAnimTime() {
	if (!ModelObject)return 0;
	auto anim = ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return 0;

	AnimeParam ap;
	ap = anim->GetAnimetionParam(m_Animparam.nowAnimId);

	return ap.mTime;
}

/********************************************アクションの変更、戦闘時の変更************************************************/
void Enemy::ChangeActionMode(ACTIONMODE nextActionMode) {
	m_ActionModeID = nextActionMode;
	actionModeInitilize[m_ActionModeID]();
	//actionModeUpdate[m_ActionModeID]();
}

void Enemy::ChangeActionAndTrackingAction(ACTIONMODE nextActionMode, TRACKINGACTION::Enum nextTrackingAction)
{
	m_ActionModeID = nextActionMode;
	m_TrackingModeParam.beforeId = m_TrackingModeParam.id;
	m_TrackingModeParam.id = nextTrackingAction;
	actionModeInitilize[m_ActionModeID]();
	//actionModeUpdate[m_ActionModeID]();
}

void Enemy::ChangeActionAndBattleAction(ACTIONMODE nextActionMode, BATTLEACTION::Enum nextBattleAction)
{
	m_ActionModeID = nextActionMode;
	m_BattleModeParam.beforeId = m_BattleModeParam.id;
	m_BattleModeParam.id = nextBattleAction;
	actionModeInitilize[m_ActionModeID]();
	//actionModeUpdate[m_ActionModeID]();
}

void Enemy::ChangeTrackingAction(TRACKINGACTION::Enum nextTrackingAction)
{
	m_TrackingModeParam.beforeId = m_TrackingModeParam.id;
	m_TrackingModeParam.id = nextTrackingAction;
	trackingActionInitilize[m_TrackingModeParam.id]();
	//trackingActionUpdate[m_TrackingModeParam.id]();
}

void Enemy::ChangeBattleAction(BATTLEACTION::Enum nextBattleAction) {
	if (m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		if (m_BattleModeParam.id == nextBattleAction) {
			m_BattleModeParam.sameActionCount++;
			m_BattleModeParam.firstInSameAction = false;
		}
		else {
			m_BattleModeParam.sameActionCount = 0;
			m_BattleModeParam.firstInSameAction = true;
		}
		m_BattleModeParam.beforeId = m_BattleModeParam.id;
		m_BattleModeParam.id = nextBattleAction;
		battleActionInitilize[m_BattleModeParam.id]();
		//battleActionUpdate[m_BattleModeParam.id]();
	}
}

class EmptyEnemy;
class EnemyOrc;
class EnemyGate;
Enemy * Enemy::GetEnemy(GameObject target)
{
	if (!target)return NULL;

	if (auto scr = target->GetScript<EnemyRezardMan>())return scr;
	if (auto scr = target->GetScript<EnemyArcher>())return scr;
	if (auto scr = target->GetScript<EnemyEbilEye>())return scr;
if (auto scr = target->GetScript<EnemyWoodMan>())return scr;
if (auto scr = target->GetScript<EmptyEnemy>())return (Enemy*)scr;
if (auto scr = target->GetScript<EnemyOrc>())return (Enemy*)scr;
if (auto scr = target->GetScript<EnemyGate>())return (Enemy*)scr;
if (auto scr = target->GetScript<EnemyMinotaur>())return (Enemy*)scr;

	return NULL;
}

GameObject Enemy::NextDestinationDecide() {
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (XMVector3Length(m_MovePointsVec[m_MoveCount]->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 2.0f) {
		GameObject movePoint;
		int maxMoveCount = m_MovePointsVec.size() - 1;
		if (m_MovePointsVec.size() == 0)return NULL;
		if (m_MoveCountUp) {
			m_MoveCount++;
			if (m_MoveCount > maxMoveCount) {
				m_MoveCount = maxMoveCount - 1;
				m_MoveCountUp = false;
			}
		}
		else {
			m_MoveCount--;
			if (m_MoveCount < 0) {
				m_MoveCount = 1;
				m_MoveCountUp = true;
			}
		}
	}

	return m_MovePointsVec[m_MoveCount];
}

GameObject Enemy::NextDestinationDeciceInit()
{
	GameObject movePoint = NULL;
	int i = 0;
	float movePointLenMin;
	for (auto& j : m_MovePointsVec) {
		auto movePointLen = XMVector3Length(gameObject->mTransform->WorldPosition() - j.second->mTransform->WorldPosition()).x;
		if (i == 0 || movePointLenMin > movePointLen) {
			movePointLenMin = movePointLen;
			m_MoveCount = i;
			movePoint = j.second;
		}
		i++;
	}

	return movePoint;
}

XMVECTOR Enemy::NaviMeshTracking(GameObject destination,float speed) {
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return XMVectorSet(0, 0, 0, 0);
	if (navi->IsMoveEnd() || XMVector3Length(destination->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 2.0f) {
		navi->RootCreate(gameObject, destination);
	}
	auto naviPos = navi->GetPosition();
	naviPos.y = gameObject->mTransform->WorldPosition().y;
	if(XMVector3Length(gameObject->mTransform->WorldPosition() - naviPos).x < 1.0f)
	navi->Move(speed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());
	return navi->GetPosition();
}

XMVECTOR Enemy::NaviMeshBattle(GameObject destination, float speed)
{
	auto rayMyPos = gameObject->mTransform->WorldPosition();
	auto rayYouPos = destination->mTransform->WorldPosition();
	if (Hx::PhysX()->Raycast(rayMyPos,
		XMVector3Normalize(rayYouPos - rayMyPos),
		XMVector3Length(rayYouPos - rayMyPos).x,
		Layer::UserTag12)) {
		
	}
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return XMVectorSet(0, 0, 0, 0);
	if (navi->IsMoveEnd() || XMVector3Length(destination->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 2.0f) {
		navi->RootCreate(gameObject, destination);
	}
	auto naviPos = navi->GetPosition();
	naviPos.y = gameObject->mTransform->WorldPosition().y;
	if (XMVector3Length(gameObject->mTransform->WorldPosition() - naviPos).x < 1.0f)
		navi->Move(speed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());
	return navi->GetPosition();
}
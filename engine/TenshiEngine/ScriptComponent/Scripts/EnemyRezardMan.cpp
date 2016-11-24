#include "EnemyRezardMan.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"

void EnemyRezardMan::FunctionSet()
{
	ModelObject = m_ModelObject;
	if (!m_Child) {
		if (!m_MovePoints)return;
		for (auto i : m_MovePoints->mTransform->Children()) {
			m_MovePointsVec.push_back(i);
		}
	}
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyRezardMan::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyRezardMan::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyRezardMan::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyRezardMan::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyRezardMan::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyRezardMan::BattleModeFinalize, this);
	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyRezardMan::ParentTrackingModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyRezardMan::ParentTrackingModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyRezardMan::ParentTrackingModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::CHILDTRACKING] = std::bind(&EnemyRezardMan::ChildTrackingModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::CHILDTRACKING] = std::bind(&EnemyRezardMan::ChildTrackingModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::CHILDTRACKING] = std::bind(&EnemyRezardMan::ChildTrackingModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyRezardMan::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyRezardMan::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyRezardMan::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::APPROACHACTION] = std::bind(&EnemyRezardMan::ApproachModeInitilize, this);
	battleActionUpdate[BATTLEACTION::APPROACHACTION] = std::bind(&EnemyRezardMan::ApproachModeUpdate, this);
	battleActionFinalize[BATTLEACTION::APPROACHACTION] = std::bind(&EnemyRezardMan::ApproachModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyRezardMan::AttackDownModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyRezardMan::AttackDownModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&EnemyRezardMan::AttackDownModeFinalize, this);

	battleActionInitilize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&EnemyRezardMan::JumpAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::JUMPATTACKACTION] = std::bind(&EnemyRezardMan::JumpAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&EnemyRezardMan::JumpAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::GUARDACTION] = std::bind(&EnemyRezardMan::GuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::GUARDACTION] = std::bind(&EnemyRezardMan::GuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::GUARDACTION] = std::bind(&EnemyRezardMan::GuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::PROVOCATION] = std::bind(&EnemyRezardMan::ProvocationModeInitilize, this);
	battleActionUpdate[BATTLEACTION::PROVOCATION] = std::bind(&EnemyRezardMan::ProvocationModeUpdate, this);
	battleActionFinalize[BATTLEACTION::PROVOCATION] = std::bind(&EnemyRezardMan::ProvocationModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyRezardMan::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyRezardMan::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyRezardMan::BackStepModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyRezardMan::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EnemyRezardMan::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyRezardMan::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::HITINGUARDACTION] = std::bind(&EnemyRezardMan::HitInGuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::HITINGUARDACTION] = std::bind(&EnemyRezardMan::HitInGuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::HITINGUARDACTION] = std::bind(&EnemyRezardMan::HitInGuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyRezardMan::AttackMonckeyModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyRezardMan::AttackMonckeyModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&EnemyRezardMan::AttackMonckeyModeFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyRezardMan::DeadModeInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyRezardMan::DeadModeUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyRezardMan::DeadModeFinalize, this);

	if (m_DrawLog) {
		Hx::Debug()->Log("捜索時の歩くスピード：" + std::to_string(m_TrackingSpeed));
		Hx::Debug()->Log("捜索時の視界の距離：" + std::to_string(m_TrackingRange));
		Hx::Debug()->Log("捜索時の視界の範囲：" + std::to_string(m_TrackingAngle) + "度");
		Hx::Debug()->Log("捜索時の回転スピード：" + std::to_string(m_TrackingRotateSpeed));
		Hx::Debug()->Log("攻撃をする範囲に入る距離：" + std::to_string(m_OnBattleRange));
		Hx::Debug()->Log("攻撃をする範囲から抜ける距離：" + std::to_string(m_OffBattleRange));
		Hx::Debug()->Log("体力：" + std::to_string(m_Hp));
		Hx::Debug()->Log("周りをうろつく時の回転スピード：" + std::to_string(m_AproachRotateSpeed));
		Hx::Debug()->Log("攻撃時の回転の補正時の回転スピード：" + std::to_string(m_CorrectionRotateSpeed));
		Hx::Debug()->Log("ログを表示するかどうか：" + m_DrawLog ? "TRUE" : "FALSE");
		Hx::Debug()->Log("ガードした時に反撃するまでの回数の最低：" + std::to_string(m_HitInGuardMinCount) + "回から");
		Hx::Debug()->Log("ガードした時に反撃するまでの回数の最高：" + std::to_string(m_HitInGuardMaxCount) + "回までをランダムで選択します");
		Hx::Debug()->Log("攻撃を受けた際絶対回避する確率：" + std::to_string(m_AbsolutelyAvoidInHitAttackProbability) + "%");
		Hx::Debug()->Log("m_Playerにはm_PlayerのGameObjectを入れてください");
		Hx::Debug()->Log("m_ModelObjectにはenemyのモデルを表示しているのGameObject(例:子に入っているRezardMan)を入れてください");
		Hx::Debug()->Log("m_MovePointsには敵を動かしたい地点を子にまとめた親のGameObject(例:enemyMovePoint)を入れてください");
	}
}

bool EnemyRezardMan::GetChildFlag()
{
	return m_Child;
}

float EnemyRezardMan::GetOnBattleRange()
{
	return m_OnBattleRange;
}

void EnemyRezardMan::ParentTrackingModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = m_gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;

	GameObject movePoint;
	if (m_MovePointsVec.size() == 0) {
		if (m_DrawLog)Hx::Debug()->Log("EnemyRezardManMovePointの中身がないよ");
		return;
	}

	int i = 0;
	float movePointLenMin;
	for (auto j : m_MovePointsVec) {
		auto movePointLen = XMVector3Length(m_gameObject->mTransform->WorldPosition() - j->mTransform->WorldPosition()).x;
		if (i == 0 || movePointLenMin > movePointLen) {
			movePointLenMin = movePointLen;
			m_MoveCount = i;
			movePoint = j;
		}
		i++;
	}
	navi->RootCreate(m_gameObject, movePoint);

	m_MoveCountUp = true;

}

void EnemyRezardMan::ParentTrackingModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);

	auto navi = m_gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;
	if (navi->IsMoveEnd()) {
		GameObject movePoint;
		if (!m_MovePoints)return;
		int maxMoveCount = m_MovePointsVec.size() - 1;
		if (m_MovePointsVec.size() == 0)return;
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

		navi->RootCreate(m_gameObject, m_MovePointsVec[m_MoveCount]);
	}

	navi->Move(m_TrackingSpeed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());

	auto cc = m_gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	auto naviVec = XMVector3Normalize(navi->GetPosition() - m_gameObject->mTransform->WorldPosition());
	m_Forward.y = 0;
	naviVec.y = 0;
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
	auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();

	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
	if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = m_gameObject->mTransform->Quaternion();
	m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));

	m_Vec += m_Forward * m_TrackingSpeed * m_ChildTranckingSpeed;
}

void EnemyRezardMan::ParentTrackingModeFinalize()
{
}

void EnemyRezardMan::ChildTrackingModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = m_gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;

	GameObject movePoint;

	if (!m_MovePoints) {
		if (m_DrawLog)Hx::Debug()->Log("enemyMovePointの中身がないよ");
		return;
	}
	movePoint = m_MovePoints;
	m_TrackingRotateSpeed *= 2;
	auto rayMyPos = m_gameObject->mTransform->WorldPosition();
	rayMyPos.y = rayMyPos.y + 3;
	auto rayBossPos = m_MovePoints->mTransform->WorldPosition();
	rayMyPos.y = rayMyPos.y + 3;
	if (Hx::PhysX()->Raycast(rayMyPos,
		XMVector3Normalize(rayBossPos - rayMyPos),
		XMVector3Length(rayBossPos - rayMyPos).x,
		Layer::UserTag4)) {
		movePoint = m_MovePoints;
		navi->RootCreate(m_gameObject, movePoint);
		m_TrackingModeParam.naviMeshFlag = true;
	}
	else {
		m_TrackingModeParam.naviMeshFlag = false;
	}
}

void EnemyRezardMan::ChildTrackingModeUpdate()
{
	if (m_TrackingModeParam.parentAlive) {
		AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
		if (!m_Player)return;
		XMVECTOR playerPos = m_Player->mTransform->WorldPosition();

		m_Forward = XMVector3Normalize(m_gameObject->mTransform->Forward());
		m_PlayerVec = playerPos - m_gameObject->mTransform->WorldPosition();
		m_Forward.y = 0.0f;
		m_PlayerVec.y = 0.0f;
		m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x, -1.0f, 1.0f));
		if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;
		auto rayMyPos = m_gameObject->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		auto rayYourPos = m_Player->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		if ((XMVector3Length(m_PlayerVec).x < m_TrackingRange && m_View / 3.14f * 180.0f < m_TrackingAngle) &&
			!Hx::PhysX()->Raycast(rayMyPos,
				XMVector3Normalize(rayYourPos - rayMyPos),
				XMVector3Length(rayYourPos - rayMyPos).x,
				Layer::UserTag4)) {
			//trackingActionFinalize[m_TrackingModeParam.id]();
			//return;
		}

		if (!m_MovePoints)return;
		rayMyPos = m_gameObject->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		rayYourPos = m_MovePoints->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		if (Hx::PhysX()->Raycast(rayMyPos,
			XMVector3Normalize(rayYourPos - rayMyPos),
			XMVector3Length(rayYourPos - rayMyPos).x,
			Layer::UserTag4)) {
			if (!m_TrackingModeParam.naviMeshFlag) {
				auto navi = m_gameObject->GetComponent<NaviMeshComponent>();
				if (!navi)return;
				GameObject movePoint;
				if (!m_MovePoints)return;
				movePoint = m_MovePoints;
				if (XMVector3Length(movePoint->mTransform->WorldPosition() - m_gameObject->mTransform->WorldPosition()).x < 1.0f) {
					m_ChildTranckingSpeed = 0.0f;
				}
				else {
					m_ChildTranckingSpeed = 1.0f;
					navi->RootCreate(m_gameObject, movePoint);
				}
				m_TrackingModeParam.naviMeshFlag = true;
			}
		}
		else {
			if (m_TrackingModeParam.naviMeshFlag) {
				m_TrackingModeParam.naviMeshFlag = false;
			}
		}

		m_TrackingModeParam.naviMeshFlag = false;

		if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - m_gameObject->mTransform->WorldPosition()).x > 1.0f) {
			m_ChildTranckingSpeed = 1.5f;
		}
		else if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - m_gameObject->mTransform->WorldPosition()).x < 0.3f) {
			m_ChildTranckingSpeed = 0.0f;
		}
		else {
			m_ChildTranckingSpeed = 1.0f;
		}

		if (m_TrackingModeParam.naviMeshFlag) {
			auto navi = m_gameObject->GetComponent<NaviMeshComponent>();
			if (!navi)return;
			if (navi->IsMoveEnd()) {
				GameObject movePoint;
				if (!m_MovePoints)return;
				movePoint = m_MovePoints;
				if (XMVector3Length(movePoint->mTransform->WorldPosition() - m_gameObject->mTransform->WorldPosition()).x < 1.0f) {
					m_ChildTranckingSpeed = 0.0f;
				}
				else {
					m_ChildTranckingSpeed = 1.0f;
					navi->RootCreate(m_gameObject, movePoint);
				}
			}

			navi->Move(m_TrackingSpeed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());

			auto cc = m_gameObject->GetComponent<CharacterControllerComponent>();
			if (!cc)return;

			auto naviVec = XMVector3Normalize(navi->GetPosition() - m_gameObject->mTransform->WorldPosition());
			m_Forward.y = 0;
			naviVec.y = 0;
			auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
			auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
			m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
			if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
			auto qua = m_gameObject->mTransform->Quaternion();
			m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
		}
		else {
			auto cc = m_gameObject->GetComponent<CharacterControllerComponent>();
			if (!cc)return;


			auto childVec = XMVector3Normalize(m_MovePoints->mTransform->WorldPosition() - m_gameObject->mTransform->WorldPosition());
			m_Forward.y = 0;
			childVec.y = 0;
			auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, childVec));
			auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
			m_View = acos(clamp(XMVector3Dot(m_Forward, childVec).x, -1.0f, 1.0f));
			if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
			auto qua = m_gameObject->mTransform->Quaternion();
			m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
		}

		m_Vec += m_Forward * m_TrackingSpeed * m_ChildTranckingSpeed;
	}
	else {
		AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
	}
}

void EnemyRezardMan::ChildTrackingModeFinalize()
{
}


void EnemyRezardMan::ConfrontModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("近づく");
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
}

void EnemyRezardMan::ConfrontModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = true;
	if (XMVector3Length(m_PlayerVec).x <= m_OnBattleRange) {
		m_BattleModeParam.actionFinish = true;
	}
	else {
		m_Vec += m_Forward * m_TrackingSpeed;
	}

	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
	auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = m_gameObject->mTransform->Quaternion();
	m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
}

void EnemyRezardMan::ConfrontModeFinalize()
{
	//ChangeBattleAction(30, 30, 20, 20, 0);
}

void EnemyRezardMan::ApproachModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("周りをうろつく");
	m_BattleModeParam.count = 0.0f;
	m_BattleModeParam.rotateVecPlus = !m_BattleModeParam.rotateVecPlus;
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)((APROACHMAXTIME - APROACHMINTIME) * 100)) / 100.0f) + APROACHMINTIME;
}

void EnemyRezardMan::ApproachModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = true;
	Prowl();
	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		m_BattleModeParam.actionFinish = true;
	}
}

void EnemyRezardMan::ApproachModeFinalize()
{
	//ChangeBattleAction(30, 0, 40, 30, 0);
}

void EnemyRezardMan::AttackDownModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("縦切り");
	AnimChange(ANIM_ID::ANIM_ATTACK_DOWN, 5.0f, false, true);
}

void EnemyRezardMan::AttackDownModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
		auto trackingNowAngle = m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (m_View < m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
		auto qua = m_gameObject->mTransform->Quaternion();
		m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	}

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		m_BattleModeParam.actionFinish = true;
	};
}

void EnemyRezardMan::AttackDownModeFinalize()
{
	//ChangeBattleAction(40, 40, 20, 0, 0);
}

void EnemyRezardMan::JumpAttackModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("ジャンプ切り");
	AnimChange(ANIM_ID::ANIM_JUMPATTACK, 5.0f, false);
}

void EnemyRezardMan::JumpAttackModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 12.5f) {
		m_Vec += m_Forward * 20.0f;
	}
	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		m_BattleModeParam.actionFinish = true;
	};
}

void EnemyRezardMan::JumpAttackModeFinalize()
{
	//ChangeBattleAction(40, 20, 10, 30, 0);
	//ChangeBattleAction(0, 0, 100, 0, 0);
}

void EnemyRezardMan::GuardModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("ガード");
	m_BattleModeParam.count = 0.0f;
	if (rand() % 2 == 0) {
		m_BattleModeParam.rotateVecPlus = true;
	}
	else {
		m_BattleModeParam.rotateVecPlus = false;
	}
	AnimChange(ANIM_ID::ANIM_GUARD, 5.0f);
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)((APROACHMAXTIME - APROACHMINTIME) * 100)) / 100.0f) + APROACHMINTIME;
}

void EnemyRezardMan::GuardModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = true;
	Prowl();
	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		m_BattleModeParam.actionFinish = true;
	}
}

void EnemyRezardMan::GuardModeFinalize()
{
	//ChangeBattleAction(20, 30, 20, 30, 0);
}

void EnemyRezardMan::ProvocationModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("挑発");
	m_BattleModeParam.count = 0.0f;
	if (rand() % 2 == 0) {
		m_BattleModeParam.rotateVecPlus = true;
	}
	else {
		m_BattleModeParam.rotateVecPlus = false;
	}
	AnimChange(ANIM_ID::ANIM_PROVOCATION, 5.0f, true);
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)((APROACHMAXTIME - APROACHMINTIME) * 100)) / 100.0f) + APROACHMINTIME;
}

void EnemyRezardMan::ProvocationModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = true;
	Prowl();
	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		m_BattleModeParam.actionFinish = true;
	}
}

void EnemyRezardMan::ProvocationModeFinalize()
{
}

void EnemyRezardMan::BackStepModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("バックステップ");
	AnimChange(ANIM_ID::ANIM_BACKSTEP, 5.0f, false, true);
}

void EnemyRezardMan::BackStepModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
		auto trackingNowAngle = m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (m_View < m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
		auto qua = m_gameObject->mTransform->Quaternion();
		m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	}

	if (GetNowAnimTime() < 12.5f) {
		m_Vec -= m_Forward * 20.0f;
	}
	else if (GetNowAnimTime() < 20.0f) {
		m_BattleModeParam.actionFinish = true;
	}
}

void EnemyRezardMan::BackStepModeFinalize()
{
	//ChangeBattleAction(30, 0, 0, 0, 70);
}

void EnemyRezardMan::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_DrawLog)
		Hx::Debug()->Log(std::to_string(m_Damage) + "ダメージ喰らった。残りの体力は" + std::to_string(m_Hp));
	if (m_Hp <= 0) {
		SetBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
}

void EnemyRezardMan::WinceModeUpdate() {
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		m_BattleModeParam.actionFinish = true;
	};
}

void EnemyRezardMan::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyRezardMan::HitInGuardModeInitilize() {
	if (m_DrawLog)
		Hx::Debug()->Log("防いだ");
	AnimChange(ANIM_ID::ANIM_HITINGUARD, 5.0f, false, true);

	if (m_BattleModeParam.firstInSameAction) {
		PatienceInThisTime = ((rand() % m_HitInGuardMaxCount) + m_HitInGuardMinCount);
		if (m_DrawLog)
			Hx::Debug()->Log(std::to_string(PatienceInThisTime + 1) + "回防いだらモンキーアタックします");
	}
}

void EnemyRezardMan::HitInGuardModeUpdate() {
	m_BattleModeParam.canChangeAttackAction = false;
	if (PatienceInThisTime > m_BattleModeParam.sameActionCount) {
		auto anim = m_ModelObject->GetComponent<AnimationComponent>();
		if (!anim)return;
		if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
			m_BattleModeParam.actionFinish = true;
		};
	}
	else {
		SetBattleAction(BATTLEACTION::ATTACKMONCKEYACTION);
	}
}

void EnemyRezardMan::HitInGuardModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyRezardMan::Prowl()
{
	if (!m_Player)return;
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();
	//auto rot = XMMatrixRotationY(m_AproachRotateSpeed * Hx::DeltaTime()->GetDeltaTime() * (m_BattleModeParam.rotateVecPlus == true ? 1.0f : -1.0f));
	//auto pos = XMMatrixMultiply(XMMatrixMultiply(trans, rot), XMMatrixTranslationFromVector(playerPos));

	m_Vec += XMVector3Normalize(m_BattleModeParam.battlePosition - m_gameObject->mTransform->WorldPosition()) * m_TrackingSpeed;

	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
	auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = m_gameObject->mTransform->Quaternion();
	m_gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
}

void EnemyRezardMan::AttackMonckeyModeInitilize() {
	if (m_DrawLog)
		Hx::Debug()->Log("モンキーアタック");
	AnimChange(ANIM_ID::ANIM_ATTACK_MONCKEY, 5.0f, false);
}

void EnemyRezardMan::AttackMonckeyModeUpdate() {
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		m_BattleModeParam.actionFinish = true;
	};
}

void EnemyRezardMan::AttackMonckeyModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 40, 20, 0, 0);
}

void EnemyRezardMan::DeadModeInitilize()
{
	if (m_DrawLog) {
		Hx::Debug()->Log("死んだ");
	}

	AnimChange(ANIM_ID::ANIM_PROVOCATION, 5.0f, false, true);
}

void EnemyRezardMan::DeadModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		if (!m_MyWeapon)return;
		//m_gameObject->Enable();
		m_gameObject->RemoveComponent<CharacterControllerComponent>();
		Hx::DestroyObject(m_MyWeapon);
		Hx::DestroyObject(m_gameObject);
	};
}

void EnemyRezardMan::DeadModeFinalize()
{
}

void EnemyRezardMan::Attack(GameObject player)
{
	if (m_DrawLog)
		Hx::Debug()->Log("何かに武器が当たった");
	if (m_BattleModeParam.id == BATTLEACTION::ATTACKDOWNACTION ||
		m_BattleModeParam.id == BATTLEACTION::ATTACKMONCKEYACTION ||
		m_BattleModeParam.id == BATTLEACTION::JUMPATTACKACTION) {
		if (m_DrawLog)
			Hx::Debug()->Log("m_Playerに攻撃がHit");
		if (!player)return;
		auto playerScript = player->GetScript<PlayerController>();
		if (!playerScript)return;

		if (m_BattleModeParam.id == BATTLEACTION::ATTACKDOWNACTION)
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
		else if (m_BattleModeParam.id == BATTLEACTION::ATTACKMONCKEYACTION)
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		else if (m_BattleModeParam.id == BATTLEACTION::JUMPATTACKACTION)
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
	}
}

/****************************************************ダメージの処理********************************************************/
void EnemyRezardMan::Damage(float damage_)
{
	m_Damage = damage_;
	if (m_ActionModeID == ACTIONMODE::BATTLEMODE && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		//if (rand() % 100 > (99 - m_AbsolutelyAvoidInHitAttackProbability) &&
		//	m_BattleModeParam.id != BATTLEACTION::HITINGUARDACTION &&
		//	m_BattleModeParam.id != BATTLEACTION::WINCEACTION	&&
		//	m_BattleModeParam.id != BATTLEACTION::BACKSTEPACTION &&
		//	m_BattleModeParam.id != BATTLEACTION::JUMPATTACKACTION) {
		//	if (m_DrawLog) {
		//		Hx::Debug()->Log("避けた");
		//	}
		//	ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
		//}
		//else 
		//{
		if (m_BattleModeParam.id == BATTLEACTION::GUARDACTION || m_BattleModeParam.id == BATTLEACTION::HITINGUARDACTION) {
			SetBattleAction(BATTLEACTION::HITINGUARDACTION);
		}
		else {
			SetBattleAction(BATTLEACTION::WINCEACTION);
		}
		//}
	}
}

bool EnemyRezardMan::DiscoveryPlayer()
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
		return true;
	}

	return false;
}

bool EnemyRezardMan::LostPlayer()
{
	if (!m_Player) {
		if (m_DrawLog)Hx::Debug()->Log("PlayerのGameObjectがいないよ");
		return false;
	}
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(m_PlayerVec).x > m_LostRange) {
		return true;
	}
	return false;
}

/****************************************************捜索時の処理**********************************************************/
void EnemyRezardMan::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EnemyRezardMan::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EnemyRezardMan::TrackingModeFinalize()
{
	//m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	//ChangeActionMode(ACTIONMODE::BATTLEMODE);
}

/****************************************************戦闘時の処理**********************************************************/
void EnemyRezardMan::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EnemyRezardMan::BattleModeUpdate()
{
	if (!m_Player)return;
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();

	m_Forward = XMVector3Normalize(gameObject->mTransform->Forward());
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	m_Forward.y = 0.0f;
	m_PlayerVec.y = 0.0f;
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x, -1.0f, 1.0f));
	if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;

	if (XMVector3Length(m_PlayerVec).x < m_OffBattleRange) {
		m_BattleModeParam.arrival = true;
	}

	battleActionUpdate[m_BattleModeParam.id]();
}

void EnemyRezardMan::BattleModeFinalize()
{
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	SetActionMode(ACTIONMODE::TRACKINGMODE);
}

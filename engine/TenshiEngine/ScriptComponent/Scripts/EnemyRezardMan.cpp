#include "EnemyRezardMan.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"

EnemyRezardMan::EnemyRezardMan()
{
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

	battleActionInitilize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyRezardMan::AttackDownModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyRezardMan::AttackDownModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyRezardMan::AttackDownModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK2ACTION] = std::bind(&EnemyRezardMan::JumpAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK2ACTION] = std::bind(&EnemyRezardMan::JumpAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK2ACTION] = std::bind(&EnemyRezardMan::JumpAttackModeFinalize, this);

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

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyRezardMan::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyRezardMan::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyRezardMan::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyRezardMan::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyRezardMan::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyRezardMan::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyRezardMan::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EnemyRezardMan::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyRezardMan::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::HITINGUARDACTION] = std::bind(&EnemyRezardMan::HitInGuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::HITINGUARDACTION] = std::bind(&EnemyRezardMan::HitInGuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::HITINGUARDACTION] = std::bind(&EnemyRezardMan::HitInGuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK3ACTION] = std::bind(&EnemyRezardMan::AttackMonckeyModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK3ACTION] = std::bind(&EnemyRezardMan::AttackMonckeyModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK3ACTION] = std::bind(&EnemyRezardMan::AttackMonckeyModeFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyRezardMan::DeadModeInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyRezardMan::DeadModeUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyRezardMan::DeadModeFinalize, this);
}

void EnemyRezardMan::ChildInitialize()
{
	m_SetPoint = true;
	ModelObject = m_ModelObject;
	m_MaxHp = hp;
	m_Hp = hp;
	if (!m_Child) {
		if (!m_MovePoints) {
			m_SetPoint = false;
			return;
		}
		for (auto i : m_MovePoints->mTransform->Children()){
			m_MovePointsVec[std::stoi(i->Name()) - 1] = i;
		}
	}

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

void EnemyRezardMan::SoloAction()
{	
	if (m_BattleModeParam.id == BATTLEACTION::CONFRONTACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(30, 30, 20, 20, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::APPROACHACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(30, 0, 40, 30, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::ATTACK1ACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 40, 20, 0, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::ATTACK2ACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 20, 0, 30, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::GUARDACTION) {
		if (m_BattleModeParam.beforeId == BATTLEACTION::BACKSTEPACTION) {
			ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(20, 30, 0, 30, 0, 5));
		}
		else {
			ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(20, 30, 20, 30, 0, 5));
		}
	}
	else if (m_BattleModeParam.id == BATTLEACTION::PROVOCATION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(30, 0, 40, 30, 0, 0));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::BACKSTEPACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(15, 0, 0, 0, 70, 15));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::WINCEACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 0, 40, 20, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::DOWNACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 0, 40, 20, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::HITINGUARDACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 0, 40, 20, 0, 5));
	}
	else if (m_BattleModeParam.id == BATTLEACTION::ATTACK3ACTION) {
		ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 40, 20, 0, 0, 5));
	}
}

ENEMY_TYPE EnemyRezardMan::GetEnemyType()
{
	ENEMY_TYPE enemy_type = ENEMY_TYPE::PARENT;
	if (m_Child) {
		enemy_type = ENEMY_TYPE::CHILD;
	}
	
	return enemy_type;
}

float EnemyRezardMan::GetOnBattleRange()
{
	return m_OnBattleRange;
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

void EnemyRezardMan::ParentTrackingModeInitilize()
{
	if (!m_SetPoint)return;
	if (m_DrawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;

	GameObject movePoint;
	if (m_MovePointsVec.size() == 0) {
		if (m_DrawLog)Hx::Debug()->Log("EnemyRezardManMovePointの中身がないよ");
		return;
	}

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
	navi->RootCreate(gameObject, movePoint);

	m_MoveCountUp = true;
}

void EnemyRezardMan::MoveFrontStart(float time)
{
	m_MovieAction = std::bind(&EnemyRezardMan::MoveFront,this);
	m_MovieActionFlag = true;
	m_MoveFrontTime = time;
	m_MoveFrontCounter = 0.0f;
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
}

void EnemyRezardMan::MoveFront()
{
	m_MoveFrontCounter += Hx::DeltaTime()->GetDeltaTime();
	m_Vec += gameObject->mTransform->Forward() * m_TrackingSpeed;
	if (m_MoveFrontCounter > m_MoveFrontTime) {
		m_MoveFrontCounter = 0.0f;
		m_MovieActionFlag = false;
		AnimChange(ANIM_ID::ANIM_IDLE, 5.0f,true,true);
	}
}

void EnemyRezardMan::ParentTrackingModeUpdate()
{
	if (!m_SetPoint) {
		AnimChange(ANIM_ID::ANIM_IDLE,5.0f);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	auto distination = NextDestinationDecide();
	if (!distination)return;
	if (m_NaviMeshUse) {
		LookPosition(NaviMeshTracking(distination,m_TrackingSpeed),m_TrackingRotateSpeed);
	}
	else {
		LookPosition(distination->mTransform->WorldPosition(), m_TrackingRotateSpeed);
	}
	m_Forward = gameObject->mTransform->Forward();
	m_Vec += m_Forward * m_TrackingSpeed;
}

void EnemyRezardMan::ParentTrackingModeFinalize()
{

}

void EnemyRezardMan::ChildTrackingModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;


	if (!m_MovePoints) {
		if (m_DrawLog)Hx::Debug()->Log("enemyMovePointの中身がないよ");
		return;
	}

	if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x > 1.0f) {
		navi->RootCreate(gameObject, m_MovePoints);
	}
}

void EnemyRezardMan::ChildTrackingModeUpdate()
{
	//if (m_DrawLog) {
	//	Hx::Debug()->Log("x :" + std::to_string(m_MovePoints->mTransform->WorldPosition().x) + 
	//		" y :" + std::to_string(m_MovePoints->mTransform->WorldPosition().y) + 
	//		" z :" + std::to_string(m_MovePoints->mTransform->WorldPosition().z));
	//}
	if (m_TrackingModeParam.parentAlive) {
		AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
		if (!m_SetPoint)return;
		if (!m_MovePoints)return;
		if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x > 3.0f) {
			m_ChildTranckingSpeed = 1.5f;
		}
		else if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 1.0f) {
			m_ChildTranckingSpeed = 0.0f;
		}
		else {
			m_ChildTranckingSpeed = 1.0f;
		}
		auto navi = gameObject->GetComponent<NaviMeshComponent>();
		if (!navi)return;

		if (Hx::PhysX()->Raycast(gameObject->mTransform->WorldPosition(),
			XMVector3Normalize(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()),
			XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x,
			Layer::UserTag4) && !m_NaviMeshUse) {
			m_NaviMeshUse = true;
			m_ChildTranckingSpeed = 1.0f;
			navi->RootCreate(gameObject, m_MovePoints);
		}

		if (m_NaviMeshUse) {
			if (navi->IsMoveEnd()) {
				m_NaviMeshUse = false;
				if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x > 1.0f) {
					
				}
			}
			navi->Move(m_TrackingSpeed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());
			LookPosition(navi->GetPosition(), m_TrackingRotateSpeed);
		}
		else {
			LookPosition(m_MovePoints->mTransform->WorldPosition(), m_TrackingRotateSpeed);
		}
		m_Forward = gameObject->mTransform->Forward();

		m_Vec += m_Forward * m_TrackingSpeed * m_ChildTranckingSpeed;
	}
	else {
		AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
		if (!m_SetPoint)return;
	}
}

void EnemyRezardMan::ChildTrackingModeFinalize()
{
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
	ChangeActionMode(ACTIONMODE::TRACKINGMODE);
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
	Prowl();
	auto battlePosVec = m_BattleModeParam.battlePosition - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(m_PlayerVec).x <= m_OnBattleRange) {
		m_BattleModeParam.actionFinish = true;
	}
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
	m_AttackHit = false;
}

void EnemyRezardMan::AttackDownModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		LookPosition(m_PlayerVec, m_CorrectionRotateSpeed);
	}

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		m_AttackHit = false;
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
	m_AttackHit = false;
}

void EnemyRezardMan::JumpAttackModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = true;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 12.5f) {
		m_Vec += m_Forward * 20.0f;
	}
	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		m_AttackHit = false;
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
		LookPosition(m_PlayerVec, m_CorrectionRotateSpeed);
	}

	if (GetNowAnimTime() < 12.5f) {
		m_Vec -= m_Forward * 20.0f;
	}
	else {
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
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!cc->IsGround()){
		m_AccelVec = m_Accel;
	}
}

void EnemyRezardMan::WinceModeUpdate() {
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		m_BattleModeParam.actionFinish = true;
	};
}

void EnemyRezardMan::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyRezardMan::UpperDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_DrawLog)
		Hx::Debug()->Log(std::to_string(m_Damage) + "ダメージ喰らった。残りの体力は" + std::to_string(m_Hp));
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	AnimChange(ANIM_ID::ANIM_UPPERDOWN, 5.0f, false, true);
	m_AccelVec = m_Accel;
}

void EnemyRezardMan::UpperDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	
	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if(cc->IsGround() && m_AccelVec.y <= 0) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
}

void EnemyRezardMan::UpperDownFinalize()
{
	
}

void EnemyRezardMan::BeatDownInitilize()
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
	AnimChange(ANIM_ID::ANIM_BEATDOWN, 5.0f, false, true);
}

void EnemyRezardMan::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	
	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
	
}

void EnemyRezardMan::BeatDownFinalize()
{
}

void EnemyRezardMan::DownInitilize()
{
	if (m_BattleModeParam.beforeId == BATTLEACTION::UPPERDOWNACTION) {
		AnimChange(ANIM_ID::ANIM_UPPERDOWNAFTER, 5.0f, false, true);
	}
	else {
		AnimChange(ANIM_ID::ANIM_BEATDOWNAFTER, 5.0f, false, true);
	}
}

void EnemyRezardMan::DownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		m_BattleModeParam.actionFinish = true;
	};
}

void EnemyRezardMan::DownFinalize()
{
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
		if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
			m_BattleModeParam.actionFinish = true;
		};
	}
	else {
		ChangeBattleAction(BATTLEACTION::ATTACK3ACTION);
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

	auto battlePosVec = m_BattleModeParam.battlePosition - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(battlePosVec).x >= 10)
		m_Vec += XMVector3Normalize(battlePosVec) * m_TrackingSpeed * 2.0f;
	else
		m_Vec += XMVector3Normalize(battlePosVec) * m_TrackingSpeed;

	LookPosition(m_Player->mTransform->WorldPosition(), m_TrackingRotateSpeed);
}

void EnemyRezardMan::AttackMonckeyModeInitilize() {
	if (m_DrawLog)
		Hx::Debug()->Log("モンキーアタック");
	AnimChange(ANIM_ID::ANIM_ATTACK_MONCKEY, 5.0f, false);
	m_AttackHit = false;
}

void EnemyRezardMan::AttackMonckeyModeUpdate() {
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		m_AttackHit = false;
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

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		if (!m_MyWeapon)return;
		//gameObject->Enable();
		m_Isend = true;
	};
}

void EnemyRezardMan::DeadModeFinalize()
{
}

//攻撃するときに呼ぶ処理

void EnemyRezardMan::Attack(GameObject player, COL_TYPE colType)
{
	if (m_AttackHit)return;
	if (m_DrawLog)
		Hx::Debug()->Log("何かに武器が当たった");
	if (m_BattleModeParam.id == BATTLEACTION::ATTACK1ACTION ||
		m_BattleModeParam.id == BATTLEACTION::ATTACK3ACTION ||
		m_BattleModeParam.id == BATTLEACTION::ATTACK2ACTION) {
		if (m_DrawLog)
			Hx::Debug()->Log("m_Playerに攻撃がHit");
		if (!player)return;
		auto playerScript = player->GetScript<PlayerController>();
		if (!playerScript)return;

		if (m_BattleModeParam.id == BATTLEACTION::ATTACK1ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
		}
		else if (m_BattleModeParam.id == BATTLEACTION::ATTACK3ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		}
		else if (m_BattleModeParam.id == BATTLEACTION::ATTACK2ACTION) {
			m_AttackHit = true;
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		}
	}
}

/****************************************************ダメージの処理********************************************************/
bool EnemyRezardMan::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION) {
		if (m_ActionModeID == ACTIONMODE::BATTLEMODE) {
			if (m_BattleModeParam.id == BATTLEACTION::GUARDACTION || m_BattleModeParam.id == BATTLEACTION::HITINGUARDACTION) {
				ChangeBattleAction(BATTLEACTION::HITINGUARDACTION);
				return false;
			}
			else {
				ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE,winceType_);
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
bool EnemyRezardMan::DiscoveryPlayer()
{
	if (!m_Player) {
		Hx::Debug()->Log("PlayerのGameObjectがいないよ");
		return false;
	}
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();

	auto rayMyPos = gameObject->mTransform->WorldPosition();
	rayMyPos.y = rayMyPos.y + 3;
	auto rayBossPos = m_Player->mTransform->WorldPosition();
	rayBossPos.y = rayMyPos.y;

	m_Forward = XMVector3Normalize(gameObject->mTransform->Forward());
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	m_Forward.y = 0.0f;
	m_PlayerVec.y = 0.0f;
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x, -1.0f, 1.0f));
	if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;
	if ((XMVector3Length(m_PlayerVec).x < m_TrackingRange && m_View / 3.14f * 180.0f < m_TrackingAngle)) {
		if (!Hx::PhysX()->Raycast(rayMyPos,
			XMVector3Normalize(rayBossPos - rayMyPos),
			XMVector3Length(rayBossPos - rayMyPos).x,
			Layer::UserTag4)) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

//敵を見失ったか
bool EnemyRezardMan::LostPlayer()
{
	if (!m_Player) {
		if (m_DrawLog)Hx::Debug()->Log("PlayerのGameObjectがいないよ");
		return false;
	}
	auto rayMyPos = gameObject->mTransform->WorldPosition();
	rayMyPos.y = rayMyPos.y + 3;
	auto rayBossPos = m_Player->mTransform->WorldPosition();
	rayBossPos.y = rayMyPos.y;

	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	if (XMVector3Length(m_PlayerVec).x > m_LostRange || 
		Hx::PhysX()->Raycast(rayMyPos,
		XMVector3Normalize(rayBossPos - rayMyPos),
		XMVector3Length(rayBossPos - rayMyPos).x,
		Layer::UserTag4)) {
		return true;
	}
	return false;
}

void EnemyRezardMan::ChildFinalize()
{
	Hx::DestroyObject(m_MyWeapon);
	//gameObject->Disable();
	gameObject->RemoveComponent<CharacterControllerComponent>();
	Hx::DestroyObject(gameObject);
}

void EnemyRezardMan::SetMovePoint(GameObject movePoint_)
{
	m_SetPoint = true;
	for (auto i : m_MovePoints->mTransform->Children()) {
		m_MovePointsVec[std::stoi(i->Name()) - 1] = i;
	}
}

BATTLEACTION::Enum EnemyRezardMan::GetChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability, int provocationProbability) {
	//if (XMVector3Length(m_PlayerVec).x > m_OffBattleRange && m_BattleModeParam.battleActionID != BATTLEACTION::BACKSTEPACTION) {
	//	ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	//	return;
	//}
	int totalProbability =
		guardProbability + approachProbability +
		backstepProbability + attackProbability +
		jumpAttackProbability + provocationProbability;

	int randam = rand() % totalProbability;

	int guardProbability_, approachProbability_,
		backstepProbability_, attackProbability_,
		jumpAttackProbability_, provocationProbability_;

	guardProbability_ = totalProbability - guardProbability;
	approachProbability_ = guardProbability_ - approachProbability;
	backstepProbability_ = approachProbability_ - backstepProbability;
	attackProbability_ = backstepProbability_ - attackProbability;
	jumpAttackProbability_ = attackProbability_ - jumpAttackProbability;
	provocationProbability_ = jumpAttackProbability_ - provocationProbability;

	if (randam > guardProbability_) {
		return BATTLEACTION::GUARDACTION;
	}
	else if (randam > approachProbability_) {
		return BATTLEACTION::APPROACHACTION;
	}
	else if (randam > backstepProbability_) {
		return BATTLEACTION::BACKSTEPACTION;
	}
	else if (randam > attackProbability_) {
		return BATTLEACTION::ATTACK1ACTION;
	}
	else if (randam > jumpAttackProbability_) {
		return BATTLEACTION::ATTACK2ACTION;
	}
	else if (randam > provocationProbability_) {
		return BATTLEACTION::PROVOCATION;
	}
	return BATTLEACTION::CONFRONTACTION;
}
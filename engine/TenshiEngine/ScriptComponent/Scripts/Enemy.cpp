#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize() {
	m_Gravity = XMVectorSet(0, -9.81f, 0, 1);
	m_MoveCount = 0;
	m_MoveCountUp = true;

	m_ChildTranckingSpeed = 1.0f;
	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_BattleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;

	if (!m_Player) {
		m_Player = Hx::FindActor("Player");
	}

	if (!m_Child) {
		if (!m_MovePoints)return;
		for (auto i : m_MovePoints->mTransform->Children()) {
			m_MovePointsVec.push_back(i);
		}
	}

	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&Enemy::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&Enemy::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&Enemy::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&Enemy::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&Enemy::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&Enemy::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&Enemy::ParentTrackingModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&Enemy::ParentTrackingModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&Enemy::ParentTrackingModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::CHILDTRACKING] = std::bind(&Enemy::ChildTrackingModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::CHILDTRACKING] = std::bind(&Enemy::ChildTrackingModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::CHILDTRACKING] = std::bind(&Enemy::ChildTrackingModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeInitilize, this);
	battleActionUpdate[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeUpdate, this);
	battleActionFinalize[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeFinalize, this);

	battleActionInitilize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::PROVOCATION] = std::bind(&Enemy::ProvocationModeInitilize, this);
	battleActionUpdate[BATTLEACTION::PROVOCATION] = std::bind(&Enemy::ProvocationModeUpdate, this);
	battleActionFinalize[BATTLEACTION::PROVOCATION] = std::bind(&Enemy::ProvocationModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeFinalize, this);

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

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）s
void Enemy::Start() {
}

//毎フレーム呼ばれます
void Enemy::Update() {
	m_Vec = XMVectorZero();
	if (Input::Trigger(KeyCode::Key_1)) {
		Damage(1.0f);
	}

	AnimLerp();

	actionModeUpdate[m_ActionModeID]();

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	//重力
	if (!cc->IsGround()) {
		m_Vec += m_Gravity;
	}

	cc->Move(m_Vec  * Hx::DeltaTime()->GetDeltaTime());
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Enemy::Finish() {

}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(GameObject target) {
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

/****************************************************ダメージの処理********************************************************/
void Enemy::Damage(float damage_)
{
	m_Damage = damage_;
	if (m_ActionModeID == ACTIONMODE::BATTLEMODE && m_BattleModeParam.battleActionID != BATTLEACTION::DEADACTION) {
		if (rand() % 100 > (99 - m_AbsolutelyAvoidInHitAttackProbability) &&
			m_BattleModeParam.battleActionID != BATTLEACTION::HITINGUARDACTION &&
			m_BattleModeParam.battleActionID != BATTLEACTION::WINCEACTION	&&
			m_BattleModeParam.battleActionID != BATTLEACTION::BACKSTEPACTION &&
			m_BattleModeParam.battleActionID != BATTLEACTION::JUMPATTACKACTION) {
			if (m_DrawLog) {
				Hx::Debug()->Log("避けた");
			}
			ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
		}
		else {
			if (m_BattleModeParam.battleActionID == BATTLEACTION::GUARDACTION || m_BattleModeParam.battleActionID == BATTLEACTION::HITINGUARDACTION) {
				ChangeBattleAction(BATTLEACTION::HITINGUARDACTION);
			}
			else {
				ChangeBattleAction(BATTLEACTION::WINCEACTION);
			}
		}
	}
}

void Enemy::Attack(GameObject player)
{
	if (m_DrawLog)
		Hx::Debug()->Log("何かに武器が当たった");
	if (m_BattleModeParam.battleActionID == BATTLEACTION::ATTACKDOWNACTION ||
		m_BattleModeParam.battleActionID == BATTLEACTION::ATTACKMONCKEYACTION ||
		m_BattleModeParam.battleActionID == BATTLEACTION::JUMPATTACKACTION) {
		if (m_DrawLog)
			Hx::Debug()->Log("m_Playerに攻撃がHit");
		if (!player)return;
		auto playerScript = player->GetScript<PlayerController>();
		if (!playerScript)return;

		if (m_BattleModeParam.battleActionID == BATTLEACTION::ATTACKDOWNACTION)
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
		else if (m_BattleModeParam.battleActionID == BATTLEACTION::ATTACKMONCKEYACTION)
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		else if (m_BattleModeParam.battleActionID == BATTLEACTION::JUMPATTACKACTION)
			playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
	}
}

/**************************************************アニメーションの処理****************************************************/
void Enemy::AnimChange(int id, float speed, bool roop, bool forcingChange)
{
	if (forcingChange) {
		if (m_DrawLog)
			Hx::Debug()->Log("強制的にアニメーションを切り替えた");

		AnimeParam ap;
		if (!m_ModelObject)return;
		auto anim = m_ModelObject->GetComponent<AnimationComponent>();
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
			if (!m_ModelObject)return;
			auto anim = m_ModelObject->GetComponent<AnimationComponent>();
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
		if (!m_ModelObject)return;
		auto anim = m_ModelObject->GetComponent<AnimationComponent>();
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

float Enemy::GetNowAnimTime() {
	if (!m_ModelObject)return 0;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
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
	m_TrackingModeParam.beforetrackingActionID = m_TrackingModeParam.trackingActionID;
	m_TrackingModeParam.trackingActionID = nextTrackingAction;
	actionModeInitilize[m_ActionModeID]();
	//actionModeUpdate[m_ActionModeID]();
}

void Enemy::ChangeActionAndBattleAction(ACTIONMODE nextActionMode, BATTLEACTION::Enum nextBattleAction)
{
	m_ActionModeID = nextActionMode;
	m_BattleModeParam.beforeBattleActionID = m_BattleModeParam.battleActionID;
	m_BattleModeParam.battleActionID = nextBattleAction;
	actionModeInitilize[m_ActionModeID]();
	//actionModeUpdate[m_ActionModeID]();
}

void Enemy::ChangeTrackingAction(TRACKINGACTION::Enum nextTrackingAction)
{
	m_TrackingModeParam.beforetrackingActionID = m_TrackingModeParam.trackingActionID;
	m_TrackingModeParam.trackingActionID = nextTrackingAction;
	trackingActionInitilize[m_TrackingModeParam.trackingActionID]();
	//trackingActionUpdate[m_TrackingModeParam.trackingActionID]();
}

void Enemy::ChangeBattleAction(BATTLEACTION::Enum nextBattleAction) {
	if (m_BattleModeParam.battleActionID != BATTLEACTION::DEADACTION) {
		if (m_BattleModeParam.battleActionID == nextBattleAction) {
			m_BattleModeParam.sameActionCount++;
			m_BattleModeParam.firstInSameAction = false;
		}
		else {
			m_BattleModeParam.sameActionCount = 0;
			m_BattleModeParam.firstInSameAction = true;
		}
		m_BattleModeParam.beforeBattleActionID = m_BattleModeParam.battleActionID;
		m_BattleModeParam.battleActionID = nextBattleAction;
		battleActionInitilize[m_BattleModeParam.battleActionID]();
		//battleActionUpdate[m_BattleModeParam.battleActionID]();
	}
}

void Enemy::ChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability) {
	if (XMVector3Length(m_PlayerVec).x > m_OffBattleRange && m_BattleModeParam.battleActionID != BATTLEACTION::BACKSTEPACTION) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
		return;
	}

	int randam = rand() % 100;

	int guardProbability_, approachProbability_, backstepProbability_, attackProbability_, jumpAttackProbability_;

	guardProbability_ = 100 - guardProbability;
	approachProbability_ = guardProbability_ - approachProbability;
	backstepProbability_ = approachProbability_ - backstepProbability;
	attackProbability_ = backstepProbability_ - attackProbability;
	jumpAttackProbability_ = attackProbability_ - jumpAttackProbability;

	if (randam > guardProbability_) {
		ChangeBattleAction(BATTLEACTION::GUARDACTION);
	}
	else if (randam > approachProbability_) {
		ChangeBattleAction(BATTLEACTION::APPROACHACTION);
	}
	else if (randam > backstepProbability_) {
		ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
	}
	else if (randam > attackProbability_) {
		ChangeBattleAction(BATTLEACTION::ATTACKDOWNACTION);
	}
	else if (randam > jumpAttackProbability_) {
		ChangeBattleAction(BATTLEACTION::JUMPATTACKACTION);
	}

}

/****************************************************捜索時の処理**********************************************************/
void Enemy::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.trackingActionID]();
}

void Enemy::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.trackingActionID]();
}

void Enemy::TrackingModeFinalize()
{
	//m_BattleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	//ChangeActionMode(ACTIONMODE::BATTLEMODE);
}

bool Enemy::DiscoveryPlayer()
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
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x,-1.0f,1.0f));
	if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;
	if ((XMVector3Length(m_PlayerVec).x < m_TrackingRange && m_View / 3.14f * 180.0f < m_TrackingAngle)) {
		return true;
	}

	return false;
}

bool Enemy::LostPlayer()
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

void Enemy::ParentTrackingModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;

	GameObject movePoint;
	if (m_MovePointsVec.size() == 0) {
		if (m_DrawLog)Hx::Debug()->Log("enemyMovePointの中身がないよ");
		return;
	}

	int i = 0;
	float movePointLenMin;
	for (auto j : m_MovePointsVec) {
		auto movePointLen = XMVector3Length(gameObject->mTransform->WorldPosition() - j->mTransform->WorldPosition()).x;
		if (i == 0 || movePointLenMin > movePointLen) {
			movePointLenMin = movePointLen;
			m_MoveCount = i;
			movePoint = j;
		}
		i++;
	}
	navi->RootCreate(gameObject, movePoint);

	m_MoveCountUp = true;

}

void Enemy::ParentTrackingModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);

	auto navi = gameObject->GetComponent<NaviMeshComponent>();
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

		navi->RootCreate(gameObject, m_MovePointsVec[m_MoveCount]);
	}

	navi->Move(m_TrackingSpeed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	auto naviVec = XMVector3Normalize(navi->GetPosition() - gameObject->mTransform->WorldPosition());
	m_Forward.y = 0;
	naviVec.y = 0;
	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
	auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();

	m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x,-1.0f,1.0f));
	if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));

	m_Vec += m_Forward * m_TrackingSpeed * m_ChildTranckingSpeed;
}

void Enemy::ParentTrackingModeFinalize()
{
	actionModeFinalize[m_ActionModeID]();
}

void Enemy::ChildTrackingModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;

	GameObject movePoint;
	
	if (!m_MovePoints) {
		if (m_DrawLog)Hx::Debug()->Log("enemyMovePointの中身がないよ");
		return;
	}
	movePoint = m_MovePoints;
	m_TrackingRotateSpeed *= 2;
	auto rayMyPos = gameObject->mTransform->WorldPosition();
	rayMyPos.y = rayMyPos.y + 3;
	auto rayBossPos = m_MovePoints->mTransform->WorldPosition();
	rayMyPos.y = rayMyPos.y + 3;
	if (Hx::PhysX()->Raycast(rayMyPos,
		XMVector3Normalize(rayBossPos - rayMyPos),
		XMVector3Length(rayBossPos - rayMyPos).x,
		Layer::UserTag4)) {
		movePoint = m_MovePoints;
		navi->RootCreate(gameObject, movePoint);
		m_TrackingModeParam.naviMeshFlag = true;
	}
	else {
		m_TrackingModeParam.naviMeshFlag = false;
	}
}

void Enemy::ChildTrackingModeUpdate()
{
	if (m_TrackingModeParam.parentAlive) {
		AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
		if (!m_Player)return;
		XMVECTOR playerPos = m_Player->mTransform->WorldPosition();

		m_Forward = XMVector3Normalize(gameObject->mTransform->Forward());
		m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
		m_Forward.y = 0.0f;
		m_PlayerVec.y = 0.0f;
		m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x, -1.0f, 1.0f));
		if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;
		auto rayMyPos = gameObject->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		auto rayYourPos = m_Player->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		if ((XMVector3Length(m_PlayerVec).x < m_TrackingRange && m_View / 3.14f * 180.0f < m_TrackingAngle) &&
			!Hx::PhysX()->Raycast(rayMyPos,
				XMVector3Normalize(rayYourPos - rayMyPos),
				XMVector3Length(rayYourPos - rayMyPos).x,
				Layer::UserTag4)) {
			//trackingActionFinalize[m_TrackingModeParam.trackingActionID]();
			//return;
		}

		if (!m_MovePoints)return;
		rayMyPos = gameObject->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		rayYourPos = m_MovePoints->mTransform->WorldPosition();
		rayMyPos.y = rayMyPos.y + 3;
		if (Hx::PhysX()->Raycast(rayMyPos,
			XMVector3Normalize(rayYourPos - rayMyPos),
			XMVector3Length(rayYourPos - rayMyPos).x,
			Layer::UserTag4)) {
			if (!m_TrackingModeParam.naviMeshFlag) {
				auto navi = gameObject->GetComponent<NaviMeshComponent>();
				if (!navi)return;
				GameObject movePoint;
				if (!m_MovePoints)return;
				movePoint = m_MovePoints;
				if (XMVector3Length(movePoint->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 1.0f) {
					m_ChildTranckingSpeed = 0.0f;
				}
				else {
					m_ChildTranckingSpeed = 1.0f;
					navi->RootCreate(gameObject, movePoint);
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

		if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x > 1.0f) {
			m_ChildTranckingSpeed = 1.5f;
		}
		else if (XMVector3Length(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 0.3f) {
			m_ChildTranckingSpeed = 0.0f;
		}
		else {
			m_ChildTranckingSpeed = 1.0f;
		}

		if (m_TrackingModeParam.naviMeshFlag) {
			auto navi = gameObject->GetComponent<NaviMeshComponent>();
			if (!navi)return;
			if (navi->IsMoveEnd()) {
				GameObject movePoint;
				if (!m_MovePoints)return;
				movePoint = m_MovePoints;
				if (XMVector3Length(movePoint->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 1.0f) {
					m_ChildTranckingSpeed = 0.0f;
				}
				else {
					m_ChildTranckingSpeed = 1.0f;
					navi->RootCreate(gameObject, movePoint);
				}
			}

			navi->Move(m_TrackingSpeed * m_ChildTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());

			auto cc = gameObject->GetComponent<CharacterControllerComponent>();
			if (!cc)return;

			auto naviVec = XMVector3Normalize(navi->GetPosition() - gameObject->mTransform->WorldPosition());
			m_Forward.y = 0;
			naviVec.y = 0;
			auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
			auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
			m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
			if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
			auto qua = gameObject->mTransform->Quaternion();
			gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
		}
		else {
			auto cc = gameObject->GetComponent<CharacterControllerComponent>();
			if (!cc)return;


			auto childVec = XMVector3Normalize(m_MovePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
			m_Forward.y = 0;
			childVec.y = 0;
			auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, childVec));
			auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
			m_View = acos(clamp(XMVector3Dot(m_Forward, childVec).x, -1.0f, 1.0f));
			if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
			auto qua = gameObject->mTransform->Quaternion();
			gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
		}

		m_Vec += m_Forward * m_TrackingSpeed * m_ChildTranckingSpeed;
	}
	else {
		AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
	}
}

void Enemy::ChildTrackingModeFinalize()
{
	actionModeFinalize[m_ActionModeID]();
}

/****************************************************戦闘時の処理**********************************************************/
void Enemy::BattleModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("戦闘モードへ移行");
	battleActionInitilize[m_BattleModeParam.battleActionID]();
}

void Enemy::BattleModeUpdate()
{
	if (!m_Player)return;
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();

	m_Forward = XMVector3Normalize(gameObject->mTransform->Forward());
	m_PlayerVec = playerPos - gameObject->mTransform->WorldPosition();
	m_Forward.y = 0.0f;
	m_PlayerVec.y = 0.0f;
	m_View = acos(clamp(XMVector3Dot(m_Forward, XMVector3Normalize(m_PlayerVec)).x,-1.0f,1.0f));
	if (XMVector3Length(m_Forward - XMVector3Normalize(m_PlayerVec)).x < 0.01f)m_View = 0.0f;

	if (XMVector3Length(m_BattleModeParam.battlePosition - gameObject->mTransform->WorldPosition()).x < 0.3f) {
		m_BattleModeParam.arrival = true;
	}

	battleActionUpdate[m_BattleModeParam.battleActionID]();
}

void Enemy::BattleModeFinalize()
{
	m_BattleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	ChangeActionMode(ACTIONMODE::TRACKINGMODE);
}

void Enemy::ConfrontModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("近づく");
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
}

void Enemy::ConfrontModeUpdate()
{
	if (XMVector3Length(m_PlayerVec).x <= m_OnBattleRange) {
		battleActionFinalize[BATTLEACTION::CONFRONTACTION]();
	}
	else {
		m_Vec += m_Forward * m_TrackingSpeed;
	}

	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
	auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
}

void Enemy::ConfrontModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(30, 30, 20, 20, 0);
}

void Enemy::ApproachModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("周りをうろつく");
	m_BattleModeParam.count = 0.0f;
	m_BattleModeParam.rotateVecPlus = !m_BattleModeParam.rotateVecPlus;
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)(APROACHMAXTIME * 100)) / 100.0f) + APROACHMINTIME;
}

void Enemy::ApproachModeUpdate()
{
	Prowl();
	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	}
}

void Enemy::ApproachModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(30, 0, 40, 30, 0);
}

void Enemy::AttackDownModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("縦切り");
	AnimChange(ANIM_ID::ANIM_ATTACK_DOWN, 5.0f, false);
}

void Enemy::AttackDownModeUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
		auto trackingNowAngle = m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (m_View < m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	}

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	};
}

void Enemy::AttackDownModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 40, 20, 0, 0);
}

void Enemy::JumpAttackModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("ジャンプ切り");
	AnimChange(ANIM_ID::ANIM_JUMPATTACK, 5.0f, false);
}

void Enemy::JumpAttackModeUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 12.5f) {
		m_Vec += m_Forward * 20.0f;
	}
	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	};
}

void Enemy::JumpAttackModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 20, 10, 30, 0);
	//ChangeBattleAction(0, 0, 100, 0, 0);
}

void Enemy::GuardModeInitilize()
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
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)(APROACHMAXTIME * 100)) / 100.0f) + APROACHMINTIME;
}

void Enemy::GuardModeUpdate()
{
	Prowl();


	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	}
}

void Enemy::GuardModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(20, 30, 20, 30, 0);
}

void Enemy::ProvocationModeInitilize()
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
	AnimChange(ANIM_ID::ANIM_PROVOCATION, 5.0f,true);
	m_BattleModeParam.decideAprochTime = ((float)(rand() % (int)(APROACHMAXTIME * 100)) / 100.0f) + APROACHMINTIME;
}

void Enemy::ProvocationModeUpdate()
{
	Prowl();

	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (m_BattleModeParam.count > m_BattleModeParam.decideAprochTime) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	}
}

void Enemy::ProvocationModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
}

void Enemy::BackStepModeInitilize()
{
	if (m_DrawLog)
		Hx::Debug()->Log("バックステップ");
	AnimChange(ANIM_ID::ANIM_BACKSTEP, 5.0f, false, true);
}

void Enemy::BackStepModeUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
		auto trackingNowAngle = m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (m_View < m_CorrectionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	}

	if (GetNowAnimTime() < 12.5f) {
		m_Vec -= m_Forward * 20.0f;
	}
	else if (GetNowAnimTime() < 20.0f) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	}
}

void Enemy::BackStepModeFinalize()
{
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(30, 0, 0, 0, 70);
}

void Enemy::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_DrawLog)
		Hx::Debug()->Log(std::to_string(m_Damage) + "ダメージ喰らった。残りの体力は" + std::to_string(m_Hp));
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
}

void Enemy::WinceModeUpdate() {
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	};
}

void Enemy::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void Enemy::HitInGuardModeInitilize() {
	if (m_DrawLog)
		Hx::Debug()->Log("防いだ");
	AnimChange(ANIM_ID::ANIM_HITINGUARD, 5.0f, false, true);

	if (m_BattleModeParam.firstInSameAction) {
		PatienceInThisTime = ((rand() % m_HitInGuardMaxCount) + m_HitInGuardMinCount);
		if (m_DrawLog)
			Hx::Debug()->Log(std::to_string(PatienceInThisTime + 1) + "回防いだらモンキーアタックします");
	}
}

void Enemy::HitInGuardModeUpdate() {
	if (PatienceInThisTime > m_BattleModeParam.sameActionCount) {
		auto anim = m_ModelObject->GetComponent<AnimationComponent>();
		if (!anim)return;
		if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
			battleActionFinalize[m_BattleModeParam.battleActionID]();
		};
	}
	else {
		ChangeBattleAction(BATTLEACTION::ATTACKMONCKEYACTION);
	}
}

void Enemy::HitInGuardModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void Enemy::Prowl()
{
	if (!m_Player)return;
	XMVECTOR playerPos = m_Player->mTransform->WorldPosition();
	//auto rot = XMMatrixRotationY(m_AproachRotateSpeed * Hx::DeltaTime()->GetDeltaTime() * (m_BattleModeParam.rotateVecPlus == true ? 1.0f : -1.0f));
	//auto pos = XMMatrixMultiply(XMMatrixMultiply(trans, rot), XMMatrixTranslationFromVector(playerPos));

	m_Vec += XMVector3Normalize(m_BattleModeParam.battlePosition - gameObject->mTransform->WorldPosition()) * m_TrackingSpeed;

	auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, XMVector3Normalize(m_PlayerVec)));
	auto trackingNowAngle = m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (m_View < m_TrackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = m_View;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
}

void Enemy::AttackMonckeyModeInitilize() {
	if (m_DrawLog)
		Hx::Debug()->Log("モンキーアタック");
	AnimChange(ANIM_ID::ANIM_ATTACK_MONCKEY, 5.0f, false);
}

void Enemy::AttackMonckeyModeUpdate() {
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		battleActionFinalize[m_BattleModeParam.battleActionID]();
	};
}

void Enemy::AttackMonckeyModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 40, 20, 0, 0);
}

void Enemy::DeadModeInitilize()
{
	if (m_DrawLog) {
		Hx::Debug()->Log("死んだ");
	}

	AnimChange(ANIM_ID::ANIM_PROVOCATION, 5.0f, false, true);
}

void Enemy::DeadModeUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.nowAnimId)) {
		if (!m_MyWeapon)return;
		//gameObject->Enable();
		gameObject->RemoveComponent<CharacterControllerComponent>();
		Hx::DestroyObject(m_MyWeapon);
		Hx::DestroyObject(gameObject);
	};
}

void Enemy::DeadModeFinalize()
{
}
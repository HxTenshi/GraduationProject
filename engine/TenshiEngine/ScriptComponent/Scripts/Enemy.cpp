#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"
#include "EnemyRezardMan.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize() {
	m_Gravity = XMVectorSet(0, -9.81f, 0, 1);
	m_MoveCount = 0;
	m_MoveCountUp = true;

	m_ChildTranckingSpeed = 1.0f;
	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;

	if (!m_Player) {
		m_Player = Hx::FindActor("Player");
	}
	

	//trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&Enemy::ParentTrackingModeInitilize, this);
	//trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&Enemy::ParentTrackingModeUpdate, this);
	//trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&Enemy::ParentTrackingModeFinalize, this);
	//
	//trackingActionInitilize[TRACKINGACTION::CHILDTRACKING] = std::bind(&Enemy::ChildTrackingModeInitilize, this);
	//trackingActionUpdate[TRACKINGACTION::CHILDTRACKING] = std::bind(&Enemy::ChildTrackingModeUpdate, this);
	//trackingActionFinalize[TRACKINGACTION::CHILDTRACKING] = std::bind(&Enemy::ChildTrackingModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::PROVOCATION] = std::bind(&Enemy::ProvocationModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::PROVOCATION] = std::bind(&Enemy::ProvocationModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::PROVOCATION] = std::bind(&Enemy::ProvocationModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeFinalize, this);
	//
	//battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeInitilize, this);
	//battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeUpdate, this);
	//battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeFinalize, this);

	FunctionSet();
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

Enemy * Enemy::GetEnemy(GameObject target)
{
	if (!target)return NULL;

	if (auto scr = target->GetScript<EnemyRezardMan>())return scr;

	return NULL;
}

BATTLEACTION::Enum Enemy::ChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability, int provocationProbability){
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
		return BATTLEACTION::ATTACKDOWNACTION;
	}
	else if (randam > jumpAttackProbability_) {
		return BATTLEACTION::JUMPATTACKACTION;
	}
	else if (randam > provocationProbability_) {
		return BATTLEACTION::PROVOCATION;
	}
	return BATTLEACTION::CONFRONTACTION;
}

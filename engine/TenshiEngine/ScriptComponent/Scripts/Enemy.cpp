#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"
#include "EnemyRezardMan.h"
#include "EnemyArcher.h"
#include "EnemyEbilEye.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Initialize() {
	m_Gravity = XMVectorSet(0, -9.81f, 0, 1);
	m_MoveCount = 0;
	m_MoveCountUp = true;
	m_AccelVec = XMVectorZero();
	m_ChildTranckingSpeed = 1.0f;
	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;

	if (!m_Player) {
		m_Player = Hx::FindActor("Player");
	}
	m_Isend = false;
	m_WasAttacked = false;
	ChildInitialize();
	m_Forward = gameObject->mTransform->Forward();
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��js
void Enemy::Start() {
}

//���t���[���Ă΂�܂�
void Enemy::Update() {
	m_Vec = XMVectorZero();
	if (!m_Player)return;
	m_PlayerVec = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	if (Input::Trigger(KeyCode::Key_1)) {
		Damage(1.0f,BATTLEACTION::WINCEACTION, XMVectorSet(0, 2, 0, 1));
	}
	else if (Input::Trigger(KeyCode::Key_2)) {
		Damage(1.0f, BATTLEACTION::UPPERDOWNACTION,XMVectorSet(0,10,0,1));
	}
	else if (Input::Trigger(KeyCode::Key_3)) {
		Damage(1.0f, BATTLEACTION::BEATDOWNACTION,XMVectorSet(0, -20, 0, 1));
	}

	if (gameObject->mTransform->WorldPosition().y < -10) {
		gameObject->RemoveComponent<CharacterControllerComponent>();
		Hx::DestroyObject(gameObject);
		m_Isend = true;
	}

	AnimLerp();

	actionModeUpdate[m_ActionModeID]();

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	//�d��
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Enemy::Finish() {

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideBegin(GameObject target) {
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Enemy::OnCollideEnter(GameObject target) {
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void Enemy::OnCollideExit(GameObject target) {
	(void)target;
}

/**************************************************�A�j���[�V�����̏���****************************************************/
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

/********************************************�A�N�V�����̕ύX�A�퓬���̕ύX************************************************/
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
	if (auto scr = target->GetScript<EnemyArcher>())return scr;
	if (auto scr = target->GetScript<EnemyEbilEye>())return scr;

	return NULL;
}
#include "EnemyOrc.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"
#include "Game/Component/NaviMeshComponent.h"
#include "UniqueObject.h"
#include "SoundManager.h"
EnemyOrc::EnemyOrc()
{
	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyOrc::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyOrc::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&EnemyOrc::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyOrc::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyOrc::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyOrc::BattleModeFinalize, this);

	trackingActionInitilize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyOrc::TrackingMoveModeInitilize, this);
	trackingActionUpdate[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyOrc::TrackingMoveModeUpdate, this);
	trackingActionFinalize[TRACKINGACTION::PARENTTRACKING] = std::bind(&EnemyOrc::TrackingMoveModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyOrc::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyOrc::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyOrc::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyOrc::WeakAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyOrc::WeakAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACK1ACTION] = std::bind(&EnemyOrc::WeakAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyOrc::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&EnemyOrc::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&EnemyOrc::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyOrc::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyOrc::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&EnemyOrc::BackStepModeFinalize, this);

	battleActionInitilize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyOrc::UpperDownInitilize, this);
	battleActionUpdate[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyOrc::UpperDownUpdate, this);
	battleActionFinalize[BATTLEACTION::UPPERDOWNACTION] = std::bind(&EnemyOrc::UpperDownFinalize, this);

	battleActionInitilize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyOrc::BeatDownInitilize, this);
	battleActionUpdate[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyOrc::BeatDownUpdate, this);
	battleActionFinalize[BATTLEACTION::BEATDOWNACTION] = std::bind(&EnemyOrc::BeatDownFinalize, this);

	battleActionInitilize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyOrc::DownInitilize, this);
	battleActionUpdate[BATTLEACTION::DOWNACTION] = std::bind(&EnemyOrc::DownUpdate, this);
	battleActionFinalize[BATTLEACTION::DOWNACTION] = std::bind(&EnemyOrc::DownFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyOrc::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyOrc::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyOrc::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	m_RotateAngle = 0;
	m_UpperdownNow = false;
}
void EnemyOrc::ChildInitialize()
{
	m_SetPoint = true;
	if (!m_MovePoints)m_SetPoint = false;
	else {
		for (auto i : m_MovePoints->mTransform->Children()) {
			if (m_MovePoints->mTransform->Children().size() == 1) {
				m_MovePointsVec[0] = i;
				break;
			}
			m_MovePointsVec[std::stoi(i->Name()) - 1] = i;
		}
	}

	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
	m_StartForward = gameObject->mTransform->Forward();
	m_StartPos = gameObject->mTransform->WorldPosition();

	AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
	//ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
}

void EnemyOrc::SoloAction()
{
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

ENEMY_TYPE EnemyOrc::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemyOrc::GetOnBattleRange()
{
	return 0.0f;
}

void EnemyOrc::Attack(GameObject player, COL_TYPE colType)
{
	//if (m_MovePoints && gameObject->mTransform->GetParent()->Name() == "OrcTeam_ZERO")Hx::Debug()->Log("ƒ€[•”ƒ|ƒCƒ“ƒg‚ ‚é‚æ");
	if (m_AttackHit)return;
	if (!player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
	if (m_ActionModeID != ACTIONMODE::BATTLEMODE)return;
	if (colType == COL_TYPE::NORMAL && m_BattleModeParam.id == BATTLEACTION::ATTACK1ACTION) {
		m_AttackHit = true;
		playerScript->Damage(m_AttackDamage, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
	}
}

bool EnemyOrc::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::UPPERDOWNACTION &&
		m_BattleModeParam.id != BATTLEACTION::BEATDOWNACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION) {
		m_WinceBeforeId = m_BattleModeParam.id;
	}
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::BACKSTEPACTION) {
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
		return true;
	}
	return false;
}

//“G‚ð”­Œ©‚µ‚½‚©
bool EnemyOrc::DiscoveryPlayer()
{
	if (!m_Player) {
		Hx::Debug()->Log("Player‚ÌGameObject‚ª‚¢‚È‚¢‚æ");
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
		if(m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION)
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
		return true;
	}
	return false;
}

//“G‚ðŒ©Ž¸‚Á‚½‚©
bool EnemyOrc::LostPlayer()
{
	return false;
	if (!m_Player) {
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
		if (m_BattleModeParam.id != BATTLEACTION::DEADACTION && m_BattleModeParam.id != BATTLEACTION::WINCEACTION && m_BattleModeParam.id != BATTLEACTION::DOWNACTION)
		ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::PARENTTRACKING);
		return true;
	}
	return false;
}

void EnemyOrc::ChildFinalize()
{
	////gameObject->Disable();
	Hx::Debug()->Log(gameObject->Name());
	Hx::DestroyObject(this->gameObject);
}

void EnemyOrc::SetMovePoint(GameObject target)
{
	m_MovePoints = target;
	if (m_MovePoints)Hx::Debug()->Log(m_MovePoints->Name());

	Hx::Debug()->Log("before" + std::to_string(m_MovePointsVec.size()));
	if (!m_MovePoints)m_SetPoint = false;
	else {
		for (auto i : m_MovePoints->mTransform->Children()) {
			if (m_MovePoints->mTransform->Children().size() == 1) {
				m_MovePointsVec[0] = i;
				break;
			}
			m_MovePointsVec[std::stoi(i->Name()) - 1] = i;
		}
	}
	Hx::Debug()->Log("after" + std::to_string(m_MovePointsVec.size()));
	NextDestinationDeciceInit();

	m_SetPoint = true;
}

void EnemyOrc::MoveFrontStart(float time)
{
	m_MovieAction = std::bind(&EnemyOrc::MoveFront, this);
	m_MovieActionFlag = true;
	m_MoveFrontTime = time;
	m_MoveFrontCounter = 0.0f;
	AnimChange(ANIM_ID::ANIM_MOVE, 5.0f);
}

void EnemyOrc::MoveFront()
{
	m_MoveFrontCounter += Hx::DeltaTime()->GetDeltaTime();
	m_Vec += gameObject->mTransform->Forward() * m_TrackingSpeed;
	if (m_MoveFrontCounter > m_MoveFrontTime) {
		m_MoveFrontCounter = 0.0f;
		m_MovieActionFlag = false;
		AnimChange(ANIM_ID::ANIM_IDLE, 5.0f, true, true);
	}
}

void EnemyOrc::TrackingModeInitilize()
{
	trackingActionInitilize[m_TrackingModeParam.id]();
}

void EnemyOrc::TrackingModeUpdate()
{
	trackingActionUpdate[m_TrackingModeParam.id]();
}

void EnemyOrc::TrackingModeFinalize()
{
	trackingActionFinalize[m_TrackingModeParam.id]();
}

void EnemyOrc::TrackingMoveModeInitilize()
{
	NextDestinationDeciceInit();
}

void EnemyOrc::TrackingMoveModeUpdate()
{
	if (!m_SetPoint) {
		AnimChange(ANIM_ID::ANIM_IDLE, 5.0f);
		return;
	}
	else {
		AnimChange(ANIM_ID::ANIM_MOVE, 5.0f);
		if (!m_SetPoint)return;
		auto distination = NextDestinationDecide();
		if (!distination)return;
		LookPosition(NaviMeshTracking(distination, m_TrackingSpeed), m_TrackingRotateSpeed);
		m_Forward = gameObject->mTransform->Forward();
		m_Vec += m_Forward * m_TrackingSpeed;
	}
}

void EnemyOrc::TrackingMoveModeFinalize()
{
}

void EnemyOrc::BattleModeInitilize()
{
	battleActionInitilize[m_BattleModeParam.id]();
}

void EnemyOrc::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
}

void EnemyOrc::BattleModeFinalize()
{
	battleActionFinalize[m_BattleModeParam.id]();
}

void EnemyOrc::ConfrontModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_MOVE, 10.0f);
	m_UpperdownNow = false;
}

void EnemyOrc::ConfrontModeUpdate()
{
	auto playerPos = m_Player->mTransform->WorldPosition();
	auto myPos = gameObject->mTransform->WorldPosition();		
	LookPosition(NaviMeshBattle(m_Player, m_TrackingSpeed), m_TrackingRotateSpeed);
	m_Forward = gameObject->mTransform->Forward();
	m_Vec += m_Forward * m_TrackingSpeed;
	if (XMVector3Length(myPos - playerPos).x < m_BattleRange) {
		ChangeBattleAction(BATTLEACTION::ATTACK1ACTION);
	}
}

void EnemyOrc::ConfrontModeFinalize()
{
}

void EnemyOrc::WeakAttackModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_NOMALATTACK, 10.0f,false);
	if(!m_Child)
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Boar_cry1, gameObject->mTransform->WorldPosition());
	else
		SoundManager::PlaySE(SoundManager::SoundSE_ID::Boar_child_cry1, gameObject->mTransform->WorldPosition());
	m_AttackHit = false;
}

void EnemyOrc::WeakAttackModeUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(ANIM_ID::ANIM_NOMALATTACK)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
		m_AttackHit = false;
	}
}

void EnemyOrc::WeakAttackModeFinalize()
{
	m_AttackHit = false;
}

void EnemyOrc::WinceModeInitilize() {
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}

	m_ManyHit++;
	if (m_ManyHit >= 8) {
		ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
		m_ManyHit = 0;
		return;
	}

	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true,true);

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (!cc->IsGround()) {
		m_AccelVec = m_Accel;
	}
	m_WinceCount = m_WinceTime;
}

void EnemyOrc::WinceModeUpdate() {
	m_WinceCount -= Hx::DeltaTime()->GetDeltaTime();
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(m_Animparam.afterAnimId) && m_WinceCount < 0) {
		m_ManyHit = 0;
		if (!m_UpperdownNow)
			ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
		else {
			ChangeBattleAction(BATTLEACTION::UPPERDOWNACTION);
			m_Damage = 0;
			m_Accel = XMVectorSet(0, 0, 0, 0);
		}
	};
}

void EnemyOrc::WinceModeFinalize() {
	m_BattleModeParam.actionFinish = true;
	//ChangeBattleAction(40, 0, 40, 20, 0);
}

void EnemyOrc::BackStepModeInitilize()
{
	AnimChange(ANIM_ID::ANIM_BACKSTEP, 5.0f, false, true);
}

void EnemyOrc::BackStepModeUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f) {
		LookPosition(m_PlayerVec, 400);
	}

	if (GetNowAnimTime() < 20.0f) {
		m_Vec -= m_Forward * 3.5f;
	}
	else {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	}
}

void EnemyOrc::BackStepModeFinalize()
{
	//ChangeBattleAction(30, 0, 0, 0, 70);
}

void EnemyOrc::UpperDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	if (!m_UpperdownNow)
		AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_AccelVec = m_Accel;
	m_UpperdownNow = true;
}

void EnemyOrc::UpperDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	m_BattleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;
	if (cc->IsGround() && m_AccelVec.y <= 0) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}
}

void EnemyOrc::UpperDownFinalize()
{

}

void EnemyOrc::BeatDownInitilize()
{
	m_Hp -= m_Damage;
	if (m_Hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += m_Accel;
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
}

void EnemyOrc::BeatDownUpdate()
{
	m_BattleModeParam.canChangeAttackAction = false;

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	if (cc->IsGround()) {
		ChangeBattleAction(BATTLEACTION::DOWNACTION);
	}

}

void EnemyOrc::BeatDownFinalize()
{
}

void EnemyOrc::DownInitilize()
{
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_TackleStartPos = gameObject->mTransform->Forward();//
	m_TackleStartPos.y = 0;
	m_TackleStartPos += gameObject->mTransform->WorldPosition();
}

void EnemyOrc::DownUpdate()
{
	LookPosition(m_TackleStartPos, m_RotateSpeed);
	m_BattleModeParam.canChangeAttackAction = false;
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(m_Animparam.afterAnimId)) {
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
	};
}

void EnemyOrc::DownFinalize()
{
}

void EnemyOrc::DeadInitilize()
{
	m_BattleModeParam.count = 0.0f;
	m_AccelVec += XMVectorSet(0, -10, 0, 0);
	AnimChange(ANIM_ID::ANIM_DOWN, 5.0f, false, true);
	m_DeadIsGround = false;
	gameObject->RemoveComponent<CharacterControllerComponent>();
}

void EnemyOrc::DeadUpdate()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(ANIM_ID::ANIM_DOWN)) {
		m_Isend = true;
	}
}

void EnemyOrc::DeadFinalize()
{
}

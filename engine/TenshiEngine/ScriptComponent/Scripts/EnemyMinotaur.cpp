#include "EnemyMinotaur.h"
#include "EnemyEbilEye.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "../h_standard.h"
#include "../h_component.h"
#include "PlayerController.h"
#include "UniqueObject.h"
#include <sstream>
#include "MossanBall.h"
#include <random>
EnemyMinotaur::EnemyMinotaur()
{

	m_hip_z = 0.0f;
	


	//BattleMode戦闘
	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyMinotaur::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyMinotaur::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyMinotaur::BattleModeFinalize, this);
	//Confrontaction警戒
	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyMinotaur::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyMinotaur::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyMinotaur::ConfrontModeFinalize, this);

	//死んだとき
	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyMinotaur::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyMinotaur::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyMinotaur::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::TRACKINGMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
	InitThoughRoutineParam();
}
void EnemyMinotaur::ChildInitialize()
{
	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
	m_before_pos = gameObject->mTransform->WorldPosition();
}


void EnemyMinotaur::SoloAction()
{
	if (m_AccelVec.y >= 0 || m_BattleModeParam.id == BATTLEACTION::UPPERDOWNACTION || m_BattleModeParam.id == BATTLEACTION::DEADACTION) {
		m_Gravity = XMVectorSet(0, -9.8f, 0, 0);
	}
	else {
		m_Gravity = XMVectorSet(0, 0, 0, 0);
	}
}

ENEMY_TYPE EnemyMinotaur::GetEnemyType()
{
	return ENEMY_TYPE::PARENT;
}

float EnemyMinotaur::GetOnBattleRange()
{
	return 0.0f;
}

void EnemyMinotaur::Attack(GameObject player, COL_TYPE colType)
{
	if (!player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
	//if (!m_RotateEnd) {
	//	if (colType == COL_TYPE::NORMAL && m_BattleModeParam.id == BATTLEACTION::ATTACKDOWNACTION) {
	//		playerScript->Damage(1.0f, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Low);
	//	}
	//	else if (colType == COL_TYPE::ROTATE && m_BattleModeParam.id == BATTLEACTION::ATTACKMONCKEYACTION) {
	//		playerScript->Damage(1.0f, XMVector3Normalize(player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
	//	}
	//}
}

bool EnemyMinotaur::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
		return true;
	}
	return false;
}

bool EnemyMinotaur::DiscoveryPlayer()
{

	return false;
}

bool EnemyMinotaur::LostPlayer()
{

	return false;
}

void EnemyMinotaur::ChildFinalize()
{
	gameObject->RemoveComponent<CharacterControllerComponent>();
	////gameObject->Disable();
	Hx::Debug()->Log(gameObject->Name());
	Hx::DestroyObject(this->gameObject);
}

void EnemyMinotaur::BattleModeInitilize()
{

	//攻撃
	Hx::Debug()->Log("戦闘");
	if (m_roucine_module.GetStateParam() == 2)Attack1();
	battleActionInitilize[m_BattleModeParam.id]();
}
int num = 0;
void EnemyMinotaur::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();

	if (Input::Trigger(KeyCode::Key_0)) {
		num++;
		if (num > 17) {
			num = 0;
		}
		auto anim = m_ModelObject->GetComponent<AnimationComponent>();
		AnimChange(num, 1.0f, false, true);
		Hx::Debug()->Log(std::to_string(num));
	}
	PositionReplaceBorn();
	if (Input::Trigger(KeyCode::Key_9)) {
		Attack5();
	}
}

void EnemyMinotaur::BattleModeFinalize()
{
}

void EnemyMinotaur::ConfrontModeInitilize()
{
	//警戒状態は待機アニメーションを再生
	Hx::Debug()->Log("警戒");
}

void EnemyMinotaur::ConfrontModeUpdate()
{
	//プレイヤーの位置を考える時間
	m_thinking_time += Hx::DeltaTime()->GetDeltaTime();
	if (m_thinking_time >= 5) {
		//プレイヤーがバトル範囲以外だったら
		if (false) {

		}
	}
}

void EnemyMinotaur::ConfrontModeFinalize()
{
}

void EnemyMinotaur::DeadInitilize()
{

}

void EnemyMinotaur::DeadUpdate()
{
}

void EnemyMinotaur::DeadFinalize()
{
}

void EnemyMinotaur::Look()
{
	if (auto p = UniqueObject::GetPlayer()) {
		auto myPos = gameObject->mTransform->WorldPosition();
		auto moveVec = p->mTransform->WorldPosition();
		auto naviVec = XMVector3Normalize(moveVec - myPos);
		naviVec.y = 0;
		m_Forward = gameObject->mTransform->Forward();
		auto cross = XMVector3Normalize(XMVector3Cross(m_Forward, naviVec));
		m_View = acos(clamp(XMVector3Dot(m_Forward, naviVec).x, -1.0f, 1.0f));
		if (m_View < 0.1f)m_View = 0.0f;
		auto trackingNowAngle = 10 * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (m_View < trackingNowAngle)
			trackingNowAngle = m_View;
		gameObject->mTransform->WorldQuaternion(
			XMQuaternionMultiply(gameObject->mTransform->WorldQuaternion(), XMQuaternionRotationAxis(cross, trackingNowAngle)));
		auto myAngle = gameObject->mTransform->Rotate();
		myAngle.z = 0;
		gameObject->mTransform->Rotate(myAngle);
	}
}

void EnemyMinotaur::PositionReplaceBorn()
{
	float time = Hx::DeltaTime()->GetDeltaTime();
	float cut_anim_time = 40.0f / 60.0f;
	XMVECTOR move_velocity = XMVectorZero();
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	
	float y = gameObject->mTransform->WorldPosition().y;
	XMVECTOR pos = m_TargetBone->mTransform->WorldPosition();
	pos.y = y + 0.001f;

	if (anim->IsAnimationEnd(num)) {
		AnimChange(17, 1.0f, false, true);
		AnimChange(num, 1.0f, false, true);
		cc->Teleport(pos);
		AnimeParam ap = anim->GetAnimetionParam(num);
		ap.mTime = 0.0f;
		anim->SetAnimetionParam(num,ap);

	}

}

void EnemyMinotaur::Move()
{

	auto player=UniqueObject::GetPlayer();
	auto nav = gameObject->GetComponent<NaviMeshComponent>();
	
	if (!player)return;
	if (!nav)return;/*
	Hx::Debug()->Log("Move");
	nav->RootCreate(gameObject,player);*/

	nav->Move(1.0f * Hx::DeltaTime()->GetDeltaTime());
	auto dir=nav->GetRouteVector();
	dir=XMVector3Normalize(dir);
	LookPosition(nav->GetPosition(), 15.0f);
	gameObject->GetComponent<CharacterControllerComponent>()->Move(dir*Hx::DeltaTime()->GetDeltaTime());
}

void EnemyMinotaur::ThoughtRoutine()
{
	if (auto p = UniqueObject::GetPlayer()) {
		auto player_pos = p->mTransform->WorldPosition();
		auto now_pos = gameObject->mTransform->WorldPosition();
		auto distance = std::abs(XMVector3Length(player_pos - now_pos).x);
		//Hx::Debug()->Log(std::to_string(distance));
		std::function<void()> change_func;

		int state = m_roucine_module.GetStateParam();
		
		//バトルモードに変更
		if (m_roucine_module.DistanceCheck(DistanceType::DSI_ATTACK, distance, funifuni::ModuleDistanceType::In)) {
			m_roucine_module.SetStateParam(StateType::STATE_BATTLE);
			change_func = [this](){ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION); };
		}else if (m_roucine_module.DistanceCheck(DistanceType::DSI_MOVE, distance, funifuni::ModuleDistanceType::In)) {
			m_roucine_module.SetStateParam(StateType::STATE_BATTLE_MOVE);
			//プレイヤー追跡モードに変更
			change_func = [this]() {ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION); };

		}


		if (m_roucine_module.GetStateParam() != state)change_func();

	}

}

void EnemyMinotaur::InitThoughRoutineParam()
{
	m_roucine_module.SetDistanceParam(DistanceType::DSI_ATTACK, 10.0f);//in
	m_roucine_module.SetDistanceParam(DistanceType::DSI_E_ATTACK, 5.0f);
	m_roucine_module.SetDistanceParam(DistanceType::DSI_MOVE, 30.0f);//in
	m_roucine_module.SetDistanceParam(DistanceType::DSI_SEARCH, 30.0f);//over

	m_roucine_module.SetStateParam(StateType::STATE_NO_SEARCH);
}

void EnemyMinotaur::Attack1()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(4, 1.0f, false, true);
	auto obj = Hx::Instance(m_Attack1Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	obj->mTransform->WorldPosition(pos);
	Look();
}

void EnemyMinotaur::Attack2()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(6, 1.0f, false, true);

	auto obj = Hx::Instance(m_Attack2Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	obj->mTransform->WorldPosition(pos);
	auto rot = gameObject->mTransform->Rotate();
	obj->mTransform->Rotate(rot);
}
//とげを出す奴
void EnemyMinotaur::Attack3()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(5, 1.0f, false, true);
	auto obj = Hx::Instance(m_Attack3Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*3.0f;
	obj->mTransform->WorldPosition(pos);

}

void EnemyMinotaur::Attack4()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(8, 1.0f, false, true);

	auto obj = Hx::Instance(m_Attack4Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	obj->mTransform->WorldPosition(pos);
	auto rot = gameObject->mTransform->Rotate();
	obj->mTransform->Rotate(rot);
}

void EnemyMinotaur::Attack5()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(9, 1.0f, false, true);
	auto ap = anim->GetAnimetionParam(9);
	ap.mTimeScale = 2.0f;
	anim->SetAnimetionParam(9,ap);
	auto obj = Hx::Instance(m_Attack5Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	obj->mTransform->WorldPosition(pos);
	auto rot = gameObject->mTransform->Rotate();
	obj->mTransform->Rotate(rot);
}

void EnemyMinotaur::Attack6()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(10, 1.0f, false, true);
	auto obj = Hx::Instance(m_Attack6Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	pos.y += 2.0f;
	obj->mTransform->WorldPosition(pos);
	obj->GetScript<MossanBall>()->SetDir(gameObject->mTransform->Forward());

	auto obj1 = Hx::Instance(m_Attack2Effect);
	auto pos1 = gameObject->mTransform->WorldPosition();
	pos1.y -= 0.3f;
	auto front1 = gameObject->mTransform->Forward();
	pos1 += front1*2.0f;
	obj1->mTransform->WorldPosition(pos1);
	auto rot = gameObject->mTransform->Rotate();
	obj1->mTransform->Rotate(rot);
}

void EnemyMinotaur::Attack7()
{
	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(14, 1.0f, false, true);
	auto obj = Hx::Instance(m_Attack7Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	obj->mTransform->WorldPosition(pos);
}

void EnemyMinotaur::None()
{
}

void EnemyMinotaur::PlayerSearch()
{
	auto player = UniqueObject::GetPlayer();
	auto nav = gameObject->GetComponent<NaviMeshComponent>();
	if (!player)return;
	if (!nav)return;
	nav->RootCreate(gameObject, player);
}

void EnemyMinotaur::SetAttackAction()
{
}

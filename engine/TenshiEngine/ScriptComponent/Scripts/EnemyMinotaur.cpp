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
	//BattleMode�퓬
	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyMinotaur::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyMinotaur::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&EnemyMinotaur::BattleModeFinalize, this);
	//Confrontaction�x��
	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyMinotaur::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyMinotaur::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&EnemyMinotaur::ConfrontModeFinalize, this);

	//���񂾂Ƃ�
	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&EnemyMinotaur::DeadInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&EnemyMinotaur::DeadUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&EnemyMinotaur::DeadFinalize, this);

	m_ActionModeID = ACTIONMODE::BATTLEMODE;
	m_TrackingModeParam.id = TRACKINGACTION::PARENTTRACKING;
	m_BattleModeParam.id = BATTLEACTION::CONFRONTACTION;
	anim_loop = true;
	is_change_attack = false;
}
void EnemyMinotaur::ChildInitialize()
{

	m_Hp = hp;
	m_MaxHp = hp;
	ModelObject = m_ModelObject;
	m_before_pos = gameObject->mTransform->WorldPosition();
	Hx::Debug()->Log("CI");
	ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
	InitThoughRoutineParam();
	m_action_func = [this]() {Move(walk_speed); };
	m_attack_flag = false;
	recast = 0.0f;
	change_battle_time = 0.0f;
	change_battle_counter = 0;
	anim_cast = 0.0f;
	is_changed_take_over = false;
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
	if (!m_Player)return;
	auto playerScript = player->GetScript<PlayerController>();
	if (!playerScript)return;
}

bool EnemyMinotaur::Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_)
{
	m_Damage = damage_;
	m_Accel = accelPower_;
	if (m_BattleModeParam.id != BATTLEACTION::DOWNACTION && m_BattleModeParam.id != BATTLEACTION::DEADACTION) {
		//ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, winceType_);
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
	//�U��
	if (m_roucine_module.GetStateParam() == StateType::STATE_BATTLE)Attack1();

	auto nav = gameObject->GetComponent<NaviMeshComponent>();
	if (!m_Player)return;
	if (!nav)return;
	nav->RootCreate(gameObject, m_Player);
	battleActionInitilize[m_BattleModeParam.id]();

	
	std::string debug_draw_anim[18]{
		"ANIM_F_WALK","ANIM_B_WALK","ANIM_L_WALK","ANIM_R_WALK",
		"ANIM_ATTACK1","ANIM_ATTACK2","ANIM_ATTACK3","ANIM_BLOCK",
		"ANIM_ATTACK4","ANIM_ATTACK5","ANIM_ATTACK6","ANIM_DYING",
		"ANIM_REACT_LARGE_BUT","ANIM_F_RUN","ANIM_ATTACK7","ANIM_STUNNED",
		"ANIM_TAUNT","ANIM_CHEST_THUMP"
	};

	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	AnimChange(m_roucine_module.GetAnimState(), 10.0f,anim_loop, false);
	Hx::Debug()->Log("Anim:" + debug_draw_anim[m_roucine_module.GetAnimState()]+":Loop:"+std::to_string(anim_loop));
	
}
void EnemyMinotaur::BattleModeUpdate()
{
	battleActionUpdate[m_BattleModeParam.id]();
	
	if (is_change_attack) {
		BattleRoutine();
	}
	else {
		HuntRoutine();
	}
	//�U�����̃A�j���[�V�������I���܂�
	if (auto anim = m_ModelObject->GetComponent<AnimationComponent>()) {
		if (anim->IsAnimationEnd(m_anim_state)&m_attack_flag) {
			m_attack_flag = false;
			if (is_changed_take_over) {
				is_changed_take_over = false;
				m_action_func = [this]() {MoveSide(); };
				anim_loop = true;
				RoutineSetUp(ANIM_R_WALK);
			}
		}
	}


}

void EnemyMinotaur::BattleModeFinalize()
{
}

void EnemyMinotaur::ConfrontModeInitilize()
{
	//�x����Ԃ͑ҋ@�A�j���[�V�������Đ�

}

void EnemyMinotaur::ConfrontModeUpdate()
{
	//�v���C���[�̈ʒu���l���鎞��
	//m_thinking_time += Hx::DeltaTime()->GetDeltaTime();
	//auto p = m_Player;
	//if (!p)return;
	//auto player_pos = p->mTransform->WorldPosition();
	//auto now_pos = gameObject->mTransform->WorldPosition();
	//auto distance = std::abs(XMVector3Length(player_pos - now_pos).x);
	//if (m_thinking_time >= 5.0f) {
	//	//�v���C���[���o�g���͈͈ȊO��������
	//	if (m_roucine_module.DistanceCheck(DistanceType::DSI_ATTACK, distance, funifuni::ModuleDistanceType::Over)) {
	//		m_thinking_time = 0.0f;
	//		//ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
	//	}
	//}



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

void EnemyMinotaur::PositionReplaceBorn()
{
	//float time = Hx::DeltaTime()->GetDeltaTime();
	//float cut_anim_time = 40.0f / 60.0f;
	//XMVECTOR move_velocity = XMVectorZero();
	//auto anim = m_ModelObject->GetComponent<AnimationComponent>();

	//auto cc = gameObject->GetComponent<CharacterControllerComponent>();

	//float y = gameObject->mTransform->WorldPosition().y;
	//XMVECTOR pos = m_TargetBone->mTransform->WorldPosition();
	//pos.y = y + 0.001f;

	//if (anim->IsAnimationEnd(num)) {
	//	AnimChange(17, 1.0f, false, true);
	//	AnimChange(num, 1.0f, false, true);
	//	cc->Teleport(pos);
	//	AnimeParam ap = anim->GetAnimetionParam(num);
	//	ap.mTime = 0.0f;
	//	anim->SetAnimetionParam(num, ap);
	//}

}

void EnemyMinotaur::HuntRoutine()
{
	if (!m_Player)return;

	//���[�`���̏���
	if (!m_attack_flag) {

		//�A���U���֎~�̂��߂̕ϐ�
		recast += Hx::DeltaTime()->GetDeltaTime();

		auto distance = XMVector3Length(m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x;
		//�����U��������
		if (m_roucine_module.DistanceCheck(DSI_EM_ATTACK, distance, funifuni::ModuleDistanceType::In) && RecastCheck()) {
			m_action_func = [this]() {Attack2(); };
			recast = 0.0f;
			m_attack_flag = true;
			anim_loop = false;
			RoutineSetUp(ANIM_ATTACK2);
		}//�U���͈͓��ɂ�����
		else if (m_roucine_module.DistanceCheck(DSI_SIDE, distance, funifuni::ModuleDistanceType::In)) {
			m_action_func = [this]() {MoveSide(); };
			anim_loop = true;
			RoutineSetUp(ANIM_R_WALK);
		}//�U�����ȊO�ɋ߂Â��ꂽ�牺����
		else if (m_roucine_module.DistanceCheck(DSI_BACK, distance, funifuni::ModuleDistanceType::In)) {

			m_action_func = [this]() {MoveBack(); };
			anim_loop = true;
			RoutineSetUp(ANIM_B_WALK);
		}//�U���͈͊O�ɏo�Ă�����
		else if (m_roucine_module.DistanceCheck(DSI_MOVE, distance, funifuni::ModuleDistanceType::Over)) {
			m_action_func = [this]() {Move(walk_speed); };
			anim_loop = true;
			RoutineSetUp(ANIM_F_WALK);
		}
		//�������Ɉړ������Ƃ��Ƀo�g�����[�h�ɐ؂�ւ��邽�߂̃^�C�}�[
		if (m_roucine_module.GetAnimState() == ANIM_R_WALK ||
			m_roucine_module.GetAnimState() == ANIM_L_WALK) {
			change_battle_time += Hx::DeltaTime()->GetDeltaTime();
		}
		//�ǐՃ��[�h����o�g�����[�h��
		if (change_battle_time > 2.0f) {
			change_battle_time = 0.0f;
			is_change_attack = true;
		}

		if (m_action_func)m_action_func();
		m_anim_state = m_roucine_module.GetAnimState();
	}
}

void EnemyMinotaur::BattleRoutine()
{
	if (!m_Player)return;
	if (!m_attack_flag) {

		recast = max_recast;// += Hx::DeltaTime()->GetDeltaTime();
		auto distance = XMVector3Length(m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x;
		Hx::Debug()->Log(std::to_string(recast));

		if (m_roucine_module.DistanceCheck(DSI_BATTLE_ATTACK,distance,funifuni::ModuleDistanceType::In) && RecastCheck()) {
			m_action_func = [this]() {Attack3(); };
			recast = 3.0f;
			m_attack_flag = true;
			anim_loop = false;
			change_battle_counter++;
			is_changed_take_over = true;
			RoutineSetUp(ANIM_ATTACK3);
		}
		//else if (m_roucine_module.DistanceCheck(DSI_BATTLE_BACK, distance, funifuni::ModuleDistanceType::In)) {
		//	m_action_func = [this]() {MoveBack(); };
		//	anim_loop = true;
		//	RoutineSetUp(ANIM_B_WALK);
		//}//�U���ł��鋗���ɓ�������U��
		//else if (m_roucine_module.DistanceCheck(DSI_BATTLE_SIDE, distance, funifuni::ModuleDistanceType::In)) {
		//	m_action_func = [this]() {MoveSide(); };
		//	anim_loop = true;
		//	RoutineSetUp(ANIM_R_WALK);
		//}
		//�U���ł��Ȃ�������������ړ�����
		else {
			m_action_func = [this]() {Move(walk_speed-3.0f); };
			anim_loop = true;
			RoutineSetUp(ANIM_F_WALK);
		}

		if (m_action_func)m_action_func();
		m_anim_state = m_roucine_module.GetAnimState();
		if (change_battle_counter >= 1) {
			change_battle_counter = 0;
			change_battle_time = 0.0f;
			is_change_attack = false;
		}
	}
}

void EnemyMinotaur::RoutineSetUp(AnimType type)
{
	m_roucine_module.SetAnimState(type);
	if (m_anim_state != m_roucine_module.GetAnimState()) {
		Hx::Debug()->Log("BattleChange");
		ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);

	}
}

void EnemyMinotaur::Move(float s)
{

	auto player = m_Player;
	auto nav = gameObject->GetComponent<NaviMeshComponent>();

	if (!player)return;
	if (!nav)return;/*
					Hx::Debug()->Log("Move");
					nav->RootCreate(gameObject,player);*/


	auto dir = nav->GetRouteVector();
	dir = XMVector3Normalize(dir);
	nav->Move(7.0f* Hx::DeltaTime()->GetDeltaTime());
	LookPosition(player->mTransform->WorldPosition(), 270.0f, false);
	gameObject->GetComponent<CharacterControllerComponent>()->Move(gameObject->mTransform->Forward()*Hx::DeltaTime()->GetDeltaTime()*s);
}
void EnemyMinotaur::MoveSide(bool right)
{
	auto player = m_Player;
	if (!player)return;
	LookPosition(player->mTransform->WorldPosition(), 360.0f, false);
	auto vec = (right) ? -gameObject->mTransform->Left() : gameObject->mTransform->Left();
	gameObject->GetComponent<CharacterControllerComponent>()->Move(gameObject->mTransform->Left()*Hx::DeltaTime()->GetDeltaTime()*side_speed);
}

void EnemyMinotaur::MoveBack()
{
	auto player = m_Player;
	if (!player)return;
	LookPosition(player->mTransform->WorldPosition(), 270.0f, false);
	gameObject->GetComponent<CharacterControllerComponent>()->Move(-gameObject->mTransform->Forward() *Hx::DeltaTime()->GetDeltaTime()*back_speed);

}

bool EnemyMinotaur::RecastCheck()
{
	return (max_recast<=recast);
}

void EnemyMinotaur::ThoughtRoutine()
{
	if (auto p = m_Player) {
		auto player_pos = p->mTransform->WorldPosition();
		auto now_pos = gameObject->mTransform->WorldPosition();
		auto distance = std::abs(XMVector3Length(player_pos - now_pos).x);
		//Hx::Debug()->Log(std::to_string(distance));

		int state = m_roucine_module.GetStateParam();
		std::function<void()> change_func;
		//�o�g�����[�h�ɕύX
		if (m_roucine_module.DistanceCheck(DistanceType::DSI_ATTACK, distance, funifuni::ModuleDistanceType::In)) {
			m_roucine_module.SetStateParam(StateType::STATE_BATTLE);
			change_func = [this]() {ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION); };
		}//�A�C�h����Ԃ̃��[�V����
		else if (m_roucine_module.DistanceCheck(DistanceType::DSI_MOVE, distance, funifuni::ModuleDistanceType::In)) {
			m_roucine_module.SetStateParam(StateType::STATE_BATTLE_MOVE);
			change_func = [this]() {ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION); };
		}//�ړ���Ԃ̃��[�V����
		else if (m_roucine_module.DistanceCheck(DistanceType::DSI_MOVE, distance, funifuni::ModuleDistanceType::In)) {
			m_roucine_module.SetStateParam(StateType::STATE_BATTLE_MOVE);
			//�v���C���[�ǐՃ��[�h�ɕύX
			change_func = [this]() {ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION); };

		}
		if (m_roucine_module.GetStateParam() != state)change_func();

	}

}

void EnemyMinotaur::InitThoughRoutineParam()
{
	//�ǐ՗p�̋���
	m_roucine_module.SetDistanceParam(DistanceType::DSI_EM_ATTACK, 4.5f);
	m_roucine_module.SetDistanceParam(DistanceType::DSI_ATTACK, 6.0f);//in
	m_roucine_module.SetDistanceParam(DistanceType::DSI_SIDE, 6.0f);//in
	m_roucine_module.SetDistanceParam(DistanceType::DSI_MOVE, 8.0f);//in
	m_roucine_module.SetDistanceParam(DistanceType::DSI_BACK, 5.0f);//in

	//�o�g���p�̋���
	m_roucine_module.SetDistanceParam(DistanceType::DSI_SEARCH, 1000.0f);//over
	m_roucine_module.SetDistanceParam(DistanceType::DSI_BATTLE_RUN, 6.0f);
	m_roucine_module.SetDistanceParam(DistanceType::DSI_BATTLE_ATTACK, 5.0f);
	m_roucine_module.SetDistanceParam(DistanceType::DSI_BATTLE_SIDE, 5.0f);
	m_roucine_module.SetDistanceParam(DistanceType::DSI_BATTLE_BACK, 4.0f);

	m_roucine_module.SetStateParam(StateType::STATE_NO_SEARCH);
	m_roucine_module.SetAnimState(AnimType::ANIM_F_WALK);


	auto anim = m_ModelObject->GetComponent<AnimationComponent>();
	float animspeeds[18] = {
		2.0f,1.0f,1.5f,1.5f,
		2.0f,2.0f,2.0f,2.0f,
		2.0f,2.0f,2.0f,2.0f,
		1.0f,1.0f,1.0f,1.0f,
		2.0f,2.0f
	};
	for (int i = 0; i < 18; ++i) {
		auto p = anim->GetAnimetionParam(i);
		p.mTimeScale = animspeeds[i];
		anim->SetAnimetionParam(i, p);
	}
}

void EnemyMinotaur::Attack1()
{
	auto obj = Hx::Instance(m_Attack1Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	obj->mTransform->WorldPosition(pos);
	m_action_func = nullptr;
}

void EnemyMinotaur::Attack2()
{

	auto obj = Hx::Instance(m_Attack2Effect);
	auto pos = gameObject->mTransform->WorldPosition();
	pos.y -= 0.3f;
	auto front = gameObject->mTransform->Forward();
	pos += front*2.0f;
	obj->mTransform->WorldPosition(pos);
	auto rot = gameObject->mTransform->Rotate();
	obj->mTransform->Rotate(rot);
	m_action_func = nullptr;
}
//�Ƃ����o���z
void EnemyMinotaur::Attack3()
{
	if (!m_Player)return;
	auto obj = Hx::Instance(m_Attack3Effect);
	auto pos = m_Player->mTransform->WorldPosition();
	obj->mTransform->WorldPosition(pos);
	m_action_func = nullptr;
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
	anim->SetAnimetionParam(9, ap);
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
	m_action_func = nullptr;
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
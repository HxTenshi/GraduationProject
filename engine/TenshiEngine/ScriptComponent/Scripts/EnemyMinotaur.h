
#pragma once
#include "main.h"
#include "Enemy.h"
#include <map>

namespace funifuni
{
	enum ModuleDistanceType {
		In,
		Over
	};
	template<class T, class U>
	class ThoughtRoutineModule {
	public:

		ThoughtRoutineModule() {

		}
		template<class T>
		void SetDistanceParam(T id, float d) {
			m_distance.insert(std::pair<T, float>(id, d));
		}
		template<class T>
		float GetDistanceParam(T id) {
			if (m_distance.find(id) == m_distance.end())return 0.0f;
			return m_distance[id];

		}
		template<class T>
		bool DistanceCheck(T id, float d, ModuleDistanceType type = ModuleDistanceType::In) {
			return (type == ModuleDistanceType::In) ? (GetDistanceParam(id) >= d) : (GetDistanceParam(id) <= d);
		}
		void SetStateParam(int s) {
			state = s;
		}

		int GetStateParam() {
			return state;
		}
		void SetAnimState(int s) {
			anim_state = s;
		}
		int GetAnimState() {
			return anim_state;
		}
		template<class U>
		void SetFlag(U id, bool f) {
			m_flags.insert(std::pair<U, bool>(id, f));
		}
		template<class U>
		bool GetFlag(U id) {
			if (m_flags.find(id) == m_distance.end())return false;
			return m_flags[id];
		}
	private:
		std::map<T, int> m_distance;
		std::map<U, bool> m_flags;
		int state;
		int anim_state;
	};

}
class EnemyMinotaur :public Enemy {
public:
	EnemyMinotaur();
	void ChildInitialize() override;
	void SoloAction() override;
	ENEMY_TYPE GetEnemyType() override;
	float GetOnBattleRange() override;
	void Attack(GameObject player, COL_TYPE colType) override;
	bool Damage(float damage_, BATTLEACTION::Enum winceType_, XMVECTOR accelPower_) override;
	bool DiscoveryPlayer() override;
	bool LostPlayer() override;
	void ChildFinalize() override;
private:
	enum StateType {
		STATE_NO_SEARCH,
		STATE_SEARCH,
		STATE_BATTLE,
		STATE_BATTLE_MOVE,
		STATE_ANGREE,
		STATE_AWAKENING
	};
	enum DistanceType {
		DSI_MOVE,
		DSI_BACK,
		DSI_EM_ATTACK,
		DSI_ATTACK,
		DSI_SIDE,
		DSI_E_ATTACK,
		DSI_SEARCH,
		DSI_BATTLE_ATTACK,
		DSI_BATTLE_RUN,
		DSI_BATTLE_SIDE,
		DSI_BATTLE_BACK
	};
	enum FlagType {
		FLAG_ANGREE,
		FLAG_MOVED,
		FLAG_THINKING,
	};
	enum AnimType {
		ANIM_F_WALK = 0,ANIM_B_WALK,ANIM_L_WALK,ANIM_R_WALK,
		ANIM_ATTACK1,ANIM_ATTACK3,ANIM_ATTACK2,ANIM_BLOCK,
		ANIM_ATTACK4,ANIM_ATTACK5,ANIM_ATTACK6,ANIM_DYING,
		ANIM_REACT_LARGE_BUT,ANIM_F_RUN,ANIM_ATTACK7,ANIM_STUNNED,
		ANIM_TAUNT,ANIM_CHEST_THUMP,
	};

private:
	void BattleModeInitilize();
	void BattleModeUpdate();
	void BattleModeFinalize();

	void ConfrontModeInitilize();
	void ConfrontModeUpdate();
	void ConfrontModeFinalize();

	void DeadInitilize();
	void DeadUpdate();
	void DeadFinalize();
	//�����E�Q�֐�
	void DebugDead();
	//�o�����̐ݒ�֐�
	void EnemyEmergence(bool flag);


	//�v���C���[�ǐՒ��̃��[�`��
	void HuntRoutine();
	//�v���C���[�Ƃ̃o�g���p���[�`��
	void BattleRoutine();
	//���[�`���̃A�j���[�V�������ς�����Ƃ��̐ݒ�֐�
	void RoutineSetUp(AnimType type);
	void Move(float s);
	//�U�����̑O�ړ�����
	void MoveAttackd(float s);
	void MoveSide(bool right = true);
	void MoveBack();
	//�A���U���֎~�̂��߂̊֐�
	bool RecastCheck();
	//���鋗����������
	void InitThoughRoutineParam();

	
	//TODO:�����蔻��͂܂��Ȃ�
	void Attack1();
	void Attack2();
	void Attack3();
	void Attack4();
	void Attack5();
	void Attack6();
	void Attack7();
	void ChestThump();
	void Stan();
	void PlayerSearch();

	void SetAttackAction();

	//�v�l���[�`�����Ǘ����邽�߂̓Ǝ��N���X
	funifuni::ThoughtRoutineModule<DistanceType, FlagType> m_roucine_module;
	SERIALIZE float hp;
	//�A�j���[�V�����������Ă郂�f��
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE GameObject m_TargetBone;
	//�U���̃G�t�F�N�g
	SERIALIZE PrefabAsset m_Attack1Effect;//id 4
	SERIALIZE PrefabAsset m_Attack2Effect;//id 5
	SERIALIZE PrefabAsset m_Attack3Effect;//id 6
	SERIALIZE PrefabAsset m_Attack4Effect;//id 8
	SERIALIZE PrefabAsset m_Attack5Effect;//id 9
	SERIALIZE PrefabAsset m_Attack6Effect;//id 10
	SERIALIZE PrefabAsset m_Attack7Effect;//id 14
	SERIALIZE PrefabAsset m_DeadEffect;
	SERIALIZE PrefabAsset m_Archer;//Archer
	SERIALIZE GameObject m_AngerEffect;//�{���Ԃɓ��������ɏo������G�t�F�N�g
	SERIALIZE GameObject m_AngerMesh;//�{���Ԃɓ������Ƃ��ɔ��ɐԂ݂���������
	//�f�o�b�O�p�̃t���O
	SERIALIZE bool m_Debug_flag;


	//�O�����̃X�s�[�h
	SERIALIZE float walk_speed;
	//�������̃X�s�[�h
	SERIALIZE float back_speed;
	//�������̃X�s�[�h
	SERIALIZE float side_speed;
	//�ǐՒ��ɔ����U�����Ă��玟�̔����U��������܂ł̎���
	SERIALIZE float max_recast;
	//�~�m�^�E���X�̍U���p�̃t�@���N�V����
	std::function<void(void)> m_attack_func;
	//�o�g�����[�h����ǐՃ��[�h�ւ̃t�@���N�V�����ύX����Ƃ��ɃA�j���[�V�����������Ȃ����C������t���O
	bool is_changed_take_over;
	//�~�m�^�E���X�̍s�������E����t�@���N�V����
	std::function<void()> m_action_func;
	//�U�����̈ړ��p�t�@���N�V����
	std::function<void()> m_attackd_func;
	//�ǐՎ��̍s���A�j���[�V�����̕ύX����
	float anim_cast;
	//�܂��g���ĂȂ�
	float m_thinking_time;
	//�O�̃t���[���̃A�j���[�V�����Ɣ�r���邽�߂̕ϐ�
	int m_anim_state;
	//���ݗp�̃J�E���^
	int stan_count;
	//�U�������Ƃ��ɕʂ̊֐�������Ȃ��悤�ɒ������邽�߂̃t���O
	bool m_attack_flag;
	//�ǐՒ��̋߂Â������̔����U���̃L���X�g
	float recast;
	bool anim_loop;
	//�ǐՃ��[�h�ƃo�g�����[�h��؂�ւ���t���O
	bool is_change_attack;
	//�ǐՂ���o�g���ɋA�邽�߂̃^�C���i�T�C�h�ړ��̎��̂݉��Z�j
	float change_battle_time;
	//�o�g������ǐՂɐ؂�ւ��邽�߂̃J�E���^�[
	int change_battle_counter;
	XMVECTOR m_before_pos;
	float m_hip_z;
	//���񂾂Ƃ��̃t���O
	bool is_dead;
	//�_���[�W���������Ƃ��̃t���O
	bool is_damage;
	//���񂾂Ƃ��ɃG�t�F�N�g�𐶐�����^�C�}�[
	float m_crate_dead_effect_timer;
	//���S��ԂŃA�j���[�V�������I�������
	bool is_dead_anim_end;
	//�{���Ԃɓ���t���O
	bool is_anger;
	//�U���ړ����̃^�C�}�[
	float m_move_attackd_timer;
	//�U���ړ����̃^�C�}�[���
	float m_move_attackd_timemax;
	//�U������������Ƃ��̃J�E���^
	int m_damage_counter;
	//�o�����̃t���O
	SERIALIZE
	bool m_emergence_flag;
};
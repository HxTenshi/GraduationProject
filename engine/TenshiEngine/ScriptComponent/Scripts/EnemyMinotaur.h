
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
	template<class T,class U>
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
	};

}
class EnemyMinotaur:public Enemy {
public :
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
	enum ANIM_ID {
		
	};
	void BattleModeInitilize();
	void BattleModeUpdate();
	void BattleModeFinalize();

	void ConfrontModeInitilize();
	void ConfrontModeUpdate();
	void ConfrontModeFinalize();

	void DeadInitilize();
	void DeadUpdate();
	void DeadFinalize();


	void Look();
	void PositionReplaceBorn();

	void Move();
	//évçlä÷êî
	void ThoughtRoutine();
	void InitThoughRoutineParam();
	//â°êÿÇË
	//TODO:Ç†ÇΩÇËîªíËÇÕÇ‹ÇæÇ»Ç¢
	void Attack1();
	void Attack2();
	void Attack3();
	void Attack4();
	void Attack5();
	void Attack6();
	void Attack7();
	void None();
	void PlayerSearch();

	void SetAttackAction();

private:
	enum StateType {
		STATE_NO_SEARCH,
		STATE_SEARCH,
		STATE_BATTLE,
		STATE_BATTLE_MOVE,
		STATE_ANGREE,
		STATE_AWAKENING
	};
	enum DistanceType{
		DSI_MOVE,
		DSI_ATTACK,
		DSI_E_ATTACK,
		DSI_SEARCH
	};
	enum FlagType {
		FLAG_ANGREE,
		FLAG_MOVED,
		FLAG_THINKING,
	};

	funifuni::ThoughtRoutineModule<DistanceType, FlagType> m_roucine_module;

	SERIALIZE float hp;
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE GameObject m_TargetBone;

	SERIALIZE PrefabAsset m_Attack1Effect;//id 4
	SERIALIZE PrefabAsset m_Attack2Effect;//id 5
	SERIALIZE PrefabAsset m_Attack3Effect;//id 6
	SERIALIZE PrefabAsset m_Attack4Effect;//id 8
	SERIALIZE PrefabAsset m_Attack5Effect;//id 9
	SERIALIZE PrefabAsset m_Attack6Effect;//id 10
	SERIALIZE PrefabAsset m_Attack7Effect;//id 14

	std::function<void(void)> m_attack_func;


	float m_thinking_time;


	XMVECTOR m_before_pos;
	float m_hip_z;

};	

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
	//強制殺害関数
	void DebugDead();
	//出現時の設定関数
	void EnemyEmergence(bool flag);


	//プレイヤー追跡中のルーチン
	void HuntRoutine();
	//プレイヤーとのバトル用ルーチン
	void BattleRoutine();
	//ルーチンのアニメーションが変わったときの設定関数
	void RoutineSetUp(AnimType type);
	void Move(float s);
	//攻撃中の前移動処理
	void MoveAttackd(float s);
	void MoveSide(bool right = true);
	void MoveBack();
	//連続攻撃禁止のための関数
	bool RecastCheck();
	//測る距離を初期化
	void InitThoughRoutineParam();

	
	//TODO:あたり判定はまだない
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

	//思考ルーチンを管理するための独自クラス
	funifuni::ThoughtRoutineModule<DistanceType, FlagType> m_roucine_module;
	SERIALIZE float hp;
	//アニメーションが入ってるモデル
	SERIALIZE GameObject m_ModelObject;
	SERIALIZE GameObject m_TargetBone;
	//攻撃のエフェクト
	SERIALIZE PrefabAsset m_Attack1Effect;//id 4
	SERIALIZE PrefabAsset m_Attack2Effect;//id 5
	SERIALIZE PrefabAsset m_Attack3Effect;//id 6
	SERIALIZE PrefabAsset m_Attack4Effect;//id 8
	SERIALIZE PrefabAsset m_Attack5Effect;//id 9
	SERIALIZE PrefabAsset m_Attack6Effect;//id 10
	SERIALIZE PrefabAsset m_Attack7Effect;//id 14
	SERIALIZE PrefabAsset m_DeadEffect;
	SERIALIZE PrefabAsset m_Archer;//Archer
	SERIALIZE GameObject m_AngerEffect;//怒り状態に入った時に出現するエフェクト
	SERIALIZE GameObject m_AngerMesh;//怒り状態に入ったときに肌に赤みを持たせる
	//デバッグ用のフラグ
	SERIALIZE bool m_Debug_flag;


	//前方向のスピード
	SERIALIZE float walk_speed;
	//後ろ方向のスピード
	SERIALIZE float back_speed;
	//横方向のスピード
	SERIALIZE float side_speed;
	//追跡中に反撃攻撃してから次の反撃攻撃をするまでの時間
	SERIALIZE float max_recast;
	//ミノタウロスの攻撃用のファンクション
	std::function<void(void)> m_attack_func;
	//バトルモードから追跡モードへのファンクション変更するときにアニメーションが動かないを修正するフラグ
	bool is_changed_take_over;
	//ミノタウロスの行動を左右するファンクション
	std::function<void()> m_action_func;
	//攻撃時の移動用ファンクション
	std::function<void()> m_attackd_func;
	//追跡時の行動アニメーションの変更時間
	float anim_cast;
	//まだ使ってない
	float m_thinking_time;
	//前のフレームのアニメーションと比較するための変数
	int m_anim_state;
	//怯み用のカウンタ
	int stan_count;
	//攻撃したときに別の関数が入らないように調整するためのフラグ
	bool m_attack_flag;
	//追跡中の近づいた時の反撃攻撃のキャスト
	float recast;
	bool anim_loop;
	//追跡モードとバトルモードを切り替えるフラグ
	bool is_change_attack;
	//追跡からバトルに帰るためのタイム（サイド移動の時のみ加算）
	float change_battle_time;
	//バトルから追跡に切り替えるためのカウンター
	int change_battle_counter;
	XMVECTOR m_before_pos;
	float m_hip_z;
	//死んだときのフラグ
	bool is_dead;
	//ダメージが入ったときのフラグ
	bool is_damage;
	//死んだときにエフェクトを生成するタイマー
	float m_crate_dead_effect_timer;
	//死亡状態でアニメーションが終わったら
	bool is_dead_anim_end;
	//怒り状態に入るフラグ
	bool is_anger;
	//攻撃移動中のタイマー
	float m_move_attackd_timer;
	//攻撃移動中のタイマー上限
	float m_move_attackd_timemax;
	//攻撃をもらったときのカウンタ
	int m_damage_counter;
	//出現時のフラグ
	SERIALIZE
	bool m_emergence_flag;
};
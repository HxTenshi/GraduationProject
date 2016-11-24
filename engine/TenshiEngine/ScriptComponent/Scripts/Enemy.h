
#pragma once
#include "main.h"

struct AnimParameter {
	int beforeAnimId = 0;
	int afterAnimId = 0;
	int nowAnimId = 0;
	float nowLerpTime = 0;
	bool animLerpFlag = false;
	float lerpSpeed = 1.0f;
	bool beforeAnimLoop = true;
	bool afterAnimLoop = true;
};

struct TRACKINGACTION {
	enum Enum {
		NONE,
		PARENTTRACKING,
		CHILDTRACKING,
	};
};

struct TrackingModeParameter {
	//今のアクションのID
	TRACKINGACTION::Enum id = TRACKINGACTION::NONE;

	//前のアクションのID
	TRACKINGACTION::Enum beforeId = TRACKINGACTION::NONE;
	
	//ナビメッシュを使うかどうか
	bool naviMeshFlag = true;

	//親が生きているかどうか
	bool parentAlive = true;
};

struct BATTLEACTION {
	enum Enum {
	NONE,
	CONFRONTACTION,
	APPROACHACTION,
	ATTACKDOWNACTION,
	JUMPATTACKACTION,
	GUARDACTION,
	PROVOCATION,
	BACKSTEPACTION,
	WINCEACTION,
	HITINGUARDACTION,
	ATTACKMONCKEYACTION,
	DEADACTION
	};
};

struct BattleModeParameter{	
	//今のアクションのID
	BATTLEACTION::Enum id = BATTLEACTION::NONE;

	//前のアクションのID
	BATTLEACTION::Enum beforeId = BATTLEACTION::NONE;
	
	//バトルモード中の思考時間を図るもの
	float count = 0.0f;

	//うろうろする時間
	float decideAprochTime = 0;

	//同じアクションが何回続いたかカウント
	int sameActionCount = 0;

	//同じアクションが連続で行われている場合このアクションは一番最初か？
	bool firstInSameAction = true;

	//バトルモード中のアプローチとガード時にどちらに回転するか決める為
	bool rotateVecPlus = true;

	//アクションが終わったかどうか
	bool actionFinish = false;

	//ポジションについたか
	bool arrival = false;

	//攻撃に遷移できるアクションかどうか
	bool canChangeAttackAction = false;

	//戦いを行う場所
	XMVECTOR battlePosition = XMVectorSet(0, 0, 0, 0);
};


enum ANIM_ID{
	ANIM_IDLE,
	ANIM_WALK_FORWARD,
	ANIM_BACKSTEP,
	ANIM_ATTACK_DOWN,
	ANIM_GUARD,
	ANIM_JUMPATTACK,
	ANIM_WINCE,
	ANIM_HITINGUARD,
	ANIM_ATTACK_MONCKEY,
	ANIM_PROVOCATION,
	ANIM_ATTACK_SIDE,
	ANIM_RUSH,
	ANIM_SIDESTEPLEFT,
	ANIM_SIDESTEPRIGHT,
	ANIM_THRUST,
	ANIM_THRUSTRUN,
};

enum ACTIONMODE{
	TRACKINGMODE,
	BATTLEMODE
};

struct EnemyAllParamter{
	TrackingModeParameter trackingModeParameter;
	BattleModeParameter battleModeParameter;
	ACTIONMODE actionMode;
};

template<class T>
const T& clamp(const T& v, const T& min, const T& max) {
	if (v <= min)return min;
	else if (v >= max)return max;
	return v;
}


class Enemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	virtual void FunctionSet() = 0;
	virtual bool GetChildFlag() = 0;
	virtual float GetOnBattleRange() = 0;
	virtual void Attack(GameObject player) = 0;
	virtual void Damage(float damage_) = 0;
	virtual bool DiscoveryPlayer() = 0;
	virtual bool LostPlayer() = 0;
	void SetParentAlive(bool flag) { m_TrackingModeParam.parentAlive = flag; }
	EnemyAllParamter GetEnemyAllParameter(bool reset) {
		EnemyAllParamter eap;
		auto bmp = m_BattleModeParam;
		if (reset) {
			m_BattleModeParam.actionFinish = false;
			m_BattleModeParam.arrival = false;
		}
		eap.trackingModeParameter = m_TrackingModeParam;
		eap.battleModeParameter = bmp;
		eap.actionMode = m_ActionModeID;
		return eap;
	}
	void SetBattlePosition(XMVECTOR battlePosition_) { m_BattleModeParam.battlePosition = battlePosition_; }
	void SetPlayer(GameObject player) { m_Player = player; }
	void AnimChange(int id, float lerpSpeed, bool roop = true, bool forcingChange = false);
	float GetNowAnimTime();

private:
	void AnimLerp();

	void ChangeActionMode(ACTIONMODE nextActionMode);
	void ChangeActionAndTrackingAction(ACTIONMODE nextActionMode, TRACKINGACTION::Enum nextTrackingAction);
	void ChangeActionAndBattleAction(ACTIONMODE nextActionMode, BATTLEACTION::Enum nextBattleAction);
	void ChangeTrackingAction(TRACKINGACTION::Enum nextTrackingAction);
	void ChangeBattleAction(BATTLEACTION::Enum nextBattleAction);
	
protected:
	GameObject ModelObject;
protected:

	std::map<ACTIONMODE,std::function<void()>> actionModeInitilize;
	std::map<ACTIONMODE,std::function<void()>> actionModeUpdate;
	std::map<ACTIONMODE,std::function<void()>> actionModeFinalize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionInitilize;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionUpdate;
	std::map<TRACKINGACTION::Enum, std::function<void()>> trackingActionFinalize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionInitilize;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionUpdate;
	std::map<BATTLEACTION::Enum, std::function<void()>> battleActionFinalize;
	GameObject m_gameObject = gameObject;

	GameObject m_Player;

	//前方向
	XMVECTOR m_Forward;
	//m_Playerへのベクトル
	XMVECTOR m_PlayerVec;
	//自分の正面のベクトル自分からm_Playerへのベクトルの角度
	float m_View;
	//移動量
	XMVECTOR m_Vec;
	//受けるダメージを保存するもの
	float m_Damage;

	//捜索時の移動ポイントの数値
	int m_MoveCount;
	//捜索時の移動ポイントの方向
	bool m_MoveCountUp;


	std::vector<GameObject> m_MovePointsVec;

	//アクションモードのID
	ACTIONMODE m_ActionModeID;
	//バトル中のパラメーター
	BattleModeParameter m_BattleModeParam;
	//捜索中のパラメーター
	TrackingModeParameter m_TrackingModeParam;

	//アニメーションに必要なパラメーター
	AnimParameter m_Animparam;

	float m_ChildTranckingSpeed;
	//重力
	XMVECTOR m_Gravity;

public:
	void SetActionID(ACTIONMODE actionMode) {
		m_ActionModeID = actionMode;
	}
	void SetTrackingID(TRACKINGACTION::Enum trackingAction) {
		m_TrackingModeParam.id = trackingAction;
	}
	void SetBattleID(BATTLEACTION::Enum battleAction) {
		m_BattleModeParam.id = battleAction;
	}

	void SetActionMode(ACTIONMODE actionMode) {
		ChangeActionMode(actionMode);
	}

	void SetActionModeAndTrackingAction(ACTIONMODE actionMode, TRACKINGACTION::Enum trackingAction) {
		ChangeActionAndTrackingAction(actionMode, trackingAction);
	}

	void SetActionModeAndBattleAction(ACTIONMODE actionMode, BATTLEACTION::Enum battleAction) {
		ChangeActionAndBattleAction(actionMode, battleAction);
	}

	void SetTrackingAction(TRACKINGACTION::Enum trackingAction) {
		ChangeTrackingAction(trackingAction);
	}

	void SetBattleAction(BATTLEACTION::Enum battleAction) {
		ChangeBattleAction(battleAction);
	}


};

#pragma once
#include "main.h"
# include "WeaponControl.h"
#include "WeaponCommon.h"
#include <functional>
enum AttackType {
	WEAK,
	USUALLY,
	STRONG
};
class Weapon :public IDllScriptComponent{
public:
	struct HitState {
		enum Type {
			Damage,
			Guard,
			Counter,
			Dogde
		};
	};
	typedef std::function<void(GameObject, Weapon*, Weapon::HitState::Type)> HitCollbackType;

	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
//追加関数
	/// <summary>
	///武器へのダメージ処理
	/// </summary>
	void Damage(DamageType type,float mag);
	/// <summary>
	///壊れたか
	/// </summary>
	bool isBreak();
	/// <summary>
	///投げ捨てる
	/// </summary>
	void ThrowAway();
	/// <summary>
	///投げ捨てる
	/// </summary>
	void ThrowAttack();
	/// <summary>
	///投げ捨てる
	/// </summary>
	void ThrowAway(XMVECTOR& throwdir);
	/// <summary>
	///WeaponのPhysXを有効にする
	/// </summary>
	void WeaponUsePhysX();
	/// <summary>
	///攻撃時のコールバック関数追加
	/// </summary>
	void SetHitCollback(const HitCollbackType& collback);
	/// <summary>
	///武器の取得
	/// </summary>
	void GetWeapon();
	/// <summary>
	///攻撃力の取得
	/// </summary>
	float GetAttackPower();
	/// <summary>
	///耐久力の取得
	/// </summary>
	float GetDurable();
	/// <summary>
	///武器の捨て方の種類を指定
	/// </summary>
	WeaponType GetWeaponType();
	/// <summary>
	///武器の交換処理
	/// </summary>
	void SwapWeapon(GameObject target);
	/// <summary>
	///武器を取得できるか
	/// </summary>
	bool isGetWeapon();
private:
	/// <summary>
	///投げ捨てる時のアクション
	/// </summary>
	void ThrowAwayAction();
	void PierceSupport(GameObject obj);
//演出関係
private:
	void Break();
	void Effect();
private:
	//メンバ変数
	SERIALIZE
	float m_Recast;
	SERIALIZE
	GameObject m_ThrowHit;

	//鈴木追加
	SERIALIZE GameObject mWeaponControl;
	bool mIsEnemyThrow;
	AttackType m_attack_type;

	//武器のパラメータ
	funifuni::WeaponParametor m_param;
private:
	//テスト用
	SERIALIZE
	GameObject mSwapTarget;
private:
	float m_weapon_rot;
	bool is_hand;
	bool is_ground_hit;
	//捨てる処理が変わる時に無くなる
	bool is_fly;

	HitCollbackType m_HitCollback;
};
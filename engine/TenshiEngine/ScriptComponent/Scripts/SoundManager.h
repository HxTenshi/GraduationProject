
#pragma once
#include "main.h"

class SoundManager :public IDllScriptComponent{
public:
	struct  SoundSE_ID{
		enum Enum {
			Player_Damage = 0,
			Player_Down,
			Player_Duchroll,
			Player_Gurad,
			Player_Landing,
			Player_Recovery,
			Player_Run1,
			Player_Run2,
			Player_SP,
			Enemy_Attack,
			Enemy_Bomb,
			Enemy_Swing,
			Enemy_Ya,
			Eviro_Sound1,
			Eviro_Sound2,
			Gimmick_Cannon,
			Gimmick_Firing,
			Kan_ge_iwaoti01,
			Taur_Quake,
			Weapon_Break,
			Weapon_SP_Stab,
			Weapon_SP_Swing,
			Weapon_SW_Swing1,
			Weapon_SW_Swing2,
			Camera_Move,
			Bomb,
			Bomb2,
			Fire,
			Fire_Loop,
			WoodBreak,
			Boar_child_cry1,
			Boar_cry1,
			Cursour,
			Decision,
			Lion2,
			VolumeDown,
			VolumeUp,
			Simbal,
			DrumRoll,
			HintSound,
			____EmptySE4,//使ってどうぞ
			Rezard_Attack_1,
			Rezard_Attack_2,
			WoodMan_Attack,


			Reis_SE_Warp = 59,
			Reis_Damage = 60,
			Reis_Pop,
			Reis_Dead,
			Reis_BackStep,
			Reis_Near1,
			Reis_Near2,
			Reis_Shot,
			Reis_CitrusThrow,
			Reis_WarpNear,
			Reis_SonicWave1,
			Reis_Chage1,
			Reis_Chage2,

			Reis_SE_Near_Swing,
			Reis_SE_Burret_Spawn,
			Reis_SE_Burret_Shot,
			Reis_SE_Burret_Hit,
			Reis_SE_Citrus_Throw,
			Reis_SE_Chage1,
			Reis_SE_Chage2,
			Reis_SE_SonicWave,

			Player_SW_L1=80,
			Player_SW_L2,
			Player_SW_L3,
			Player_SW_H1,
			Player_SW_H2,
			Player_SW_FL1,
			Player_SW_FL2,
			Player_SW_FH1,
			Player_SW_PlaySP,
			Player_SW_DL,
			Player_SW_SJL,
			Player_Throw,
			Player_Drop,
			Player_SpeedJump,
			Player_CV_Damage1,
			Player_CV_Damage2,
			Player_CV_Dead,
			Player_CV_Gurad,

			Player_SE_SW_Low_Swing,
			Player_SE_SW_Low_Hit,
			Player_SE_SW_High_Swing,
			Player_SE_SW_High_Hit,
			Player_SE_R_Low_Hit,

			Player_SE_WeaponGet,
		};
	};
	struct SoundBGM_ID {
		enum Enum {
			Title = 0,
			GamePlay,
			GameClear,
			GameOver,
			Credit
		};
	};

	void Initialize()override;
	void Finish()override;

	
	static void PlaySE(SoundSE_ID::Enum key,XMVECTOR pos);
	static void PlaySE(SoundSE_ID::Enum key, XMVECTOR pos, float volume);
	static void PlaySE_2D(SoundSE_ID::Enum key);
	static void PlayBGM(SoundBGM_ID::Enum key);
	static void SetBGMVolume(float volume);
private:
	//メンバ変数
	SERIALIZE
		std::vector<SoundAsset> m_soundSEs;
	SERIALIZE
		std::vector<SoundAsset> m_soundBGMs;
	SERIALIZE
	PrefabAsset soundBox;

	GameObject m_bgmObj;
	float se_master_volume;
	float bgm_master_volume;
};

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
	static void PlayBGM(SoundBGM_ID::Enum key);
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
		std::vector<SoundAsset> m_soundSEs;
	SERIALIZE
		std::vector<SoundAsset> m_soundBGMs;
	SERIALIZE
	PrefabAsset soundBox;

	float se_master_volume;
	float bgm_master_volume;
};
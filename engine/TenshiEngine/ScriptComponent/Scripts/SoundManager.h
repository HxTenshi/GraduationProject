
#pragma once
#include "main.h"

class SoundManager :public IDllScriptComponent{
public:
	struct  SoundSE_ID{
		enum Enum {
			CursorMove = 0,
			Enter
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
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void GetSoundSE(SoundSE_ID::Enum key,XMVECTOR pos);
	void GetSoundBGM(SoundBGM_ID::Enum key);
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
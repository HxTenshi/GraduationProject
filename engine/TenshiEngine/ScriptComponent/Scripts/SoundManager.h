
#pragma once
#include "main.h"

class SoundManager :public IDllScriptComponent{
public:
	struct  SoundSE_ID{
		enum Enum {
			kiru = 0
		};
	};
	struct SoundBGM_ID {
		enum Enum {
			test = 0
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
	//ƒƒ“ƒo•Ï”
	SERIALIZE
	SoundAsset sound;
	SERIALIZE
	PrefabAsset soundBox;

	float se_master_volume;
	float bgm_master_volume;
	std::map<SoundSE_ID::Enum, SoundAsset> m_SoundSEMap;
	std::map<SoundBGM_ID::Enum, SoundAsset> m_SoundBGMMap;
};
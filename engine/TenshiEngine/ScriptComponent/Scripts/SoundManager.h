
#pragma once
#include "main.h"


class SoundManager :public IDllScriptComponent{
public:
	struct  SoundID{
		enum Enum {
			kiru = 0
		};
	};

	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void GetSound(SoundID::Enum key,XMVECTOR pos);


private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE
	SoundAsset sound;
	SERIALIZE
	PrefabAsset soundBox;
	std::map<SoundID::Enum , SoundAsset> m_SoundMap;
};
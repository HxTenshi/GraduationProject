
#pragma once
#include "main.h"


class Tutrial_1_5_Program :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ƒƒ“ƒo•Ï”
	bool m_ComboClear;
	SERIALIZE PrefabAsset m_ComboTexture;
	SERIALIZE float m_Time;
	GameObject m_ComboTextureObject;
	float m_Count;
};
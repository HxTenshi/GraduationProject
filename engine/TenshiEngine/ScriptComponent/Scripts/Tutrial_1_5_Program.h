
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

	void DestroySpecialBotton();

private:
	//ÉÅÉìÉoïœêî
	bool m_ComboClear;
	SERIALIZE PrefabAsset m_ComboTexture;
	SERIALIZE float m_Time;
	GameObject m_ComboTextureObject;
	float m_Count;
	bool m_GageMax;
	SERIALIZE std::vector<PrefabAsset> m_ComboBottonPref;
	SERIALIZE PrefabAsset m_SpercialBottonPref;
	std::vector<GameObject> m_ComboBotton;
	GameObject m_SpercialBotton;

	bool m_ComboBottonFlag;
	bool m_SpecialBottonFlag;
};

#pragma once
#include "main.h"


class ChangeTexture :public IDllScriptComponent{
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
	SERIALIZE std::vector<TextureAsset> m_Textures;
	SERIALIZE float m_ChangeTime;
	int m_NowNumber;
	float m_Count;
};
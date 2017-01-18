
#pragma once
#include "main.h"


class MapChangePoint :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void ChangeMiniMap();

private:
	//ƒƒ“ƒo•Ï”
	SERIALIZE TextureAsset m_MapTexture;
	SERIALIZE float m_MapScale;
	SERIALIZE GameObject m_LU_Point;
	SERIALIZE GameObject m_RD_Point;
};
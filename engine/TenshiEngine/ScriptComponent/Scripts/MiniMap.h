
#pragma once
#include "main.h"


class MiniMap :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void Change(TextureAsset map, float scale, GameObject lu, GameObject rd);

	static MiniMap* GetMiniMapUI();


private:
	//ƒƒ“ƒo•Ï”
	GameObject m_LU_Point;
	GameObject m_RD_Point;
};
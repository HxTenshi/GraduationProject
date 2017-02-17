
#pragma once
#include "main.h"


class CreditDraw :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	TextureAsset GetTexture();

private:
	//メンバ変数
	SERIALIZE
	TextureAsset m_texture;
};
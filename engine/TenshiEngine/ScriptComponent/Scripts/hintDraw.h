
#pragma once
#include "main.h"
#include "OutputGimic.h"

class hintDraw :public OutputGimic {
public:
	void Initialize()override;
	void Update()override;
	bool OnStart(GameObject Sender) override;

private:
	SERIALIZE float m_WaitTime;
	SERIALIZE float m_TimeScale;
	float m_Timer;
	bool m_FirstFrame;
	//ƒƒ“ƒo•Ï”
	SERIALIZE std::vector<TextureAsset> m_Textures;
	bool m_OnStart;
	TextureAsset m_NowTexture;
	float m_NowTextureNumber;
};

#pragma once
#include "main.h"
#include "OutputGimic.h"

class hintDraw :public OutputGimic {
public:
	void Initialize()override;
	void Update()override;
	void Finish()override;
	bool OnStart(GameObject Sender) override;
	
	static bool OnStart_(GameObject Sender);

	void OnFinish();

private:
	SERIALIZE float m_WaitTime;
	SERIALIZE float m_TimeScale;
	float m_Timer,m_Lerp;
	bool m_FirstFrame;
	//ƒƒ“ƒo•Ï”
	SERIALIZE std::vector<TextureAsset> m_Textures;
	bool m_OnStart;
	TextureAsset m_NowTexture;
	float m_NowTextureNumber;
};
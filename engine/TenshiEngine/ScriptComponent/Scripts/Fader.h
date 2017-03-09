
#pragma once
#include "main.h"
#include <h_component.h>
#include <h_standard.h>
#include <string>


class Fader :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void OnSceneChnage(std::string nextScenePass, bool isDestroy = false);
	bool IsFader() {
		return m_isSceneChange;
	}

private:
	//メンバ変数
	SERIALIZE
		GameObject m_faderTexObj;

	SERIALIZE PrefabAsset m_BlackTex;
	bool m_isFadeIn;	//暗くなっていくか
	bool m_isSceneChange;	//シーンを遷移するかどうか
	bool m_Destoroy;
	int m_SceneChange;
	std::string m_nextScenePass;
};

#pragma once
#include "main.h"


class ConfigScene :public IDllScriptComponent{
public:
	ConfigScene();
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void CursourMove();
	void EnterScene(int num);
	std::string GetScenePass(int num);

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE
	std::string m_volumeScenePass;
	SERIALIZE
	std::string m_howtoScenePass;
	SERIALIZE
	std::string m_comboScenePass;
	SERIALIZE
	std::string m_titleScenePass;

	SERIALIZE
	GameObject m_volumeTexObj;
	SERIALIZE
	GameObject m_howToTexObj;
	SERIALIZE
	GameObject m_comboTexObj;
	SERIALIZE
	GameObject m_toTitleTexObj;
	SERIALIZE
	GameObject m_kakoiTexObj;
	std::vector<GameObject> m_texObjs;
	int num;
};
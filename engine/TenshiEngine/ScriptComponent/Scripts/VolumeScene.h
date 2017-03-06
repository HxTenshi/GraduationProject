#pragma once
#include "main.h"
#include "VolumeBarCtrl.h"
#include "SoundManager.h"


class VolumeScene :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void Decision();
	void writeCSV(std::string fileName,int bgmVolume,int seVolume);
	std::vector<std::vector<std::string>> readCSV(std::string fileName);
	void OnSE(SoundManager::SoundSE_ID::Enum key);
	

private:
	//�����o�ϐ�
	SERIALIZE
	GameObject m_bgmBarObj;
	SERIALIZE
	GameObject m_seBarObj;
	SERIALIZE
	GameObject m_backConfigObj;
	SERIALIZE
	GameObject m_kakoi;
	std::vector<GameObject> m_objList;
	int num;
	VolumeBarCtrl* bgmCtrl;
	VolumeBarCtrl* seCtrl;

	SERIALIZE
		float STICK_INTERVAL;
	float m_stickIntervalTime;
	float m_stickTilTimer;		//�X�e�B�b�N���X��������
	float m_continuTimer;		

	SERIALIZE
		GameObject m_fader;
};
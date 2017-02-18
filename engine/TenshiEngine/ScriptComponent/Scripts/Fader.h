
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
	void OnSceneChnage(std::string nextScenePass);

private:
	//�����o�ϐ�
	SERIALIZE
		GameObject m_blackTex;
	bool m_isFadeIn;	//�Â��Ȃ��Ă�����
	bool m_isSceneChange;	//�V�[����J�ڂ��邩�ǂ���
	std::string m_nextScenePass;
};
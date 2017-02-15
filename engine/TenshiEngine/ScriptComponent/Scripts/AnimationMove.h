
#pragma once
#include "main.h"


class AnimationMove :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

private:
	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject m_MoveObject;
	SERIALIZE GameObject m_BoneObject;
	SERIALIZE bool m_Center;
	SERIALIZE bool m_ZeroSetting;
	XMVECTOR m_BackPos;
};
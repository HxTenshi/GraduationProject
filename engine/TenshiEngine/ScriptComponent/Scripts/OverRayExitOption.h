
#pragma once
#include "main.h"
enum OverRaySelector {
	OR_YES=0,
	OR_NO,
	OR_END
};

class OverRayExitOption :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;
	void SetAlpha(GameObject g,float a);
	void InitCall();

	void Select();
	void Next();
	void Back();
	void SetOverrayFlag(bool f);
private:
	//ÉÅÉìÉoïœêî

	SERIALIZE bool is_overray;
	SERIALIZE GameObject m_overray_object;
	SERIALIZE GameObject m_select_object;
	SERIALIZE GameObject m_yes_object;
	SERIALIZE GameObject m_no_object;
	OverRaySelector selector;

	XMVECTOR selectpos[OR_END];
	bool is_arrow;
	float m_select_timer;
	bool is_select;
	bool is_use_pad;

};
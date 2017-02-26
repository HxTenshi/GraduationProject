
#pragma once
#include "main.h"


class GetMapShift :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	GameObject GetMinMapShift();

private:
	GameObject _GetMinMapShift();

	//ƒƒ“ƒo•Ï”
	std::list<GameObject> m_MapShiftList;
	GameObject m_MiniMapShift;
	SERIALIZE
	GameObject m_MapShiftUI;
};
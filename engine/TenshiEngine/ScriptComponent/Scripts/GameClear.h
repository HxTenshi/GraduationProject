
#pragma once
#include "main.h"
#include <string>
#include "h_component.h"



class GameClear :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	void SetTimeAndCount(int time, int count);
	std::string GetClockTime();
private:
	int m_time;
	int m_count;

	SERIALIZE
	GameObject m_time_text;
	SERIALIZE
	GameObject m_count_text;
	//ƒƒ“ƒo•Ï”
};
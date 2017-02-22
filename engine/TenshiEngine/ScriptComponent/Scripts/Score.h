
#pragma once
#include "main.h"


class Score :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	static void AddScore();
	static void Write();
	static int m_Score;
	static float m_Time;

	
private:

};
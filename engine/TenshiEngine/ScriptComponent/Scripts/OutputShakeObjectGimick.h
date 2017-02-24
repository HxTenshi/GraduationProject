
#pragma once
#include "main.h"
#include "OutputGimic.h"


class OutputShakeObjectGimick :public OutputGimic {

	void Initialize()override;
	void Update()override;

	bool OnStart(GameObject Sender) override;
private:
	//ÉÅÉìÉoïœêî
	SERIALIZE float m_Time;
	SERIALIZE float m_ShakePower;
	SERIALIZE GameObject m_ShakeObject;
	SERIALIZE bool m_Camera;
	SERIALIZE bool m_NoScaleDeltaTimeMode;
	XMVECTOR m_Vect;
	float m_Timer;
};
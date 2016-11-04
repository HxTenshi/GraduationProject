
#pragma once
#include "main.h"


class Matinee :public IDllScriptComponent{
public:
	Matinee();
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	bool IsPlay();
	void Play();
	void Stop();

	void SetCurrentCameraPointStart(bool flag);
	bool GetCurrentCameraPointStart();

	void SetEndCollback(const std::function<void(void)>& func);
	void SetSpeed(float speed);
	float GetSpeed();
	void SetDeltaTimeScale(float scale);
	float GetDeltaTimeScale();

private:

	void next();

	//ÉÅÉìÉoïœêî
	SERIALIZE GameObject m_Point_1;
	SERIALIZE GameObject m_Point_2;
	SERIALIZE GameObject m_Point_3;
	SERIALIZE GameObject m_Point_4;
	SERIALIZE GameObject m_Point_5;
	SERIALIZE GameObject m_Point_6;

	SERIALIZE float m_Point_0_EndWaitTime;
	SERIALIZE float m_Point_1_EndWaitTime;
	SERIALIZE float m_Point_2_EndWaitTime;
	SERIALIZE float m_Point_3_EndWaitTime;
	SERIALIZE float m_Point_4_EndWaitTime;
	SERIALIZE float m_Point_5_EndWaitTime;
	SERIALIZE float m_Point_6_EndWaitTime;

	SERIALIZE GameObject m_Point_0_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_1_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_2_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_3_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_4_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_5_EndTriggerGimmick;
	SERIALIZE GameObject m_Point_6_EndTriggerGimmick;


	SERIALIZE
		float m_Speed;
	SERIALIZE
		float m_DeltaTimeScale;
	float m_DeltaTimeScaleBack;
	SERIALIZE
		GameObject m_Camera;
	SERIALIZE
		bool m_CurrentCameraPointStart;
	bool m_IsPlay;
	std::function<void(void)> m_EndCollback;
	float m_Timer;
	float m_TimerMax;
	bool m_WaitMode;
	int m_CurrentPos;
	XMVECTOR m_position[2];
	XMVECTOR m_quaternion[2];
};
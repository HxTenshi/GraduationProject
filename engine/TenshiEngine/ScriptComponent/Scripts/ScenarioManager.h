
#pragma once
#include "main.h"

struct Scenario {
	enum Enum
	{
		GameStart,
		S1_BlockRock,
		S1_ButtleLizard,
		S1_ButtleLizard_End,
		S1_BreakRock,
		S1_Tutorial_1b,
		S1_Tutorial_1b_End,

		S2_WaveStart,
		S2_ButtleMinotaur,
		S2_ButtleMinotaur_End,

		S3_ButtleReis,
		S3_ButtleReis_End,
		Count
	};
};

class ScenarioManager :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;

	//フラグを立てる
	static bool SetFlag(Scenario::Enum id);
	//フラグが立っているか
	static bool GetFlag(Scenario::Enum id);

private:
	//メンバ変数

	struct ScenarioData {
		std::function<bool(void)> Prerequisite = []() {return true; };//前提条件
		std::function<void(void)> Action = []() {};//フラグ時の処理
		bool Flag = false;//フラグ
	};
	std::vector<ScenarioData> m_Scenario;
};
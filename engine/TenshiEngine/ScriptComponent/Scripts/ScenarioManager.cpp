#include "ScenarioManager.h"
#include "SoundManager.h"

static ScenarioManager* g_sm=NULL;

//生成時に呼ばれます（エディター中も呼ばれます）
void ScenarioManager::Initialize(){


	m_Scenario.resize(Scenario::Count);

	m_Scenario[Scenario::GameStart].Action = [](){
		SoundManager::PlayBGM(SoundManager::SoundBGM_ID::GamePlay);
	};
	m_Scenario[Scenario::S2_ButtleMinotaur].Action = []() {
		//SoundManager::PlayBGM(SoundManager::SoundBGM_ID::GamePlay);
	};
	m_Scenario[Scenario::S2_ButtleMinotaur_End].Action = []() {
		//SoundManager::PlayBGM(SoundManager::SoundBGM_ID::GamePlay);
	};
	m_Scenario[Scenario::S3_ButtleReis].Action = []() {
		//SoundManager::PlayBGM(SoundManager::SoundBGM_ID::GamePlay);
	};
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ScenarioManager::Start(){
	if (g_sm) {
		return;
	}
	g_sm = this;
	SetFlag(Scenario::GameStart);
}

//毎フレーム呼ばれます
void ScenarioManager::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ScenarioManager::Finish(){
	g_sm = NULL;
}

bool ScenarioManager::SetFlag(Scenario::Enum id)
{
	if (!g_sm)return false;
	if (id < 0 || id >= Scenario::Count)return false;
	if (g_sm->m_Scenario[id].Flag)return false;
	if (!g_sm->m_Scenario[id].Prerequisite())return false;
	g_sm->m_Scenario[id].Flag = true;
	g_sm->m_Scenario[id].Action();
	return true;
}

bool ScenarioManager::GetFlag(Scenario::Enum id)
{
	if (!g_sm)return false;
	if (id < 0 || id >= Scenario::Count)return false;
	return g_sm->m_Scenario[id].Flag;
}

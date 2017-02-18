#include "GameClear.h"
#include <sstream>

//生成時に呼ばれます（エディター中も呼ばれます）
void GameClear::Initialize(){
	m_time =80;
	m_count = 0;
	SetTimeAndCount(m_time, m_count);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GameClear::Start(){
}

//毎フレーム呼ばれます
void GameClear::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GameClear::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GameClear::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void GameClear::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void GameClear::OnCollideExit(GameObject target){
	(void)target;
}

void GameClear::SetTimeAndCount(int time, int count)
{
	m_time = time;
	m_count = count;
	std::string t = GetClockTime();
	std::ostringstream s;
	s << m_count;
	std::string c = s.str();
	m_time_text->GetComponent<TextComponent>()->ChangeText(t);
	m_count_text->GetComponent<TextComponent>()->ChangeText(c);
}
//めんどいので簡易
std::string GameClear::GetClockTime()
{
	std::string result="";
	int time = m_time;

	int imin;
	int isec;
	std::ostringstream min;
	min << time / 60;
	imin = time / 60;


	time = time % 60;
	std::ostringstream sec;
	sec << time;
	isec = time % 60;

	if (imin < 10) {
		result += "0" + min.str() + ":";
	}
	else {
		result = min.str(); ":";
	}
	if (isec < 10) {
		result += "0" + sec.str();
	}
	else {
		result += sec.str();
	}



	return result;
}

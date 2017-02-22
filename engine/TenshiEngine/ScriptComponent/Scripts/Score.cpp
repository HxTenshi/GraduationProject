#include "Score.h"

#include "../h_standard.h"
#include "../h_component.h"

#include <istream>
#include <fstream>
#include <sstream>
using namespace std;

int Score::m_Score = 0;
float Score::m_Time = 0.0f;
//生成時に呼ばれます（エディター中も呼ばれます）
void Score::Initialize(){
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Score::Start(){

}

//毎フレーム呼ばれます
void Score::Update(){
	m_Time += Hx::DeltaTime()->GetDeltaTime();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Score::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Score::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Score::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Score::OnCollideExit(GameObject target){
	(void)target;
}

void Score::AddScore()
{
	Score::m_Score++;
	Hx::Debug()->Log("m_Score : " + std::to_string(m_Score));
}

void Score::Write()
{
	//ファイルを開く
	ofstream ofs("Assets/data/Score.csv");
	//BGM,SEの音量をstring型に変換
	std::string bgm = std::to_string(m_Score);
	std::string se = std::to_string((int)m_Time);
	//書き込み
	ofs << bgm << "," << se << endl;
}

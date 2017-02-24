#include "Score.h"

#include "../h_standard.h"
#include "../h_component.h"

#include <istream>
#include <fstream>
#include <sstream>
using namespace std;

int Score::m_Score = 0;
float Score::m_Time = 0.0f;
int Score::m_CountEnemy = 0;
//生成時に呼ばれます（エディター中も呼ばれます）
void Score::Initialize(){
	Score::m_Score = 0;
	Score::m_Time = 0.0f;
	Score::m_CountEnemy = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Score::Start(){

}

//毎フレーム呼ばれます
void Score::Update(){
	m_Time += Hx::DeltaTime()->GetNoScaleDeltaTime();
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

void Score::AddCountEnemy()
{
	Score::m_CountEnemy++;
}

void Score::Write()
{
	//ファイルを開く
	ofstream ofs("Assets/data/Score.csv");
	//BGM,SEの音量をstring型に変換
	std::string score = std::to_string(m_Score);
	std::string countEnemy = std::to_string(m_CountEnemy);
	std::string time = std::to_string((int)m_Time);
	//書き込み
	ofs << score << "," << countEnemy << "," << time << endl;
}
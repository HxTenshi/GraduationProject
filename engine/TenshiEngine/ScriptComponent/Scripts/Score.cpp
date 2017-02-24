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
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Score::Initialize(){
	Score::m_Score = 0;
	Score::m_Time = 0.0f;
	Score::m_CountEnemy = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Score::Start(){

}

//���t���[���Ă΂�܂�
void Score::Update(){
	m_Time += Hx::DeltaTime()->GetNoScaleDeltaTime();
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void Score::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Score::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void Score::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
	//�t�@�C�����J��
	ofstream ofs("Assets/data/Score.csv");
	//BGM,SE�̉��ʂ�string�^�ɕϊ�
	std::string score = std::to_string(m_Score);
	std::string countEnemy = std::to_string(m_CountEnemy);
	std::string time = std::to_string((int)m_Time);
	//��������
	ofs << score << "," << countEnemy << "," << time << endl;
}
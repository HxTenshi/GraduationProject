#include "Score.h"

#include "../h_standard.h"
#include "../h_component.h"

#include <istream>
#include <fstream>
#include <sstream>
using namespace std;

int Score::m_Score = 0;
float Score::m_Time = 0.0f;
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Score::Initialize(){
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void Score::Start(){

}

//���t���[���Ă΂�܂�
void Score::Update(){
	m_Time += Hx::DeltaTime()->GetDeltaTime();
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

void Score::Write()
{
	//�t�@�C�����J��
	ofstream ofs("Assets/data/Score.csv");
	//BGM,SE�̉��ʂ�string�^�ɕϊ�
	std::string bgm = std::to_string(m_Score);
	std::string se = std::to_string((int)m_Time);
	//��������
	ofs << bgm << "," << se << endl;
}

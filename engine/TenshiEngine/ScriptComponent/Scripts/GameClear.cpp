#include "GameClear.h"
#include <sstream>

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameClear::Initialize(){
	m_time =80;
	m_count = 0;
	SetTimeAndCount(m_time, m_count);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GameClear::Start(){
}

//���t���[���Ă΂�܂�
void GameClear::Update(){

}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GameClear::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameClear::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GameClear::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
//�߂�ǂ��̂ŊȈ�
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

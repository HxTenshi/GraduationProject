#include "OutputConectorRandomGimic.h"
#include "h_standard.h"

#include <random>

bool OutputConectorRandomGimic::OnStart(GameObject Sender)
{
	static std::random_device rnd;     // �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
	static std::mt19937 mt(rnd()); //  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
							//std::uniform_int_distribution<> rand100(0, 99);     // [0, 99] �͈͂̈�l����
	//std::normal_distribution<> norm(50.0, 10.0);       // ����50, ���U�l10�̐��K���z
	//for (int i = 0; i < 20; ++i) {
	//	//std::cout << rand100(mt) << "\n";
	//	std::cout << norm(mt) << "\n";
	//}
	float total =
		m_Output_1_Rate +
		m_Output_2_Rate +
		m_Output_3_Rate +
		m_Output_4_Rate +
		m_Output_5_Rate +
		m_Output_6_Rate;

	if (total == 0.0f)return false;
	float r = mt()/100000.0f;

	float res = fmod(r, total);
	res = abs(res);

	res -= m_Output_1_Rate; if (res<=0){ if (auto scr = OutputGimic::GetOutputGimic(m_Output_1))scr->OnStart(Sender);return true;}
	res -= m_Output_2_Rate; if (res<=0){ if (auto scr = OutputGimic::GetOutputGimic(m_Output_2))scr->OnStart(Sender);return true;}
	res -= m_Output_3_Rate; if (res<=0){ if (auto scr = OutputGimic::GetOutputGimic(m_Output_3))scr->OnStart(Sender);return true;}
	res -= m_Output_4_Rate; if (res<=0){ if (auto scr = OutputGimic::GetOutputGimic(m_Output_4))scr->OnStart(Sender);return true;}
	res -= m_Output_5_Rate; if (res<=0){ if (auto scr = OutputGimic::GetOutputGimic(m_Output_5))scr->OnStart(Sender);return true;}
	res -= m_Output_6_Rate; if (res<=0){ if (auto scr = OutputGimic::GetOutputGimic(m_Output_6))scr->OnStart(Sender);return true;}

	return false;
}

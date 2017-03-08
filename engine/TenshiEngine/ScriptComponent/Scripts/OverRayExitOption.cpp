#include "OverRayExitOption.h"
#include "h_standard.h"
#include "h_component.h"
#include "SoundManager.h"
//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OverRayExitOption::Initialize(){
	selector = OR_NO;
	m_func_type = 0;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void OverRayExitOption::Start(){
	selectpos[OR_YES] = m_yes_object->mTransform->Position();
	selectpos[OR_YES].z--;
	selectpos[OR_NO] = m_no_object->mTransform->Position();
	selectpos[OR_NO].z--;
	m_select_timer = 0.0f;
	is_select = true;
	SetAlpha(m_select_object, 0.9f);


}

//���t���[���Ă΂�܂�
void OverRayExitOption::Update(){
	if (!is_overray)return;
	if (!is_select) {
		m_select_timer += Hx::DeltaTime()->GetNoScaleDeltaTime();
	}
	if (m_select_timer > 0.3f)is_select = true;
	is_arrow = false;
	Select(m_func_type);
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void OverRayExitOption::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OverRayExitOption::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void OverRayExitOption::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void OverRayExitOption::OnCollideExit(GameObject target){
	(void)target;
}

void OverRayExitOption::SetAlpha(GameObject g,float a)
{
	auto c = g->GetComponent<MaterialComponent>()->mAlbedo;
	c.w = a;
	g->GetComponent<MaterialComponent>()->SetAlbedoColor(c);


}

void OverRayExitOption::InitCall()
{
	if (m_overray_object) {
		SetAlpha(m_overray_object, 0.99f);
	}
}

void OverRayExitOption::Select(int type)
{
	if (m_select_object) {

		auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
		bool isRightLS = ls.x > 0.5f;	//�E�ɓ|�������H
		bool isLeftLS = ls.x < -0.5f;	//���ɓ|�������H
		bool isEnter = Input::Trigger(KeyCode::Key_SPACE) || Input::Up(PAD_X_KeyCode::Button_B);	//����
		bool isCansel = Input::Trigger(KeyCode::Key_B) || Input::Up(PAD_X_KeyCode::Button_A);		//�L�����Z��

		if ((isLeftLS || isRightLS|| isEnter)&!is_use_pad) {
			is_use_pad = true;
			return;
		}

		if (!is_use_pad)return;



		if (is_select) {
			if ((Input::Trigger(KeyCode::Key_LEFT) || isLeftLS)) {
				Back();
				is_select = false;
				m_select_timer = 0.0f;
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Enum::Cursour, XMVectorSet(-4.6f, 17.0f, -3.3f, 0.0f));
			}
			if ((Input::Trigger(KeyCode::Key_RIGHT) || isRightLS)) {
				Next();
				is_select = false;
				m_select_timer = 0.0f;
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Enum::Cursour, XMVectorSet(-4.6f, 17.0f, -3.3f, 0.0f));
			}
		}

		m_select_object->mTransform->Position(selectpos[selector]);

		if (isEnter) {
			if (selector == OR_YES) {
				if(type==0)Hx::Shutdown();
				if (type == 1) {
					Hx::DeltaTime()->SetTimeScale(1.0f);
					Hx::LoadScene("Assets/Title.scene");
				}
			}
			if (selector == OR_NO) {
				is_overray = false;
				gameObject->Disable();
			}
		}

	}

}

void OverRayExitOption::Next()
{
	int s = (int)selector + 1;
	if (s >= OR_END) {
		s = OR_YES;
	}
	selector = (OverRaySelector)s;
}

void OverRayExitOption::Back()
{
	int s = (int)selector - 1;
	if (s < (int)OR_YES) {
		s = OR_NO;
	}
	selector = (OverRaySelector)s;
}



void OverRayExitOption::SetOverrayFlag(bool f)
{
	is_overray = true;
	is_use_pad = false;
	is_select = true;
}

void OverRayExitOption::SetFuncType(int num)
{
	m_func_type = num;
}

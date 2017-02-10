#include "ParticleController.h"
#include "h_standard.h"
#include "h_component.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void ParticleController::Initialize(){
	count_num_delete_time = 0.0f;
	num_delete_count = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ParticleController::Start(){
	if (m_start_num_delete_flag) {
		ParticleNumSwitch(false);
	}
	if (m_Particle01)m_Particle.push_back(m_Particle01);
	if (m_Particle02)m_Particle.push_back(m_Particle02);
	if (m_Particle03)m_Particle.push_back(m_Particle03);
	if (m_Particle04)m_Particle.push_back(m_Particle04);
	if (m_Particle05)m_Particle.push_back(m_Particle05);
}

//毎フレーム呼ばれます
void ParticleController::Update(){
	NumDeleteTimer();
	NumFunc();
	AutoDeleteTimer();
	AutoDeleteFunc();
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ParticleController::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ParticleController::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ParticleController::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ParticleController::OnCollideExit(GameObject target){
	(void)target;
}

void ParticleController::ParticleNumSwitch(bool flag)
{
	if(flag){
		for (auto i : m_Particle) {
			auto p = i->GetComponent<ParticleComponent>();
			p->mParticleParam.Param.x = m_particle_count;
		}
	}
	else {
		for (auto i : m_Particle) {
			auto p = i->GetComponent<ParticleComponent>();
			p->mParticleParam.Param.x = 0;
		}
	}
}
void ParticleController::NumFunc()
{
	if (!num_delete_flag)return;
	if (num_delete_time<= count_num_delete_time) {
		for (auto i : m_Particle) {
			auto p = i->GetComponent<ParticleComponent>();
			p->mParticleParam.Param.x = m_particle_count;
		}
		count_num_delete_time = 0.0f;
		if (num_once_flag)num_delete_flag = false;
	}
}

void ParticleController::NumDeleteTimer()
{
	if (!num_delete_flag)return;
	count_num_delete_time += Hx::DeltaTime()->GetDeltaTime();
	
}

void ParticleController::AutoDeleteFunc()
{
	if (!auto_delete_flag)return;
	if (auto_delete_time <= count_auto_delete_time)Hx::DestroyObject(gameObject);
}

void ParticleController::AutoDeleteTimer()
{
	if (!auto_delete_flag)return;
	count_auto_delete_time += Hx::DeltaTime()->GetDeltaTime();
}

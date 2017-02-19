#include "TargetEnemy.h"

#include "h_standard.h"
#include "TPSCamera.h"
#include "Enemy.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void TargetEnemy::Initialize(){
	m_TargetEnemy = NULL;

	if (!m_EnemyTargetMarkar) {
		m_EnemyTargetMarkar->Disable();
	}
	if (!m_EnemyHP_Bar) {
		m_EnemyHP_Bar->Disable();
	}if (!m_EnemyHP_BarFrame) {
		m_EnemyHP_BarFrame->Disable();
	}
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TargetEnemy::Start(){

}

//毎フレーム呼ばれます
void TargetEnemy::Update(){
	if (!m_Camera)return;
	auto camera = m_Camera->GetScript<TPSCamera>();
	if (!camera)return;
	auto target = camera->GetLookTarget();
	
	if (target && Enemy::GetEnemy(target)) {
		SetTargetEnemy(target);
		if (Enemy::GetEnemy(target)->GetEnemyAllParameter(false).hp <= 0) {
			camera->SetLookTarget(NULL);
			SetTargetEnemy(NULL);
		}
	}
	else {
		SetTargetEnemy(NULL);
	}

	
	MarkarUpdate();
	BarUpdate();

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TargetEnemy::Finish(){

}

//コライダーとのヒット時に呼ばれます
void TargetEnemy::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TargetEnemy::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TargetEnemy::OnCollideExit(GameObject target){
	(void)target;
}

void TargetEnemy::SetTargetEnemy(GameObject enemy)
{
	m_TargetEnemy = enemy;
}

GameObject TargetEnemy::GetTargetEnemy()
{
	return m_TargetEnemy;
}

void TargetEnemy::MarkarUpdate()
{
	if (!m_EnemyTargetMarkar)return;
	if (!m_Camera)return;

	if (m_TargetEnemy) {
		m_EnemyTargetMarkar->Enable();

		m_EnemyTargetMarkar->mTransform->WorldPosition(m_TargetEnemy->mTransform->WorldPosition());
		m_EnemyTargetMarkar->mTransform->WorldQuaternion(m_Camera->mTransform->WorldQuaternion());
	}
	else {
		m_EnemyTargetMarkar->Disable();
	}

}

#include "Enemy.h"

void TargetEnemy::BarUpdate()
{
	//if (!m_Camera)return;
	//auto camera = m_Camera->GetScript<TPSCamera>();
	//if (!camera)return;

	//auto proj = XMMatrixPerspectiveLH(1920.0f, 1080.0f, 1000.0f, 1.0f);
	//XMVECTOR nullv;
	//auto view = XMMatrixInverse(&nullv, XMMatrixTranspose(m_Camera->mTransform->GetMatrix()));
	//auto mat = XMMatrixInverse(&nullv, XMMatrixMultiply(view, proj));
	//XMVECTOR pos = XMVectorSet()
	//auto view = XMMatrixLookAtLH(m_Camera->mTransform->WorldPosition(), m_TargetEnemy->mTransform->WorldPosition(), XMVectorSet(0, 1, 0, 1));


	if (!m_EnemyHP_Bar)return;
	if (!m_EnemyHP_BarFrame)return;

	Enemy* enemy = NULL;
	if (m_TargetEnemy) {
		
		enemy = Enemy::GetEnemy(m_TargetEnemy);
	}
	if (enemy) {
		m_EnemyHP_Bar->Enable();
		m_EnemyHP_BarFrame->Enable();

		auto param = enemy->GetEnemyAllParameter(false);
		float i = (float)param.maxHp;
		if (param.maxHp == 0.0f) {
			param.maxHp = 1.0f;
		}
		float hp = param.hp / param.maxHp;

		auto s = m_EnemyHP_Bar->mTransform->Scale();
		s.x = hp * 512.0f;
		m_EnemyHP_Bar->mTransform->Scale(s);
	}
	else {
		m_EnemyHP_Bar->Disable();
		m_EnemyHP_BarFrame->Disable();
	}


}

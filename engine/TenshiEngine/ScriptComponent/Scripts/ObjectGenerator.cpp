#include "ObjectGenerator.h"

#include"h_standard.h"
#include "OutputGimic.h"

#include <random>


//生成時に呼ばれます（エディター中も呼ばれます）
void ObjectGenerator::Initialize(){
	m_GeneratObject = NULL;
	m_Timer = m_Time;
	m_Count = 0;
	m_GeneratFlag = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ObjectGenerator::Start(){

}

//毎フレーム呼ばれます
void ObjectGenerator::Update(){
	if (!m_Object.IsLoad())return;
	if (m_UniqueGeneratMode) {
		if (m_GeneratObject)return;
		//Hx::Debug()->Log("Cheack m_GeneratFlag : " + std::to_string(m_GeneratFlag));
		if (m_GeneratFlag) {	
			if (auto gimick = OutputGimic::GetOutputGimic(m_DestroyOutputGimick)) {
				gimick->OnStart(gameObject);
			}
			m_GeneratFlag = false;
		}
	}
	if (m_Num != 0 && m_Count >= m_Num) {
		return;
	}

	m_Timer -= Hx::DeltaTime()->GetDeltaTime();
	m_Timer = max(m_Timer, 0.0f);
	if (m_Timer != 0.0f)return;
	m_Timer = m_Time;
	m_Count++;

	if (m_GeneratPercent!=0.0f) {
		static std::random_device rnd;     // 非決定的な乱数生成器でシード生成機を生成
		static std::mt19937 mt(rnd()); //  メルセンヌツイスターの32ビット版、引数は初期シード

		float r = mt() / 100000.0f;

		float res = fmod(r, 1.0f);
		res = abs(res);
		if (m_GeneratPercent <= res) {
			return;
		}
	}

	GameObject obj = Hx::Instance(m_Object);
	if (!obj)return;
	if(!m_PositionCopyNot)
		obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
	if(m_RotateCopy)
		obj->mTransform->WorldQuaternion(gameObject->mTransform->WorldQuaternion());

	if (m_UniqueGeneratMode) {
		m_GeneratObject = obj;
		m_GeneratFlag = true;
	}
	if (auto gimick = OutputGimic::GetOutputGimic(m_OutputGimick)) {
		gimick->OnStart(gameObject);
	}

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ObjectGenerator::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ObjectGenerator::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ObjectGenerator::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ObjectGenerator::OnCollideExit(GameObject target){
	(void)target;
}
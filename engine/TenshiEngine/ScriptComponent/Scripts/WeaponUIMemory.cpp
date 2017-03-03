#include "WeaponUIMemory.h"
#include "h_standard.h"
#include "h_component.h"
//生成時に呼ばれます（エディター中も呼ばれます）
void WeaponUIMemory::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void WeaponUIMemory::Start(){
	auto mem = gameObject->mTransform->Children();
	for (auto i : mem) {
		int num = std::stoi(i->Name());
		if (i->GetComponent<MaterialComponent>()) {
			i->GetComponent<MaterialComponent>()->SetAlbedoColor(GetMemColor(num));
			m_GageMem[num - 1] = i;
		}
	}
	FillAlpha();
}

//毎フレーム呼ばれます
void WeaponUIMemory::Update(){

}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void WeaponUIMemory::Finish(){

}

//コライダーとのヒット時に呼ばれます
void WeaponUIMemory::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void WeaponUIMemory::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void WeaponUIMemory::OnCollideExit(GameObject target){
	(void)target;
}

XMFLOAT4 WeaponUIMemory::GetMemColor(int n)
{
	if (n > 5) {
		return XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (n > 2) {
		return XMFLOAT4(1.0f, 0.6f, 0.0f, 1.0f);
	}
	else if (n >= 0) {
		return XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	}

	return XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void WeaponUIMemory::FillAlpha()
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_GageMem[i]) {
			m_GageMem[i]->GetComponent<MaterialComponent>()->SetAlbedoColor(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
		}
	}
}

void WeaponUIMemory::WeaponAttackParamMemSet(int atk)
{
	FillAlpha();
	int num = (atk > 10) ? 10 : atk;
	for (int i = 0; i < num; ++i) {
		if (m_GageMem[i]) {
			m_GageMem[i]->GetComponent<MaterialComponent>()->SetAlbedoColor(GetMemColor(i));
		}
	}
}

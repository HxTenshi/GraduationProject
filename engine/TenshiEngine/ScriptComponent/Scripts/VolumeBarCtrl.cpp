#include "VolumeBarCtrl.h"
#include "h_standard.h"


//生成時に呼ばれます（エディター中も呼ばれます）
void VolumeBarCtrl::Initialize(){
	m_widthScale = m_memoriBar->mTransform->Scale().x / VOLUME_RATE;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void VolumeBarCtrl::Start(){

}

//毎フレーム呼ばれます
void VolumeBarCtrl::Update(){
	//クランプ処理
	int minNum = 0;
	int maxNum = 100;
	m_volume = (int)min(max(minNum, m_volume), maxNum);

	XMVECTOR scale;
	scale.x = m_volume * m_widthScale;
	scale.y = m_memoriBar->mTransform->Scale().y;
	scale.z = 1.0f;

	m_memoriBar->mTransform->Scale(scale);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void VolumeBarCtrl::Finish(){

}

//コライダーとのヒット時に呼ばれます
void VolumeBarCtrl::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void VolumeBarCtrl::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void VolumeBarCtrl::OnCollideExit(GameObject target){
	(void)target;
}

void VolumeBarCtrl::VolumeUp(){
	m_volume++;
	m_volume = min(100.0f, m_volume);
}

void VolumeBarCtrl::VolumeDown(){
	m_volume--;
	m_volume = max(0.0f, m_volume);
}

//音量をセット
void VolumeBarCtrl::SetVolume(int volume){
	m_volume = volume;
}

//音量を取得
int VolumeBarCtrl::GetVolume(){
	return m_volume;
}

bool VolumeBarCtrl::IsVolumeMax()
{
	return m_volume >= VOLUME_RATE;
}

bool VolumeBarCtrl::IsVolumeMin()
{
	return m_volume <= 0.0f;
}

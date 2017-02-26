#include "GetMapShift.h"
#include "h_standard.h"
#include "h_component.h"
#include "UniqueObject.h"
#include "MapShift.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetMapShift::Initialize(){
	m_MiniMapShift = NULL;
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void GetMapShift::Start(){

}

//���t���[���Ă΂�܂�
void GetMapShift::Update(){
	m_MiniMapShift = _GetMinMapShift();


	auto m_Camera = UniqueObject::GetCamera();
	if (!m_Camera)return;
	if (m_MiniMapShift) {
		if (auto cam = m_Camera->GetComponent<CameraComponent>()) {
			auto w = m_MiniMapShift->mTransform->GetMatrix();
			auto v = cam->GetViewMatrix();
			auto p = cam->GetProjectionMatrix();
			auto wvp = XMMatrixMultiply(XMMatrixMultiply(w, v), p);

			auto x = (wvp.r[3].x/wvp.r[3].w * 0.5f + 0.5f) * 1920;
			auto y = (wvp.r[3].y/wvp.r[3].w * 0.5f + 0.5f) * 1080;
			if (m_MapShiftUI) {
				m_MapShiftUI->Enable();
				auto z = m_MapShiftUI->mTransform->Position().z;
				m_MapShiftUI->mTransform->Position(XMVectorSet(x, y, z, 1.0f));
			}

		}
	}
	else {
		if (m_MapShiftUI) {
			m_MapShiftUI->Disable();
		}
	}
	
}

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void GetMapShift::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetMapShift::OnCollideBegin(GameObject target){

	if (!target)return;
	if (!MapShift::GetMapShift(target))return;

	m_MapShiftList.push_back(target);
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void GetMapShift::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void GetMapShift::OnCollideExit(GameObject target){
	if (!target)return;
	if (!MapShift::GetMapShift(target))return;
	m_MapShiftList.remove_if([&](auto o) {
		return o == target || !o; });
}

GameObject GetMapShift::GetMinMapShift()
{
	return m_MiniMapShift;
}

GameObject GetMapShift::_GetMinMapShift()
{
	auto m_Camera = UniqueObject::GetCamera();
	if (!m_Camera)return NULL;
	XMVECTOR camvec;
	camvec = m_Camera->mTransform->Forward();

	float lowL = 9999999.0f;
	GameObject lowObj = NULL;

	for (auto& mapshift : m_MapShiftList) {
		if (!mapshift) continue;

		auto pos = gameObject->mTransform->WorldPosition();
		auto mpos = mapshift->mTransform->WorldPosition();
		auto v = mpos - pos;
		float l = XMVector3Length(v).x;
		if (l <= 5.0f) {
			continue;
		}

		XMVECTOR vect2 = XMVector3Normalize(mapshift->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition());
		float dot = XMVector3Dot(camvec, vect2).x;
		if (dot < 0) {
			continue;
		}

		RaycastHit hit;
		if (Hx::PhysX()->RaycastHit(pos, XMVector3Normalize(v), l, &hit, Layer::UserTag4)) {
			if (XMVector3Length(hit.position - mpos).x > 1.0f) {
				continue;
			}
		}

		if (l < lowL) {
			lowObj = mapshift;
			lowL = l;
		}
	}
	return lowObj;
}

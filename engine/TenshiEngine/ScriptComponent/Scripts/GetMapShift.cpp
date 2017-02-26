#include "GetMapShift.h"
#include "h_standard.h"
#include "h_component.h"
#include "UniqueObject.h"
#include "MapShift.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void GetMapShift::Initialize(){
	m_MiniMapShift = NULL;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void GetMapShift::Start(){

}

//毎フレーム呼ばれます
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

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void GetMapShift::Finish(){

}

//コライダーとのヒット時に呼ばれます
void GetMapShift::OnCollideBegin(GameObject target){

	if (!target)return;
	if (!MapShift::GetMapShift(target))return;

	m_MapShiftList.push_back(target);
}

//コライダーとのヒット中に呼ばれます
void GetMapShift::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
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

#include "MiniMap.h"
#include "h_component.h"
#include "h_standard.h"
#include "UniqueObject.h"

static MiniMap* g_Minimap = NULL;

//生成時に呼ばれます（エディター中も呼ばれます）
void MiniMap::Initialize(){
	g_Minimap = this;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void MiniMap::Start(){
	g_Minimap = this;
}

//毎フレーム呼ばれます
void MiniMap::Update(){

	auto material = gameObject->GetComponent<MaterialComponent>();
	if (!material)return;
	auto mate = material->GetMaterialPtr(0);
	if (!mate)return;


	auto m_Player = UniqueObject::GetPlayer();
	if (!m_Player || !m_LU_Point || !m_RD_Point)return;
	
	auto p = m_Player->mTransform->WorldPosition() + XMVectorSet(10000.0f, 10000.0f, 10000.0f, 1.0f);
	auto l = m_LU_Point->mTransform->WorldPosition() + XMVectorSet(10000.0f, 10000.0f, 10000.0f, 1.0f);
	auto r = m_RD_Point->mTransform->WorldPosition() + XMVectorSet(10000.0f, 10000.0f, 10000.0f, 1.0f);

	auto v = r - l;
	if ((v.x > 0 && v.z > 0) || (v.x < 0 && v.z < 0)) {
		r = XMVectorSet(r.z, 0, r.x, 0);
		l = XMVectorSet(l.z, 0, l.x, 0);
		p = XMVectorSet(p.z, 0, p.x, 0);
		//Hx::Debug()->Log("a");
	}
	
	p -= l;
	r -= l;

	XMFLOAT2 off(0.0f,0.0f);
	auto scale = mate->GetTextureScale();
	
	off.x = p.x / r.x - scale.x / 2.0f;
	off.y = p.z / r.z - scale.y / 2.0f;
	mate->SetOffset(off);
	
	//mate->mDiffuse.x = 0;
	//mate->mDiffuse.y = 0;
	//mate->mDiffuse.z = 0;


	//material->SetMaterial(0, *mate);
	//material->SetAlbedoColor(XMFLOAT4(1,0,1,1));
	//material->SetMaterial(0, mate);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void MiniMap::Finish(){
	g_Minimap = NULL;
}

//コライダーとのヒット時に呼ばれます
void MiniMap::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void MiniMap::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void MiniMap::OnCollideExit(GameObject target){
	(void)target;
}

void MiniMap::Change(TextureAsset map, float scale, GameObject lu, GameObject rd)
{

	auto material = gameObject->GetComponent<MaterialComponent>();
	if (!material)return;
	auto mate = material->GetMaterialPtr(0);
	if (!mate)return;
	if (scale == 0.0f) {
		scale = 1.0f;
	}
	mate->SetTextureScale(XMFLOAT2(1.0f / scale, 1.0f / scale));
	
	mate->SetTexture(map, 0);

	//mCBUseTexture->UpdateSubresource();

	m_LU_Point = lu;
	m_RD_Point = rd;

	//if (!m_Player || !m_LU_Point || !m_RD_Point)return;
	//
	//auto p = m_Player->mTransform->WorldPosition()+ XMVectorSet(10000.0f, 10000.0f, 10000.0f, 1.0f);
	//auto l = m_LU_Point->mTransform->WorldPosition()+ XMVectorSet(10000.0f, 10000.0f, 10000.0f, 1.0f);
	//auto r = m_RD_Point->mTransform->WorldPosition()+ XMVectorSet(10000.0f, 10000.0f, 10000.0f, 1.0f);
	//
	//p -= l;
	//
	//mate->mOffset.x = p.x / r.x - mate->mTexScale.x/2.0f;
	//mate->mOffset.y = p.z / r.z - mate->mTexScale.y / 2.0f;
	//
	//mate->ParamUpdate();

}

MiniMap * MiniMap::GetMiniMapUI()
{
	return g_Minimap;
}

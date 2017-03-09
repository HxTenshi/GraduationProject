#include "Fader.h"
#include "h_standard.h"
# include "UniqueObject.h"
# include "hintDraw.h"
#include <istream>
//生成時に呼ばれます（エディター中も呼ばれます）
void Fader::Initialize(){
	if (!m_faderTexObj) return;
	m_isFadeIn = false;
	m_isSceneChange = false;
	m_nextScenePass = "";

	m_faderTexObj->Enable();
	auto material = m_faderTexObj->GetComponent<MaterialComponent>();
	if (!material) return;

	//material->GetMaterialPtr(0)->SetTexture(blackTex);
	material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1,1,1,1));
	m_SceneChange = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Fader::Start(){

}

//毎フレーム呼ばれます
void Fader::Update(){
	if (!m_faderTexObj) return;
	//マテリアルを取得
	auto material = m_faderTexObj->GetComponent<MaterialComponent>();
	if (!material) return;
	//マテリアルの色を取得
	XMFLOAT4 color = material->GetMaterialPtr(0)->GetAlbedo();

	//フラグに応じて加算減算
	if (!m_isFadeIn) color.w -= 1.0f * Hx::DeltaTime()->GetDeltaTime();
	else color.w += 1.0f * Hx::DeltaTime()->GetDeltaTime(); 

	color.w = max(min(color.w, 1.0f), 0.0f);
	material->GetMaterialPtr(0)->SetAlbedo(color);

	if (m_isSceneChange && color.w >= 1.0f) {
		//遷移前にテクスチャを変更
		//material->GetMaterialPtr(0)->SetTexture(loadingTex);
		//Hx::DestroyObject(UniqueObject::GetPlayer()->mTransform->GetParent());

		if (m_Destoroy)Hx::FindActor("EventHitDraw")->GetScript<hintDraw>()->OnFinish();
		//Hx::Debug()->Log(m_nextScenePass + "に遷移しました");
		//Hx::LoadScene(m_nextScenePass);
		Hx::Debug()->Log("LoadingSceneに遷移します");
		if (m_SceneChange == 0)Hx::Instance(m_BlackTex);
		m_SceneChange++;
		if(m_SceneChange == 2)
		Hx::LoadScene("Assets/Mossan/Loading.scene");
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Fader::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Fader::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Fader::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Fader::OnCollideExit(GameObject target){
	(void)target;
}

void Fader::OnSceneChnage(std::string nextScenePass, bool isDestroy){
	if (m_isSceneChange) return;

	m_Destoroy = isDestroy;
	m_nextScenePass = nextScenePass;
	m_isFadeIn = true;
	m_isSceneChange = true;
	
	std::ofstream ofs("Assets/data/NextScenePass.csv");
	ofs << nextScenePass << std::endl;
}

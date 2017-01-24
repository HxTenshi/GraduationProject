#include "ConfigScene.h"
#include "h_standard.h"
#include "h_component.h"

ConfigScene::ConfigScene() {
	
}

//生成時に呼ばれます（エディター中も呼ばれます）
void ConfigScene::Initialize(){
	m_texObjs.push_back(m_volumeTexObj);
	m_texObjs.push_back(m_howToTexObj);
	m_texObjs.push_back(m_comboTexObj);
	m_texObjs.push_back(m_toTitleTexObj);
	num = 0;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void ConfigScene::Start(){
}

//毎フレーム呼ばれます
void ConfigScene::Update(){
	//カーソルの移動
	CursourMove();
	//シーンの決定
	EnterScene(num);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void ConfigScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void ConfigScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void ConfigScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void ConfigScene::OnCollideExit(GameObject target){
	(void)target;
}

//カーソルの移動
void ConfigScene::CursourMove(){
	//nullチェック
	if (m_texObjs[0] == NULL)return;
	if (m_texObjs[1] == NULL)return;
	if (m_texObjs[2] == NULL)return;
	if (m_texObjs[3] == NULL)return;


	if (Input::Trigger(KeyCode::Key_UP)) num--;
	if (Input::Trigger(KeyCode::Key_DOWN)) num++;

	//クランプ処理
	int minNum = 0;
	int maxNum = m_texObjs.size() - 1;
	num = (int)min(max(minNum, num), maxNum);

	//囲いの座標移動
	XMVECTOR kakoiPos = m_texObjs[num]->mTransform->Position();
	m_kakoiTexObj->mTransform->Position(kakoiPos);
}

//シーンの決定
void ConfigScene::EnterScene(int num){
	if (Input::Trigger(KeyCode::Key_SPACE)) {
		std::string nextScenePass = GetScenePass(num);
		Hx::Debug()->Log(nextScenePass + "に遷移しました");
		Hx::LoadScene(nextScenePass);
	}
}

//シーンのパスを取得
std::string ConfigScene::GetScenePass(int num){
	std::string sceneNamesPass[] = {
		m_volumeScenePass,
		m_howtoScenePass,
		m_comboScenePass,
		m_titleScenePass
	};
	return sceneNamesPass[num];
}

#include "LoadingScene.h"
#include "FuckTenshiEngine.h"
#include "h_standard.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void LoadingScene::Initialize(){
	isload = false;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void LoadingScene::Start(){
	
}

//毎フレーム呼ばれます
void LoadingScene::Update(){
	if (isload) return;
	isload = true;
	std::vector<std::vector<std::string>> arrays = Mossan::CSVScript::readCSV("Assets/data/NextScenePass.csv");
	Hx::Debug()->Log(arrays[0][0] + "に遷移します");
	Hx::LoadScene(arrays[0][0]);
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void LoadingScene::Finish(){

}

//コライダーとのヒット時に呼ばれます
void LoadingScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void LoadingScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void LoadingScene::OnCollideExit(GameObject target){
	(void)target;
}
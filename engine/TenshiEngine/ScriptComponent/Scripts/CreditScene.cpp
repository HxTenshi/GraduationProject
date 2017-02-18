#include "CreditScene.h"
#include "CreditDraw.h"
#include "Fader.h"	
#include <h_standard.h>
#include <h_component.h>

//生成時に呼ばれます（エディター中も呼ばれます）
void CreditScene::Initialize(){
	m_rightArrow = Mossan::ArrowControl(arrowRight);
	m_leftArrow = Mossan::ArrowControl(arrowLeft);

	m_objList.push_back(m_weaponObj1);
	m_objList.push_back(m_weaponObj2);
	m_objList.push_back(m_weaponObj3);
	m_objList.push_back(m_weaponObj4);
	m_objList.push_back(m_weaponObj5);
	m_objList.push_back(m_weaponObj6);
	m_objList.push_back(m_weaponObj7);
	m_objList.push_back(m_weaponObj8);
	m_objList.push_back(m_weaponObj9);
	m_objList.push_back(m_weaponObj10);
	m_curNum--;
	m_preNum = m_curNum;
	m_lerpTime = 0.0f;
	m_timer = 0.0f;
	ChangeTexture();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void CreditScene::Start(){

}

//毎フレーム呼ばれます
void CreditScene::Update(){
	auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_LStick);
	bool isRight = ls.x > 0.5f;
	bool isLeft = ls.x< -0.5f;


	//カメラの移動が完了していたなら操作を受け付ける
	if (IsCameraMoved()) {
		//クレジットを表示
		CreditOnDraw();

		//選択中のオブジェ
		if (Input::Trigger(KeyCode::Key_RIGHT) || isRight) { 
			//右端まで移動していたなら
			if (IsRightOver()) return;
			CursorFunc(1); 
		}
		if (Input::Trigger(KeyCode::Key_LEFT) || isLeft) {
			//左端まで移動していたなら
			if (IsLeftOver()) return;
			CursorFunc(-1);
		}

		//端まで移動していたならそちら側の矢印は表示しない
		if (IsRightOver()) m_rightArrow.Disable();
		else m_rightArrow.Enable();
		if (IsLeftOver())m_leftArrow.Disable();
		else m_leftArrow.Enable();
	}
	else {
		//非表示
		CreditNoDraw();
		m_rightArrow.Disable();
		m_leftArrow.Disable();
	}
	m_timer += 1.0f * Hx::DeltaTime()->GetDeltaTime();
	m_rightArrow.Update(m_timer);
	m_leftArrow.Update(-m_timer);

	//lerp計算
	m_lerpTime += 2.0f * Hx::DeltaTime()->GetDeltaTime();
	m_lerpTime = min(max(0.0f, m_lerpTime), 1.0f);
	//カメラ移動
	XMVECTOR preTempPos = m_objList[m_preNum]->mTransform->Position();
	XMVECTOR curTempPos = m_objList[m_curNum]->mTransform->Position();
	XMVECTOR cameraTempPos = XMVectorLerp(preTempPos, curTempPos, m_lerpTime * m_lerpTime);
	cameraTempPos += m_CameraDistance;
	m_CameraObj->mTransform->Position(cameraTempPos);
	
	bool isEnter = Input::Trigger(PAD_X_KeyCode::Button_B);
	if (Input::Trigger(KeyCode::Key_SPACE) || isEnter) {
		if (!m_fader) return;
		auto fader = m_fader->GetScript<Fader>();
		fader->OnSceneChnage("Assets/Title.scene");
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void CreditScene::Finish(){
	
}

//コライダーとのヒット時に呼ばれます
void CreditScene::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void CreditScene::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void CreditScene::OnCollideExit(GameObject target){
	(void)target;
}

//クレジット表記
void CreditScene::CreditOnDraw(){
	auto material = m_Itimaie->GetComponent<MaterialComponent>();
	XMFLOAT4 tempColor = material->GetMaterialPtr(0)->GetAlbedo();
	tempColor.w += 2.0f * Hx::DeltaTime()->GetDeltaTime();
	if (tempColor.w > 1.0f) tempColor.w = 1.0f;
	material->SetAlbedoColor(tempColor);
}

//クレジット非表記
void CreditScene::CreditNoDraw()
{
	auto material = m_Itimaie->GetComponent<MaterialComponent>();
	XMFLOAT4 tempColor = material->GetMaterialPtr(0)->GetAlbedo();
	tempColor.w = 0.0f;
	material->SetAlbedoColor(tempColor);
}

//テクスチャを切り替える
void CreditScene::ChangeTexture(){
	auto material = m_Itimaie->GetComponent<MaterialComponent>();

	auto creditDraw = m_objList[m_curNum]->GetScript<CreditDraw>();
	if (creditDraw == NULL) {
		Hx::Debug()->Log("CreditScene::ChangeTexture Null");
		return;
	}
	auto texture = creditDraw->GetTexture();
	material->GetMaterialPtr(0)->SetTexture(texture);
	material->GetMaterialPtr(0)->SetAlbedo(XMFLOAT4(1, 1, 1, 0));
}

void CreditScene::CursorFunc(int num){
	//lerpの初期化
	m_lerpTime = 0.0f;
	//前回の選択を更新
	m_preNum = m_curNum;
	//選択の更新
	m_curNum += num;
	//リストの最大値を取得
	int maxNum = m_objList.size() - 1;
	//クランプ処理
	m_curNum = min(max(0, m_curNum), maxNum);
	//テクスチャを切り替える
	ChangeTexture();
}

bool CreditScene::IsCameraMoved()
{
	return m_lerpTime >= 1.0f;
}

bool CreditScene::IsRightOver()
{
	return m_curNum == m_objList.size() - 1;
}

bool CreditScene::IsLeftOver()
{
	return m_curNum == 0;
}

/****************************************************

	ArrowControl
	
*****************************************************/
namespace Mossan {

	Mossan::ArrowControl::ArrowControl(){
	}

	Mossan::ArrowControl::ArrowControl(GameObject gameObject){
		this->gameObject = gameObject;
		m_initPos = gameObject->mTransform->Position();
	}

	void Mossan::ArrowControl::Update(float timer){
		//左右の移動幅
		float deflection = 20.0f;
		XMVECTOR tempPos = m_initPos;
		tempPos.x = m_initPos.x + sin(timer*4.0f) * deflection;
		gameObject->mTransform->Position(tempPos);
	}

	void Mossan::ArrowControl::Enable(){
		gameObject->Enable();
	}

	void Mossan::ArrowControl::Disable(){
		gameObject->Disable();
	}
};

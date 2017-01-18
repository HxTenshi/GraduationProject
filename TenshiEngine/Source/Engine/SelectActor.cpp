


#ifdef _ENGINE_MODE

#include "SelectActor.h"

#include <set>
#include <map>

#include "Device/DirectX11Device.h"

#include "Game/Actor.h"
#include "Game/Component/TransformComponent.h"
#include "Game/Component/MaterialComponent.h"
#include "Game/Component/ModelComponent.h"
#include "Game/Component/MeshDrawComponent.h"
#include "Game/Component/PhysXColliderComponent.h"
#include "Game/Component/CameraComponent.h"

#include "Window/Window.h"

#include "Input/Input.h"


#include "Graphic/Model/Model.h"

#include "Game/EngineObject/EditorCamera.h"
#include "PhysX/PhysX3.h"


#include "Game/Game.h"

#include "Game/RenderingSystem.h"
#include "Engine/AssetLoad.h"

//�I�����̈ړ��I�u�W�F�N�g
class Arrow : public Actor{
public:

	Arrow(){
		Name("arrow");

		mTransform = make_shared<TransformComponent>();
		mComponents.AddComponent<TransformComponent>(mTransform);

		auto model = make_shared<ModelComponent>();

		MeshAsset asset;
		MD5::MD5HashCode hash;
		AssetDataBase::FilePath2Hash("EngineResource/Arrow.pmx.tesmesh", hash);
		AssetLoad::Instance(hash, asset);
		model->Load(asset);
		mComponents.AddComponent<ModelComponent>(model);
		auto material = make_shared<MaterialComponent>();
		mComponents.AddComponent<MaterialComponent>(material);

		//mComponents.AddComponent<MeshDrawComponent>();

		mComponents.AddComponent<PhysXComponent>();
		auto phy = mComponents.GetComponent<PhysXComponent>();
		phy->mIsEngineMode = true;
		mComponents.AddComponent<PhysXColliderComponent>();

		this->SetUpdateStageCollQueue([this, asset](){
			auto phy = mComponents.GetComponent<PhysXComponent>();
			phy->SetKinematic(true);
			auto col = mComponents.GetComponent<PhysXColliderComponent>();
			col->ChangeShapeMesh(asset);
		});

	}
	~Arrow(){
	}
	//void Update(float deltaTime)override{
	//	(void)deltaTime;
	//}
private:
};



void EditGuide::SetGuideTransform(const XMVECTOR& pos, const XMVECTOR& quat){
	mGuidePosition = pos;
	mGuideRotate = quat;
	UpdateGuideTransform(mGuidePosition, mGuideRotate);
}
void EditGuide::SetGuideTransform(const XMVECTOR& pos) {
	mGuidePosition = pos;
	UpdateGuideTransform(mGuidePosition, mGuideRotate);
}
void EditGuide::UpdateGuideTransform(const XMVECTOR& pos, const XMVECTOR& quat){
	const XMVECTOR v[] = {
		XMVectorSet(0, 0, -90 * (XM_PI / 180.0f), 1),
		XMVectorSet(0, 0, 0, 1),
		XMVectorSet(90 * (XM_PI / 180.0f), 0, 0, 1)
	};

	for (int i = 0; i < 3; i++){
		mGuide[i]->mTransform->Position(pos);

		auto q = XMQuaternionRotationRollPitchYawFromVector(v[i]);
		q = XMQuaternionMultiply(q, quat);
		mGuide[i]->mTransform->Quaternion(q);
	}

	
	auto cam = Game::GetMainCamera();
	if (cam){
		auto wp = cam->gameObject->mTransform->WorldPosition();
		auto l = XMVector3Length(pos - wp).x / 100.0f;
		for (int i = 0; i < 3; i++){
			mGuide[i]->mTransform->Scale(XMVectorSet(l, l, l, 1));
		}
	}
}

int EditGuide::SetGuideHit(Actor* act){
	mSelectGuide = -1;
	for (int i = 0; i < 3; i++){
		if (mGuide[i].Get() != act)continue;
		mSelectGuide = i;
	}
	return mSelectGuide;
}

void EditGuide::Update(){

	for (int _i = 0; _i < 3; _i++){
		mGuide[_i]->UpdateComponent();
	}
	//�A���[�`��
	Game::AddDrawList(DrawStage::Engine, std::function<void()>([&](){

		auto render = RenderingEngine::GetEngine(ContextType::MainDeferrd);

		render->PushSet(DepthStencil::Preset::DS_All_Alawys);
		render->PushSet(Rasterizer::Preset::RS_Back_Solid);


		auto f = Game::GetMainCamera()->gameObject->mTransform->Forward();

		auto func = [](const std::pair<float, std::function<void()>>& p1, const std::pair<float, std::function<void()>>& p2)
		{
			return p1.first > p2.first;
		};
		std::set<std::pair<float, std::function<void()>>, std::function<bool(const std::pair<float, std::function<void()>>&, const std::pair<float, std::function<void()>>&)> > draws(func);


		for (int _i = 0; _i < 3; _i++){
			int i = _i;

			auto mModel = mGuide[i]->GetComponent<ModelComponent>();
			Model& model = *mModel->mModel;

			auto au = mGuide[i]->mTransform->Up();
			auto dot = XMVector3Dot(au, f);

			auto result = draws.insert(std::pair<float, std::function<void()>>((float)dot.x, [=](){
				mModel->SetMatrix();
				model.Draw(render->m_Context, mGuide[i]->GetComponent<MaterialComponent>());
			}));
			if (!result.second){
				dot.x -= 0.01f;
				draws.insert(std::pair<float, std::function<void()>>((float)dot.x, [=](){
					mModel->SetMatrix();
					model.Draw(render->m_Context, mGuide[i]->GetComponent<MaterialComponent>());
				}));
			}
		}
		//Game::GetMainCamera()->SetOrthographic();

		for (auto & d : draws){
			d.second();
		}

		render->PopRS();
		render->PopDS();
	}));
}


void EditGuide::Enable(){
	for (int i = 0; i < 3; i++){
		mGuide[i]->Enable();
	}
}
void EditGuide::Disable(){
	for (int i = 0; i < 3; i++){
		mGuide[i]->Disable();
	}
}

bool EditGuide::IsTransformModeLocal()
{
	return m_TransformMode == TransformMode::Local;
}

void EditGuide::ChangeTransformMode()
{
	m_TransformMode = (TransformMode)((m_TransformMode+1)%TransformMode::Count);
}

class MoveEditGuide :public EditGuide{
public:
	MoveEditGuide(){

		mMove = XMVectorSet(0, 0, 0, 1);

		float s = 0.1f;

		XMFLOAT4 color[] = {
			XMFLOAT4(1, 0, 0, 1),
			XMFLOAT4(0, 1, 0, 1),
			XMFLOAT4(0, 0, 1, 1)
		};
		XMVECTOR v[] = {
			XMVectorSet(0, 0, -90, 1),
			XMVectorSet(0, 0, 0, 1),
			XMVectorSet(90, 0, 0, 1)
		};

		for (int i = 0; i < 3; i++){

			mGuide[i] = make_shared<Arrow>();
			Game::AddEngineObject(mGuide[i]);

			mGuide[i]->mTransform->Scale(XMVectorSet(s, s, s, 1));
			mGuide[i]->mTransform->DegreeRotate(v[i]);
			auto mate = mGuide[i]->GetComponent<MaterialComponent>();
			if (mate){
				auto m = mate->GetMaterial(0);
				m.Create("EngineResource/NoLighting.fx");
				m.mCBMaterial.mParam.Diffuse = color[i];
				mate->SetMaterial(0, m);
			}

		}
		
	}
	~MoveEditGuide(){}

	void GuideDrag(float pow)override{
		mMove = mGuide[mSelectGuide]->mTransform->Up();
		mMove = XMVector3Normalize(mMove);
		mMove *= pow;
		mMove.w = 0.0f;
	}

	void UpdateTransform(std::list<Actor*>& actors)override{

		//SetGuideTransform(mGuidePosition + mMove);

		for (auto& act : actors){
			auto move = mMove * act->mTransform->GetParent()->mTransform->LossyScale();
			auto pos = act->mTransform->WorldPosition();
			act->mTransform->WorldPosition(pos + move);
		}
	}
private:
	XMVECTOR mMove;
};

class ScaleEditGuide :public EditGuide{
public:
	ScaleEditGuide(){

		mScale = XMVectorSet(0, 0, 0, 1);

		float s = 0.1f;

		XMFLOAT4 color[] = {
			XMFLOAT4(1, 0, 0, 1),
			XMFLOAT4(0, 1, 0, 1),
			XMFLOAT4(0, 0, 1, 1)
		};
		XMVECTOR v[] = {
			XMVectorSet(0, 0, -90, 1),
			XMVectorSet(0, 0, 0, 1),
			XMVectorSet(90, 0, 0, 1)
		};

		for (int i = 0; i < 3; i++){

			mGuide[i] = make_shared<Arrow>();
			Game::AddEngineObject(mGuide[i]);

			mGuide[i]->mTransform->Scale(XMVectorSet(s, s, s, 1));
			mGuide[i]->mTransform->DegreeRotate(v[i]);
			auto mate = mGuide[i]->GetComponent<MaterialComponent>();
			if (mate){
				auto m = mate->GetMaterial(0);
				m.Create("EngineResource/NoLighting.fx");
				m.mCBMaterial.mParam.Diffuse = color[i];
				mate->SetMaterial(0, m);
			}

		}

	}
	~ScaleEditGuide(){}

	void GuideDrag(float pow)override{
		XMVECTOR s[] = {
			XMVectorSet(1, 0, 0, 1),
			XMVectorSet(0, 1, 0, 1),
			XMVectorSet(0, 0, 1, 1)
		};
		mScale = s[mSelectGuide];
		//mScale = XMVector3Normalize(mScale);
		mScale *= pow;
		mScale.w = 0.0f;
	}

	void UpdateTransform(std::list<Actor*>& actors)override{

		for (auto& act : actors){
			auto s = act->mTransform->Scale();
			act->mTransform->Scale(s + mScale);
		}
	}
private:
	XMVECTOR mScale;
};

class RotateEditGuide :public EditGuide{
public:
	RotateEditGuide(){

		mQuat = XMVectorSet(0, 0, 0, 1);

		float s = 0.1f;

		XMFLOAT4 color[] = {
			XMFLOAT4(1, 0, 0, 1),
			XMFLOAT4(0, 1, 0, 1),
			XMFLOAT4(0, 0, 1, 1)
		};
		XMVECTOR v[] = {
			XMVectorSet(0, 0, -90, 1),
			XMVectorSet(0, 0, 0, 1),
			XMVectorSet(90, 0, 0, 1)
		};

		for (int i = 0; i < 3; i++){

			mGuide[i] = make_shared<Arrow>();
			Game::AddEngineObject(mGuide[i]);

			mGuide[i]->mTransform->Scale(XMVectorSet(s, s, s, 1));
			mGuide[i]->mTransform->DegreeRotate(v[i]);
			auto mate = mGuide[i]->GetComponent<MaterialComponent>();
			if (mate){
				auto m = mate->GetMaterial(0);
				m.Create("EngineResource/NoLighting.fx");
				m.mCBMaterial.mParam.Diffuse = color[i];
				mate->SetMaterial(0, m);
			}

		}

	}
	~RotateEditGuide(){}

	void GuideDrag(float pow)override{
		XMVECTOR q[] = {
			XMVectorSet(1, 0, 0, 1),
			XMVectorSet(0, 1, 0, 1),
			XMVectorSet(0, 0, 1, 1)
		};
		mQuat = q[mSelectGuide];

		//mQuat = mGuide[mSelectGuide]->mTransform->Up();
		//mQuat = XMVector3Normalize(mQuat);
		mQuat *= pow;
		mQuat.w = 0.0f;
		mQuat *= (XM_PI / 180.0f);
	}

	void UpdateTransform(std::list<Actor*>& actors)override{

		for (auto& act : actors){
			auto quat = act->mTransform->Quaternion();
			auto q = XMQuaternionRotationRollPitchYawFromVector(mQuat);
			q = XMQuaternionMultiply(q,quat);
			act->mTransform->Quaternion(q);
		}
	}
private:
	XMVECTOR mQuat;
};






Selects::Selects(){
	mLastVect = XMVectorZero();
}
Selects::~Selects(){
	CopyDelete();
}
void Selects::OneSelect(Actor* act){
	mSelects.clear();
	if (act){
		mSelects.push_back(act);
	}
}
bool Selects::TriggerSelect(Actor* act){
	//�k���͖���
	if (!act)return false;
	int num = (int)mSelects.size();
	mSelects.remove(act);
	//�폜����Ă����
	if (num != (int)mSelects.size()){
		return false;
	}
	//�폜����Ȃ���Βǉ�
	mSelects.push_back(act);
	return true;
}
void Selects::Copy(){
	CopyDelete();
	for (auto& act : mSelects){
		auto data = new picojson::value();
		act->ExportData(*data, true);

		mCopy.push_back(data);
	}
}
void Selects::CopyDelete(){
	for (auto act : mCopy){
		delete act;
	}
	mCopy.clear();
}
void Selects::Paste(){
	mSelects.clear();
	for (auto& act : mCopy){
		auto obj = Game::Get()->Instance(*act,true,false);
		//auto postactor = make_shared<Actor>();
		//postactor->ImportDataAndNewID(*act, [](auto o) {Game::AddObject(o); });
		//Game::AddObject(postactor,true);
		TriggerSelect(obj.Get());
	}
}

void Selects::SetUndo(){
	for (auto& act : mSelects){
		Game::SetUndo(act);
	}
}

int Selects::SelectNum(){
	return (int)mSelects.size();
}
XMVECTOR Selects::GetPosition(){
	XMVECTOR pos = XMVectorZero();
	for (auto& act : mSelects){
		pos += act->mTransform->WorldPosition();
	}
	int num = (int)mSelects.size();
	if (num){
		pos = pos / (FLOAT)num;
	}
	return pos;
}
XMVECTOR Selects::GetQuaternion()
{
	int num = (int)mSelects.size();
	if (num==1) {
		return mSelects.front()->mTransform->WorldQuaternion();
	}
	return XMQuaternionIdentity();
}

Actor* Selects::GetSelectOne(){
	int num = (int)mSelects.size();
	if (num){
		return *mSelects.begin();
	}
	return NULL;
}
std::list<Actor*>& Selects::GetSelects(){
	return mSelects;
}

SelectUndo::SelectUndo()
{
}
SelectUndo::~SelectUndo(){
	for (auto& com : mCommands){
		delete com.second;
	}
}
void SelectUndo::Clear(){
	for (auto& com : mCommands){
		delete com.second;
	}
	mCommands.clear();
}
void SelectUndo::Set(Actor* act){
	if (!act)return;
	auto temp = mCommands.find((int)act);
	if (temp != mCommands.end()){
		delete temp->second;
		temp->second = new ActorUndoCommand(act);
	}
	else{
		mCommands.insert(std::make_pair((int)act, new ActorUndoCommand(act)));
	}
}
void SelectUndo::Remove(Actor* act){
	if (!act)return;
	auto temp = mCommands.find((int)act);
	if (temp != mCommands.end()){
		delete temp->second;
		mCommands.erase(temp);
	}
}
void SelectUndo::PushUndo(){
	if (mCommands.size() == 0)return;

	auto Link = new std::list<ICommand*>();

	for (auto& com : mCommands){
		Link->push_back(com.second);
	}
	Game::SetUndo(new LinkUndoCommand(Link));
	mCommands.clear();
}

SelectActor::SelectActor()
	:mCreateInspector(false)
	, mDontTreeViewSelect(false)
	, m_DrawMode(SelectObjectDrawMode::Wire)
{
	mSelectAsset = false;
	mCurrentGuide = 0;
	//mDragBox = -1;

	for (int i = 0; i < 3; i++){
		mEditGuide[i] = NULL;
	}
	mSelectWireMaterial.mDiffuse.x = 0.95f;
	mSelectWireMaterial.mDiffuse.y = 0.6f;
	mSelectWireMaterial.mDiffuse.z = 0.1f;
	mSelectWireMaterial.Create("EngineResource/NoLighting.fx");
	
	mSelectPhysxWireMaterial.mDiffuse.x = 0.1f;
	mSelectPhysxWireMaterial.mDiffuse.y = 0.95f;
	mSelectPhysxWireMaterial.mDiffuse.z = 0.6f;
	mSelectPhysxWireMaterial.Create("EngineResource/NoLighting.fx");
}

void SelectActor::Finish(){
	for (int i = 0; i < 3; i++){
		if (mEditGuide[i])
			delete mEditGuide[i];
	}
}

void SelectActor::Initialize(){
	mEditGuide[0] = new MoveEditGuide();
	mEditGuide[1] = new ScaleEditGuide();
	mEditGuide[2] = new RotateEditGuide();
}

Actor* SelectActor::GetSelectOne(){
	return mSelects.SelectNum()==1?mSelects.GetSelectOne():NULL;
}

void SelectActor::UpdateInspector(){
	if (mSelects.SelectNum() != 1 && !mSelectAsset)return;

	if (!mCreateInspector){
		if (mSelectAsset){
			if (mSelects.SelectNum() <= 1){
				AssetDataBase::CreateInspector(mAssetFileName.c_str());
			}
		}
		else{
			if (mSelects.SelectNum() == 1){
				mSelects.GetSelectOne()->CreateInspector();
				if (!mDontTreeViewSelect){
					Window::SelectTreeViewItem(mSelects.GetSelectOne()->mTreeViewPtr);
				}
				mDontTreeViewSelect = false;
			}
		}

		mCreateInspector = true;
	}
	else{

		static unsigned long time_start = timeGetTime();
		unsigned long current_time = timeGetTime();
		unsigned long b = (current_time - time_start);
		if (b >= 16){
			time_start = timeGetTime();
			//�������ǂ����Ȃ��ꍇ������
			Window::UpdateInspector();
		}
	}
}
void SelectActor::Update(){
	//(void)deltaTime;
	//if (Window::GetTreeViewWindow()->IsActive()){
	//	void* s = (Actor*)Window::GetTreeViewWindow()->GetSelectItem();
	//	
	//	if (s){
	//		if (s != mTreeSelect){
	//			mTreeSelect = (Actor*)s;
	//			mSelect = mTreeSelect;
	//		}
	//	}
	//}


	if (Input::Trigger(MouseCode::Left)) {

		//�K�C�h���N���b�N���Ă��Ȃ���
		if (!ChackHitRay()) {

			auto camera = Game::Get()->GetEditorCamera();
			int x, y;
			Input::MousePosition(&x, &y);
			XMVECTOR point = XMVectorSet((FLOAT)x, (FLOAT)y, 0.0f, 1.0f);
			XMVECTOR vect = camera->PointRayVector(point);
			XMVECTOR pos = camera->GetPosition();
			std::list<::RaycastHit> hits;


			//�I�������������o
			Game::GetPhysX()->Raycast(pos, vect,1000, [&](::RaycastHit* hit) {
				hits.push_back(*hit);
			});
			//�Q�[���I�u�W�F�N�g�̑I���ɐ���
			if (hits.size() >= 1) {

				hits.sort([](auto &hit1, auto &hit2) {
					return hit1.distance < hit2.distance;
				});

				//�I������Ă���Ύ��̂�I��
				auto select = GetSelectOne();
				GameObject result = NULL;
				if (select) {
					bool nextSelect = false;
					for (auto &hit : hits) {
						if (nextSelect) {
							result = hit.hit;
							nextSelect = false;
						}

						//�������̂�����Ύ��̂�I��
						if (hit.hit.Get() == select) {
							nextSelect = true;
						}
					}
				}

				//�Ō�܂ōs�������������̂��Ȃ���� �܂���
				//�I������Ă��Ȃ���Έ�ԋ߂��̂�I��
				if (!result) {
					result = hits.front().hit;
				}

				SetSelect(result.Get());
			}
			//�Q�[���I�u�W�F�N�g�̑I���Ɏ��s
			else{
				SetSelect(NULL);
			}
		}
	}


	if (!mSelects.SelectNum() || mSelectAsset)return;

	if (EngineInput::Down(KeyCode::Key_LCONTROL) && EngineInput::Trigger(KeyCode::Key_C)){
		mSelects.Copy();
	}
	if (EngineInput::Down(KeyCode::Key_LCONTROL) && EngineInput::Trigger(KeyCode::Key_V)){
		mSelects.Paste();
		SelectActor::ReCreateInspector();
	}
	if (EngineInput::Down(KeyCode::Key_LCONTROL) && EngineInput::Trigger(KeyCode::Key_D)) {
		mSelects.Copy();
		mSelects.Paste();
		SelectActor::ReCreateInspector();
	}

	int giude = -1;
	int g = mCurrentGuide;
	if (Input::Down(MouseCode::Right)==0 && EngineInput::Trigger(KeyCode::Key_W)){
		giude = 0;
		mIsDragMode = false;
	}
	if (Input::Down(MouseCode::Right) == 0 && EngineInput::Trigger(KeyCode::Key_E)){
		giude = 1;
		mIsDragMode = false;
	}
	if (Input::Down(MouseCode::Right) == 0 && EngineInput::Trigger(KeyCode::Key_R)){
		giude = 2;
		mIsDragMode = false;
	}
	if (giude != -1 && giude != mCurrentGuide){
		mCurrentGuide = giude;
		for (int i = 0; i < 3; i++){
			if (mCurrentGuide == i){
				mEditGuide[i]->Enable();
			}
			else{
				mEditGuide[i]->Disable();
			}
		}
	}else if (giude != -1) {
		mEditGuide[mCurrentGuide]->ChangeTransformMode();
	}

	auto tarpos = mSelects.GetPosition();
	auto tarquat = XMQuaternionIdentity();
	if (mEditGuide[mCurrentGuide]->IsTransformModeLocal()) {
		tarquat = mSelects.GetQuaternion();
	}
	//mVectorBox[0]->mTransform->Position(tar);
	//mVectorBox[1]->mTransform->Position(tar);
	//mVectorBox[2]->mTransform->Position(tar);
	//
	//auto cam = Game::GetMainCamera();
	//if (cam){
	//	auto pos = cam->gameObject->mTransform->WorldPosition();
	//	auto l = XMVector3Length(tar - pos).x/100.0f;
	//	mVectorBox[0]->mTransform->Scale(XMVectorSet(l, l, l, 1));
	//	mVectorBox[1]->mTransform->Scale(XMVectorSet(l, l, l, 1));
	//	mVectorBox[2]->mTransform->Scale(XMVectorSet(l, l, l, 1));
	//
	//
	//	auto col = mVectorBox[0]->GetComponent<PhysXColliderComponent>();
	//	col->SetScale(XMVectorSet(l, l, l, 1));
	//	col = mVectorBox[1]->GetComponent<PhysXColliderComponent>();
	//	col->SetScale(XMVectorSet(l, l, l, 1));
	//	col = mVectorBox[2]->GetComponent<PhysXColliderComponent>();
	//	col->SetScale(XMVectorSet(l, l, l, 1));
	//
	//}

	mEditGuide[mCurrentGuide]->Update();
	mEditGuide[mCurrentGuide]->UpdateGuideTransform(tarpos, tarquat);

	static float mBeforePow = 0.0f;
	float pow = 0.0f;
	if (mIsDragMode){
		if (Input::Down(MouseCode::Left)){
			int x, y;
			Input::MouseLeftDragVector(&x, &y);
			//float l = XMVector2Length(XMVectorSet(x,-y,0,1)).x;
			pow = y *0.05f;
			if (Input::Down(KeyCode::Key_LSHIFT)){
				pow += 0.5f;
				pow = (float)(int)pow;
			}
			mEditGuide[mCurrentGuide]->GuideDrag(mBeforePow-pow);
			mEditGuide[mCurrentGuide]->UpdateTransform(mSelects.GetSelects());
		}

		if (Input::Up(MouseCode::Left)){
			mSelects.SetUndo();
			mIsDragMode = false;
		}
		//�E�N���b�N�ŃL�����Z��
		else if (Input::Trigger(MouseCode::Right)) {
			mEditGuide[mCurrentGuide]->GuideDrag(mBeforePow);
			mEditGuide[mCurrentGuide]->UpdateTransform(mSelects.GetSelects());
			mIsDragMode = false;
		}
	}
	mBeforePow = pow;

	//�h���[���[�h�ύX
	if (EngineInput::Trigger(KeyCode::Key_V)) {
		m_DrawMode = (SelectObjectDrawMode::Enum) ((m_DrawMode+1) % SelectObjectDrawMode::Count);
	}

	//�`��
	Draw();

}
void SelectActor::Draw(){

	switch (m_DrawMode)
	{
	case SelectObjectDrawMode::None:
		break;
	case SelectObjectDrawMode::Wire:
		for (auto select : mSelects.GetSelects()) {
			DrawMeshWire(select->shared_from_this());
			DrawPhysxWire(select->shared_from_this());
		}
		break;
	case SelectObjectDrawMode::ChildrenWire: {
		//�ċA
		std::function<void(GameObject)> re = [&](GameObject object) {
			DrawMeshWire(object);
			DrawPhysxWire(object);
			for (auto child : object->mTransform->Children()) {
				re(child);
			}
		};
		for (auto select : mSelects.GetSelects()) {
			re(select->shared_from_this());
		}
		break;
	}
	case SelectObjectDrawMode::Count:
		break;
	default:
		break;
	}
}

void SelectActor::ReleaseSelect(Actor* actor){

	mSelects.GetSelects().remove(actor);
}

void SelectActor::SetSelect(Actor* select, bool dontTreeViewSelect){
	mIsDragMode = false;

	if (mSelectAsset){
		mSelects.OneSelect(NULL);
	}
	mSelectAsset = false;

	//���݂��Ȃ����NULL
	if (!Game::FindActor(select) && !Game::FindEngineActor(select)){
		select = NULL;
	}
	if (EngineInput::Down(KeyCode::Key_LCONTROL)){
		if (mSelects.TriggerSelect(select)){
			mSelectUndo.Set(select);
		}
		else{
			mSelectUndo.Remove(select);

		}
	}
	else{
		mSelects.OneSelect(select);
		mSelectUndo.Clear();
		mSelectUndo.Set(select);
	}

	Window::ClearInspector();
	mCreateInspector = false;
	mDontTreeViewSelect = dontTreeViewSelect;
}
void SelectActor::SetSelectAsset(Actor* select,const char* filename){
	mIsDragMode = false;
	if (!mSelectAsset){
		mSelects.OneSelect(NULL);
	}
	mSelectAsset = true;
	

	if (EngineInput::Down(KeyCode::Key_LCONTROL)){
		mSelects.TriggerSelect(select);
	}
	else{
		mSelects.OneSelect(select);
	}

	Window::ClearInspector();
	mCreateInspector = false;

	mAssetFileName = filename;

}

void SelectActor::ReCreateInspector(){
	Window::ClearInspector();
	mCreateInspector = false;

}

void SelectActor::DrawMeshWire(weak_ptr<Actor> object)
{
	Game::AddDrawList(DrawStage::Engine, std::function<void()>([&, object]() {
		if (!object)return;
		auto mModel = object->GetComponent<ModelComponent>();
		if (!mModel)return;
		Model& model = *mModel->mModel;

		auto render = RenderingEngine::GetEngine(ContextType::MainDeferrd);

		render->PushSet(DepthStencil::Preset::DS_Zero_Alawys);
		render->PushSet(Rasterizer::Preset::RS_Back_Wireframe);

		mModel->SetMatrix();
		model.Draw(render->m_Context, mSelectWireMaterial);


		render->PopRS();
		render->PopDS();
	}));
}

void SelectActor::DrawPhysxWire(weak_ptr<Actor> object)
{
	Game::AddDrawList(DrawStage::Engine, std::function<void()>([&, object]() {
		if (!object)return;
		auto com = object->GetComponent<PhysXColliderComponent>();
		if (!com)return;

		auto render = RenderingEngine::GetEngine(ContextType::MainDeferrd);

		render->PushSet(DepthStencil::Preset::DS_Zero_Alawys);
		render->PushSet(Rasterizer::Preset::RS_Back_Wireframe);


		com->DrawMesh(render->m_Context, mSelectPhysxWireMaterial);


		render->PopRS();
		render->PopDS();
	}));
}


bool SelectActor::ChackHitRay(){

	if (!mSelects.SelectNum() || mSelectAsset)return false;
	auto camera = Game::Get()->GetEditorCamera();

	int x, y;
	Input::MousePosition(&x, &y);
	XMVECTOR point = XMVectorSet((FLOAT)x, (FLOAT)y, 0.0f, 1.0f);
	XMVECTOR vect = camera->PointRayVector(point);
	XMVECTOR pos = camera->GetPosition();

	auto act = Game::GetPhysX()->EngineSceneRaycast(pos, vect);
	if (!act)return false;

	if (mEditGuide[mCurrentGuide]->SetGuideHit(act.Get()) == -1) return false;
	auto p = mSelects.GetPosition();
	mEditGuide[mCurrentGuide]->SetGuideTransform(p);
	mIsDragMode = true;
	return true;
	
}

void SelectActor::PushUndo(){
	mSelectUndo.PushUndo();
}


#endif
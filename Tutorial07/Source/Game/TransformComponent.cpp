
#include "TransformComponent.h"
#include "Window/Window.h"
#include "Window/InspectorWindow.h"

#include "Actor.h"

TransformComponent::TransformComponent()
	:mFixMatrixFlag(true){
	mMatrix = XMMatrixIdentity();
	mScale = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	mRotate = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	mPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

}

const XMVECTOR& TransformComponent::Scale() const{
	return mScale;
}
const XMVECTOR& TransformComponent::Rotate() const{
	return mRotate;
}
void TransformComponent::Scale(const XMVECTOR& scale){
	mScale = scale;
	mFixMatrixFlag = false;
}
void TransformComponent::Rotate(const XMVECTOR& rotate){
	mRotate = rotate;
	mFixMatrixFlag = false;
}

const XMMATRIX& TransformComponent::GetMatrix() const{
	Window::UpdateInspector();
	mFixMatrixFlag = false;
	if (!mFixMatrixFlag){
		//クォータニオンで計算
		if (mRotate.w != 1.0f){
			mMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
				XMMatrixScalingFromVector(mScale),
				XMMatrixRotationQuaternion(mRotate)),
				XMMatrixTranslationFromVector(mPosition));
		}
		//ロール、ピッチ、ヨウで計算
		else{
			mMatrix = XMMatrixMultiply(
				XMMatrixMultiply(
				XMMatrixScalingFromVector(mScale),
				//XMMatrixRotationQuaternion(mRotate)),
				XMMatrixRotationRollPitchYawFromVector(mRotate)),
				XMMatrixTranslationFromVector(mPosition));
		}
		//テスクチャー用（仮）
		mMatrix._44 = mPosition.w;
		mFixMatrixFlag = true;
	}
	return mMatrix;
}

void TransformComponent::CreateInspector(){

	std::function<void(float)> collbackpx = [&](float f){
		auto pos = this->Position();
		this->Position(XMVectorSet(f, pos.y, pos.z, pos.w));
	};
	std::function<void(float)> collbackpy = [&](float f){
		auto pos = this->Position();
		this->Position(XMVectorSet(pos.x, f, pos.z, pos.w));
	};
	std::function<void(float)> collbackpz = [&](float f){
		auto pos = this->Position();
		this->Position(XMVectorSet(pos.x, pos.y, f, pos.w));
	};
	std::function<void(float)> collbackrx = [&](float f){
		auto pos = this->Rotate();
		this->Rotate(XMVectorSet(f, pos.y, pos.z, pos.w));
	};
	std::function<void(float)> collbackry = [&](float f){
		auto pos = this->Rotate();
		this->Rotate(XMVectorSet(pos.x, f, pos.z, pos.w));
	};
	std::function<void(float)> collbackrz = [&](float f){
		auto pos = this->Rotate();
		this->Rotate(XMVectorSet(pos.x, pos.y, f, pos.w));
	};
	std::function<void(float)> collbacksx = [&](float f){
		auto pos = this->Scale();
		this->Scale(XMVectorSet(f, pos.y, pos.z, pos.w));
	};
	std::function<void(float)> collbacksy = [&](float f){
		auto pos = this->Scale();
		this->Scale(XMVectorSet(pos.x, f, pos.z, pos.w));
	};
	std::function<void(float)> collbacksz = [&](float f){
		auto pos = this->Scale();
		this->Scale(XMVectorSet(pos.x, pos.y, f, pos.w));
	};
	auto data = Window::CreateInspector();
	Window::AddInspector(new InspectorLabelDataSet("Transform"), data);
	Window::AddInspector(new InspectorVector3DataSet("Position", &mPosition.x, collbackpx, &mPosition.y, collbackpy, &mPosition.z, collbackpz), data);
	Window::AddInspector(new InspectorVector3DataSet("Rotate", &mRotate.x, collbackrx, &mRotate.y, collbackry, &mRotate.z, collbackrz), data);
	Window::AddInspector(new InspectorVector3DataSet("Scale", &mScale.x, collbacksx, &mScale.y, collbacksy, &mScale.z, collbacksz), data);
	Window::ViewInspector(data);

	//Window::GetInspectorWindow()->AddLabel("Transform");
	//Window::GetInspectorWindow()->AddParam(&mPosition.x, &mFixMatrixFlag);
	//Window::GetInspectorWindow()->AddParam(&mPosition.y, &mFixMatrixFlag);
	//Window::GetInspectorWindow()->AddParam(&mPosition.z, &mFixMatrixFlag);
}

void TransformComponent::ExportData(File& f){
	ExportClassName(f);
	if (!f.Out(mScale.x)){
		return;
	}
	f.Out(mScale.y);
	f.Out(mScale.z);
	f.Out(mScale.w);

	f.Out(mRotate.x);
	f.Out(mRotate.y);
	f.Out(mRotate.z);
	f.Out(mRotate.w);

	f.Out(mPosition.x);
	f.Out(mPosition.y);
	f.Out(mPosition.z);
	f.Out(mPosition.w);
}
void TransformComponent::ImportData(File& f){
	if (!f.In(&mScale.x))
		return;
	f.In(&mScale.y);
	f.In(&mScale.z);
	f.In(&mScale.w);

	f.In(&mRotate.x);
	f.In(&mRotate.y);
	f.In(&mRotate.z);
	f.In(&mRotate.w);

	f.In(&mPosition.x);
	f.In(&mPosition.y);
	f.In(&mPosition.z);
	f.In(&mPosition.w);
	mFixMatrixFlag = false;
}

void TransformComponent::UpdatePhysX(){
	//Physicsと競合するためポジションを動かした場合コチラを優先
	auto c = gameObject->GetComponent<PhysXComponent>();
	if (c)
		c->SetChengeTransformFlag();
}


void TransformComponent::AddForce(XMVECTOR& force){
	auto c = gameObject->GetComponent<PhysXComponent>();
	if (c)
		c->AddForce(force);
}
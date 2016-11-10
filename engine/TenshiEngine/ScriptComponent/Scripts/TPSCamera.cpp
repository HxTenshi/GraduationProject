#include "TPSCamera.h"
#include "h_standard.h"
#include "h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

TPSCamera::TPSCamera(){
	mDistance = 5.0f;
	mUp = 1.0f;
	mTarget = NULL;
	mRotate = XMFLOAT2(0, 0);
	m_SpringVelocity = XMVectorZero();
}

//生成時に呼ばれます（エディター中も呼ばれます）
void TPSCamera::Initialize(){

	m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TPSCamera::Start(){

	Hx::System()->LockCursorPositionToWindowCenter(true);
}

//毎フレーム呼ばれます
void TPSCamera::Update(){

	if (Input::Down(KeyCode::Key_ESCAPE)) {
		Hx::System()->LockCursorPositionToWindowCenter(false);
	}
	if (Input::Down(MouseCode::Left)) {
		Hx::System()->LockCursorPositionToWindowCenter(true);
	}

	if (!m_CharacterControllerComponent) {
		m_CharacterControllerComponent = gameObject->GetComponent<CharacterControllerComponent>();
	}

	if (mTarget){
		auto currentPos = gameObject->mTransform->WorldPosition();
		auto pos = mTarget->mTransform->WorldPosition();
		auto back = -gameObject->mTransform->Forward() * mDistance;
		auto up = gameObject->mTransform->Up() * mUp;
		auto left = gameObject->mTransform->Left() * mLeft;
		auto campos = pos + back + up + left;

		if (m_CharacterControllerComponent) {
			pos += -gameObject->mTransform->Forward()* 1.0f;
			m_CharacterControllerComponent->Teleport(pos);
			campos -= pos;
			m_CharacterControllerComponent->Move(campos);

			campos = gameObject->mTransform->WorldPosition();
			auto p = spring(currentPos, campos);
			gameObject->mTransform->WorldPosition(p);
		}
		else {
			auto p = spring(currentPos, campos);
			gameObject->mTransform->WorldPosition(p);
		}


		//if (XMVector3Equal(pos, campos)){

		//	pos += mTarget->mTransform->Forward();
		//	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(campos, pos, XMVectorSet(0, 1, 0, 1)));
		//	auto q = XMQuaternionRotationMatrix(rotate);
		//	gameObject->mTransform->WorldQuaternion(q);
		//}
		//else{
		//	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(campos, pos, XMVectorSet(0, 1, 0, 1)));
		//	auto q = XMQuaternionRotationMatrix(rotate);
		//	gameObject->mTransform->WorldQuaternion(q);
		//}
	}

	if (mLookTarget) {
		auto eye = mTarget->mTransform->WorldPosition();
		auto at = mLookTarget->mTransform->WorldPosition();
		auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(eye, at, XMVectorSet(0, 1, 0, 1)));
		auto q = XMQuaternionRotationMatrix(rotate);
		gameObject->mTransform->WorldQuaternion(q);
	}
	else {

		int mx, my;
		Input::MousePosition(&mx, &my);
		auto p = Hx::System()->GetLockCursorPosition();
		float _mx = mx - p.x;
		float _my = my - p.y;
		if (abs(_mx) < 1.1f)_mx = 0.0f;
		if (abs(_my) < 1.1f)_my = 0.0f;
		mRotate.y += _mx / 200.0f;
		mRotate.x += _my / 200.0f;


		float MAX = XM_PI / 2 - 0.1f;
		mRotate.x = min(max(mRotate.x, -MAX), MAX);

		auto qx = XMQuaternionRotationRollPitchYaw(mRotate.x, 0, 0);
		auto qy = XMQuaternionRotationRollPitchYaw(0, mRotate.y, 0);
		qx = XMQuaternionMultiply(qx, qy);
		qx = XMQuaternionNormalize(qx);

		gameObject->mTransform->WorldQuaternion(qx);

	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void TPSCamera::Finish(){

}

//コライダーとのヒット時に呼ばれます
void TPSCamera::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void TPSCamera::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void TPSCamera::OnCollideExit(GameObject target){
	(void)target;
}

XMVECTOR TPSCamera::spring(XMVECTOR position, const XMVECTOR& desiredPosition)
{
	float time = Hx::DeltaTime()->GetDeltaTime();
	// バネの力を計算
	auto stretch = position - desiredPosition;
	auto force = -m_SpringStiffness * stretch - m_SpringDamping * m_SpringVelocity;

	float mass = 1.0f;
	// 加速度の分を追加
	auto acceleration = force / mass;
	m_SpringVelocity += acceleration * time;

	// 速度の分を追加
	position += m_SpringVelocity * time;
	position.w = 1.0f;
	return position;
}

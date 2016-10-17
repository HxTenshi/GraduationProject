#include "TPSCamera.h"
#include "h_standard.h"

TPSCamera::TPSCamera(){
	mDistance = 5.0f;
	mUp = 1.0f;
	mTarget = NULL;
	mRotate = XMFLOAT2(0, 0);
}

//生成時に呼ばれます（エディター中も呼ばれます）
void TPSCamera::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void TPSCamera::Start(){

	Hx::System()->LockCursorPositionToWindowCenter(true);
}

//毎フレーム呼ばれます
void TPSCamera::Update(){

	if (Input::Down(KeyCoord::Key_ESCAPE)) {
		Hx::System()->LockCursorPositionToWindowCenter(false);
	}
	if (Input::Down(MouseCoord::Left)) {
		Hx::System()->LockCursorPositionToWindowCenter(true);
	}

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

	if (mTarget){
		auto pos = mTarget->mTransform->WorldPosition();
		auto back = -gameObject->mTransform->Forward() * mDistance;
		auto up = gameObject->mTransform->Up() * mUp;
		up += mUp * up;
		auto campos = pos + back + up;
		gameObject->mTransform->WorldPosition(campos);
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
#include "TPSCamera.h"
#include "h_standard.h"
#include "h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

TPSCamera::TPSCamera(){
	mTarget = NULL;
	mSaveEnemy = NULL;
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

	auto currentPos = gameObject->mTransform->WorldPosition();
	auto pos = mTarget->mTransform->WorldPosition();
	auto back = -gameObject->mTransform->Forward() * mDistance;
	auto up = XMVectorSet(0.0f,1.0f,0.0f,1.0f) * mUp;
	float l = 1.0f;
	if (mLookTarget) {
		l = XMVector3Length(mTarget->mTransform->WorldPosition() - mLookTarget->mTransform->WorldPosition()).x;
		l *= 0.1f;
		l = min(l, 0.1f);

	}
	auto left = gameObject->mTransform->Left() * mLeft * l;
	auto campos = pos + back + up + left;

	gameObject->mTransform->WorldPosition(campos);
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
		auto up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f) * mUp;
		float l = 1.0f;
		if (mLookTarget) {
			l = XMVector3Length(mTarget->mTransform->WorldPosition()-mLookTarget->mTransform->WorldPosition()).x;
			l *= 0.1f;
			l = min(l, 0.1f);

		}
		auto left = gameObject->mTransform->Left() * mLeft * l;
		auto campos = pos + back + up + left;

		if (m_CharacterControllerComponent) {
			//pos += gameObject->mTransform->Forward()* 1.0f;
			campos -= pos;
			pos.y += 1.0f;

			//campos = gameObject->mTransform->WorldPosition();
			//campos = spring(currentPos, campos);

			m_CharacterControllerComponent->Teleport(pos);
			m_CharacterControllerComponent->Move(campos);

			campos = gameObject->mTransform->WorldPosition();
			auto p = spring(currentPos, campos);
			//m_CharacterControllerComponent->Teleport(p);
			auto nan = XMVectorIsNaN(p);
			if (!nan.x&&!nan.y&&!nan.z&&!nan.w)
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
		auto wq = gameObject->mTransform->WorldQuaternion();
		auto wf = gameObject->mTransform->Forward();
		auto eye = gameObject->mTransform->WorldPosition();
		eye -= XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f) * mUp;

		auto at = mLookTarget->mTransform->WorldPosition();
		auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(eye, at, XMVectorSet(0, 1, 0, 1)));
		auto q = XMQuaternionRotationMatrix(rotate);
		gameObject->mTransform->WorldQuaternion(q);


		auto f = gameObject->mTransform->Forward();
		float angle = acos(XMVector3Dot(wf, f).x);
		float limit = m_RotateLimit / 180.0f * XM_PI * Hx::DeltaTime()->GetDeltaTime();
		float t = 1.0f;
		if (limit < angle) {
			if(angle)
			t = limit / angle;
		}
		q = XMQuaternionSlerp(wq, q, t);
		if(!XMQuaternionIsNaN(q))
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

		_my = Input::Down(KeyCode::Key_UP) != 0 ? -1 : 0;
		_my = Input::Down(KeyCode::Key_DOWN) != 0 ? 1 : _my;
		_mx = Input::Down(KeyCode::Key_RIGHT) != 0 ? 1 : 0;
		_mx = Input::Down(KeyCode::Key_LEFT) != 0 ? -1 : _mx;
		_my *= 15.0f;
		_mx *= 15.0f;

		auto ls = Input::Analog(PAD_X_Velo2Code::Velo2_RStick);
		if (XMVector2Length(ls).x > 0.05f) {
			_mx = ls.x * 15.0f;
			_my = ls.y * -15.0f;
		}

		mRotate.y += _mx / 15.0f * Hx::DeltaTime()->GetDeltaTime() * m_CameraSpeed;
		mRotate.x += _my / 15.0f * Hx::DeltaTime()->GetDeltaTime() * m_CameraSpeed;


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
	float time = Hx::DeltaTime()->GetNoScaleDeltaTime();
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

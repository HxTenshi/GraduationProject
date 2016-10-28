#include "Sandbag.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Sandbag::Initialize(){
	mGravity = XMVectorSet(0, -9.81f, 0, 1);
	angle = 0.0f;
	maxAngle = 0.0f;
	subAngle = 0.0f;
	walkReturnFlag = false;
	changeVec = false;
	concussion = 0.0f;
	damageFlag = false;
	moveCount = 0;
	moveCountUp = true;

	am = ActionMode::TRACKINGMODE;
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void Sandbag::Start(){
}

//毎フレーム呼ばれます
void Sandbag::Update(){
	vec = XMVectorZero();
	if (damageFlag) {
		concussion += Hx::DeltaTime()->GetDeltaTime();
		if (concussion > concussionTime) {
			damageFlag = false;
			concussion = 0.0f;
		}
		vec += XMVectorSet(0, jumpPower, 0, 0);
	}

	AnimLerp();

	if (am == ActionMode::TRACKINGMODE){
		TrackingMode();
	}
	else if (am == ActionMode::BATTLEMODE){
		BattleMode();
	}

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	//重力
	if (!cc->IsGround()) {
		vec += mGravity;
	}

	cc->Move(vec  * Hx::DeltaTime()->GetDeltaTime());
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void Sandbag::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Sandbag::OnCollideBegin(GameObject target) {
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Sandbag::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Sandbag::OnCollideExit(GameObject target){
	(void)target;
}

void Sandbag::SetHitWall(bool hitWall)
{
	if (am == ActionMode::TRACKINGMODE){
		walkReturnFlag = false;
		if (!changeVec) {
			changeVec = true;
			int random = rand();
			maxAngle = rand() % 90;
			if (random % 2 == 0) {
				maxAngle *= -1;
			}
		}
	}
}

void Sandbag::Damage(int damage)
{
	if (!damageFlag){
		if (bp.battleAction != BATTLEACTION::GUARD){
			hp -= damage;
			damageFlag = true;
		}
		else{
			bp.battleAction = BATTLEACTION::ESCAPE;
			bp.guardTimeCount = 0.0f; 
			bp.actionDecideTime = 0.0f;
		}
	}
}

void Sandbag::TrackingMode(){
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();

	auto forward = XMVector3Normalize(gameObject->mTransform->Forward());
	auto playerVec = playerPos - gameObject->mTransform->WorldPosition();
	forward.y = 0.0f;
	playerVec.y = 0.0f;
	float view = acos(XMVector3Dot(forward, XMVector3Normalize(playerVec)).x);
	if (XMVector3Length(forward - XMVector3Normalize(playerVec)).x < 0.01f)view = 0.0f;

	if ((XMVector3Length(playerVec).x < trackingRange && view / 3.14f * 180.0f < trackingAngle)) {
		am = ActionMode::BATTLEMODE;
		bp.battleAction = BATTLEACTION::TRACKING;
		BattleMode();
	}
	else{
		auto navi = gameObject->GetComponent<NaviMeshComponent>();
		if (!navi)return;
		if (navi->IsMoveEnd()) {
			Hx::Debug()->Log("Think");
			auto movePointList = movePoints->mTransform->Children();
			int maxMoveCount = movePointList.size();
			if (maxMoveCount == 0)return;
			maxMoveCount--;
			int i = 0;
			GameObject movePoint = *movePointList.begin();
			for (auto itr = movePointList.begin(); i <= moveCount;++itr, i++) {
				movePoint = *itr;
			}

			if (moveCountUp) {
				moveCount++;
				if (moveCount == maxMoveCount) {
					moveCountUp = false;
				}
			}
			else {
				moveCount--;
				if (moveCount == 0) {
					moveCountUp = true;
				}
			}
			navi->RootCreate(gameObject,movePoint);
		}

		AnimChange(1, 5.0f);

		//navi->GetPosition();
	//	if(XMVector3Length(navi->GetPosition() - gameObject->mTransform->WorldPosition()).x > speed / 30.0f)
		navi->Move(speed * Hx::DeltaTime()->GetDeltaTime());

		auto cc = gameObject->GetComponent<CharacterControllerComponent>();
		if (!cc)return;

		auto forward = XMVector3Normalize(gameObject->mTransform->Forward());
		auto naviVec = XMVector3Normalize(navi->GetPosition() - gameObject->mTransform->WorldPosition());
		forward.y = 0;
		naviVec.y = 0;
		auto cross = XMVector3Normalize(XMVector3Cross(forward, naviVec));
		auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		view = acos(XMVector3Dot(forward, naviVec).x);
		if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
		cc->Move(forward * speed * Hx::DeltaTime()->GetDeltaTime());
		////壁に当たって修正したがまだ壁に当たっているか調べる
		//if (walkReturnFlag) {
		//	changeVec = false;
		//	subAngle = 0.0f;
		//}
		//
		//if (!changeVec) {
		//	AnimChange(1, 5.0f);
		//	auto forward = gameObject->mTransform->Forward();
		//	forward = XMVector3Normalize(forward);
		//	auto pos = gameObject->mTransform->Position();
		//	vec += forward * speed;
		//}
		//else {
		//	AnimChange(0, 5.0f);
		//	float rotateVec = rotateSpeed * Hx::DeltaTime()->GetDeltaTime();
		//	subAngle += rotateVec;
		//	if (maxAngle < 0.0f) {
		//		angle -= rotateVec;
		//	}
		//	else {
		//		angle += rotateVec;
		//	}
		//	if (abs(subAngle) > abs(maxAngle)) {
		//		walkReturnFlag = true;
		//	}
		//}
		//Angleのクランプ
		//if (angle > 360)angle -= 360.0f;
		//else if (angle < 0)angle += 360.0f;
		//
		////値のSet
		//gameObject->mTransform->Rotate(XMVectorSet(0, angle * 3.14f / 180.0f, 0, 0));
	}
}

void Sandbag::BattleMode(){
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();

	auto forward = XMVector3Normalize(gameObject->mTransform->Forward());
	auto playerVec = playerPos - gameObject->mTransform->WorldPosition();
	forward.y = 0.0f;
	playerVec.y = 0.0f;
	float view = acos(XMVector3Dot(forward, XMVector3Normalize(playerVec)).x);
	if (XMVector3Length(forward - XMVector3Normalize(playerVec)).x < 0.01f)view = 0.0f;

	if (XMVector3Length(playerVec).x > trackingRange) {
		am = ActionMode::TRACKINGMODE;

		bp.battleAction = BATTLEACTION::TRACKING;
		bp.actionDecideTime = 0.0f;
		bp.beforeActionDecideTime = 0;
		bp.guardTimeCount = 0;

		auto navi = gameObject->GetComponent<NaviMeshComponent>();
		if (!navi)return;
		auto movePointList = movePoints->mTransform->Children();
		int maxMoveCount = movePointList.size();
		if (maxMoveCount == 0)return;
		int i = 0;
		float movePointLenMin;
		for (auto itr = movePointList.begin(); itr == movePointList.end();++i, ++itr) {
			auto movePointLen = XMVector3Length(gameObject->mTransform->WorldPosition() - itr->Get()->mTransform->WorldPosition()).x;
			if (i == 0 || movePointLenMin > movePointLen) {
				movePointLenMin = movePointLen;
				moveCount = i;
			}
		}
		GameObject movePoint = *movePointList.begin();
		i = 0;
		for (auto itr = movePointList.begin(); i <= moveCount; ++itr, ++i) {
			movePoint = *itr;
		}
		navi->RootCreate(gameObject, movePoint);
		moveCountUp = true;

		angle = gameObject->mTransform->Rotate().y / 3.14f * 180.0f;
		TrackingMode();
	}
	else{
		if (XMVector3Length(playerVec).x < battleRange + 0.3f){
			if (bp.battleAction == BATTLEACTION::TRACKING)
			bp.battleAction = BATTLEACTION::NONE;
		}
		else{
			if (bp.battleAction == BATTLEACTION::NONE)
			bp.battleAction = BATTLEACTION::TRACKING;
		}
		bp.actionDecideTime += Hx::DeltaTime()->GetDeltaTime();

		if (bp.beforeActionDecideTime != (int)bp.actionDecideTime){
			if (bp.battleAction == BATTLEACTION::NONE){
				if (rand() % 10 >= 5){
					bp.battleAction = BATTLEACTION::ATTACK;
				}
				else{
					bp.battleAction = BATTLEACTION::GUARD;
				}
			}

			bp.beforeActionDecideTime = (int)bp.actionDecideTime;
		}

		if (bp.battleAction == BATTLEACTION::TRACKING || bp.battleAction == BATTLEACTION::NONE || bp.battleAction == BATTLEACTION::GUARD){
			if (XMVector3Length(playerVec).x > battleRange){
				vec += forward * trackingSpeed;
			}
			else{
				vec -= forward * trackingSpeed;
			}
		}
		if (bp.battleAction != BATTLEACTION::ATTACK || bp.battleAction != BATTLEACTION::ESCAPE){
			auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
			auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
			if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
			auto qua = gameObject->mTransform->Quaternion();
			gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
		}


		if (bp.battleAction == BATTLEACTION::TRACKING){
			//Hx::Debug()->Log("TRACKING");
			AnimChange(1, 5.0f);
		}
		else if (bp.battleAction == BATTLEACTION::ATTACK){
			AnimChange(3, 5.0f);
			if (GetNowAnimTime() > attackAnimTotalTime){
				bp.battleAction = BATTLEACTION::TRACKING;
				bp.actionDecideTime = 0.0f;
			};
			//Hx::Debug()->Log("ATTACK");

		}
		else if (bp.battleAction == BATTLEACTION::GUARD){
			AnimChange(4, 5.0f);
			bp.guardTimeCount += Hx::DeltaTime()->GetDeltaTime();
			if (bp.guardTimeCount > 3.0f){
				bp.guardTimeCount = 0.0f;
				bp.actionDecideTime = 0.0f;
				bp.battleAction = BATTLEACTION::TRACKING;
			}
	//		Hx::Debug()->Log("GUARD");

		}
		else if (bp.battleAction == BATTLEACTION::ESCAPE){
			AnimChange(2, 5.0f);
			if (GetNowAnimTime() < 25){
				vec -= forward * 10.0f;
			}
			else if (GetNowAnimTime() > backStepAnimTotalTime){
				bp.battleAction = BATTLEACTION::TRACKING;
				bp.actionDecideTime = 0.0f;
			}
	//		Hx::Debug()->Log("ESCAPE");

		}
		else if (bp.battleAction == BATTLEACTION::NONE){
			AnimChange(1, 5.0f);
	//		Hx::Debug()->Log("NONE");
		}
	}
}

void Sandbag::AnimChange(int id, float speed,bool roop)
{
	if (id != animparam.beforeAnimId) {
		if (!animparam.animLerpFlag) {
			animparam.nowAnimId = id;
			animparam.afterAnimId = id;
			animparam.animLerpFlag = true;
			animparam.lerpSpeed = speed;
			animparam.afterAnimLoop = roop;
			
			AnimeParam ap;
			if (!modelObject)return;
			auto anim = modelObject->GetComponent<AnimationComponent>();
			if (!anim)return;
			ap = anim->GetAnimetionParam(animparam.afterAnimId);
			ap.mTime = 0.0f;
			anim->SetAnimetionParam(animparam.afterAnimId, ap);
		}
	}
}

void Sandbag::AnimLerp()
{
	if (animparam.animLerpFlag) {
		if (!modelObject)return;
		auto anim = modelObject->GetComponent<AnimationComponent>();
		if (!anim)return;

		animparam.nowLerpTime += animparam.lerpSpeed * Hx::DeltaTime()->GetDeltaTime();

		bool endFlag = false;
		if (animparam.nowLerpTime >= 1.0f) {
			animparam.nowLerpTime = 1.0f;
			endFlag = true;
		}

		AnimeParam ap;
		ap = anim->GetAnimetionParam(animparam.beforeAnimId);
		ap.mWeight = 1.0f - animparam.nowLerpTime;
		ap.mLoop = animparam.beforeAnimLoop;
		anim->SetAnimetionParam(animparam.beforeAnimId, ap);

		ap = anim->GetAnimetionParam(animparam.afterAnimId);
		ap.mWeight = animparam.nowLerpTime;
		ap.mLoop = animparam.afterAnimLoop;
		anim->SetAnimetionParam(animparam.afterAnimId, ap);

		if (endFlag) {
			animparam.beforeAnimId = animparam.afterAnimId;
			animparam.nowLerpTime = 0.0f;
			animparam.animLerpFlag = false;
			animparam.beforeAnimLoop = animparam.afterAnimLoop;
		}
	}
}

float Sandbag::GetNowAnimTime(){
	if (!modelObject)return 0;
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return 0;

	AnimeParam ap;
	ap = anim->GetAnimetionParam(animparam.nowAnimId);
	return ap.mTime;
}
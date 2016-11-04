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

	actionModeID = ACTIONMODE::TRACKINGMODE;
	battleActionID = BATTLEACTION::CONFRONTACTION;

	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&Sandbag::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&Sandbag::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&Sandbag::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&Sandbag::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&Sandbag::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&Sandbag::BattleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&Sandbag::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&Sandbag::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&Sandbag::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::APPROACHACTION] = std::bind(&Sandbag::ApproachModeInitilize, this);
	battleActionUpdate[BATTLEACTION::APPROACHACTION] = std::bind(&Sandbag::ApproachModeUpdate, this);
	battleActionFinalize[BATTLEACTION::APPROACHACTION] = std::bind(&Sandbag::ApproachModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKACTION] = std::bind(&Sandbag::AttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKACTION] = std::bind(&Sandbag::AttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKACTION] = std::bind(&Sandbag::AttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Sandbag::JumpAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Sandbag::JumpAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Sandbag::JumpAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::GUARDACTION] = std::bind(&Sandbag::GuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::GUARDACTION] = std::bind(&Sandbag::GuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::GUARDACTION] = std::bind(&Sandbag::GuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Sandbag::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&Sandbag::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Sandbag::BackStepModeFinalize, this);

	actionModeInitilize[actionModeID]();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）s
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

	if (Input::Trigger(KeyCode::Key_1))Damage(1.0f);
	AnimLerp();

	actionModeUpdate[actionModeID]();

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

/***********************************************鼻が壁に当たった時の処理***************************************************/
void Sandbag::SetHitWall(bool hitWall)
{
	if (actionModeID == ACTIONMODE::TRACKINGMODE){
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


/****************************************************ダメージの処理********************************************************/
void Sandbag::Damage(int damage)
{
	if (!damageFlag){
		if (bp.battleAction != BATTLEACTION::GUARDACTION){
			hp -= damage;
			damageFlag = true;
		}
		else{
			bp.battleAction = BATTLEACTION::BACKSTEPACTION;
			bp.guardTimeCount = 0.0f; 
			bp.actionDecideTime = 0.0f;
		}
	}
}

/**************************************************アニメーションの処理****************************************************/
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

/****************************************************捜索時の処理**********************************************************/
void Sandbag::TrackingModeInitilize()
{
	Hx::Debug()->Log("TrackingInitilize");

	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;
	auto movePointList = movePoints->mTransform->Children();
	int maxMoveCount = movePointList.size();
	if (maxMoveCount == 0)return;
	int i = 0;
	float movePointLenMin;
	for (auto itr : movePointList) {
		auto movePointLen = XMVector3Length(gameObject->mTransform->WorldPosition() - itr->mTransform->WorldPosition()).x;
		if (i == 0 || movePointLenMin > movePointLen) {
			movePointLenMin = movePointLen;
			moveCount = i;
		}
		i++;
	}
	GameObject movePoint = *movePointList.begin();
	i = 0;
	for (auto itr = movePointList.begin(); i <= moveCount; ++itr, ++i) {
		movePoint = *itr;
	}
	navi->RootCreate(gameObject, movePoint);
	moveCountUp = true;

	angle = gameObject->mTransform->Rotate().y / 3.14f * 180.0f;
}

void Sandbag::TrackingModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();

	forward = XMVector3Normalize(gameObject->mTransform->Forward());
	playerVec = playerPos - gameObject->mTransform->WorldPosition();
	forward.y = 0.0f;
	playerVec.y = 0.0f;
	view = acos(XMVector3Dot(forward, XMVector3Normalize(playerVec)).x);
	if (XMVector3Length(forward - XMVector3Normalize(playerVec)).x < 0.01f)view = 0.0f;

	if ((XMVector3Length(playerVec).x < trackingRange && view / 3.14f * 180.0f < trackingAngle)) {
		actionModeFinalize[actionModeID]();
		return;
	}
	
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;
	if (navi->IsMoveEnd()) {
		auto movePointList = movePoints->mTransform->Children();
		int maxMoveCount = movePointList.size();
		if (maxMoveCount == 0)return;
		maxMoveCount--;
		int i = 0;
		GameObject movePoint = *movePointList.begin();
		for (auto itr = movePointList.begin(); i <= moveCount; ++itr, i++) {
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
		navi->RootCreate(gameObject, movePoint);
	}

	navi->Move(speed * Hx::DeltaTime()->GetDeltaTime());

	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	auto naviVec = XMVector3Normalize(navi->GetPosition() - gameObject->mTransform->WorldPosition());
	forward.y = 0;
	naviVec.y = 0;
	auto cross = XMVector3Normalize(XMVector3Cross(forward, naviVec));
	auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	view = acos(XMVector3Dot(forward, naviVec).x);
	if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	
	vec += forward * speed;
}

void Sandbag::TrackingModeFinalize()
{
	Hx::Debug()->Log("TrackingFinalize");
	actionModeID = ACTIONMODE::BATTLEMODE;
	bp.battleAction = BATTLEACTION::CONFRONTACTION;
	actionModeInitilize[actionModeID]();
	actionModeUpdate[actionModeID]();
}

/****************************************************戦闘時の処理**********************************************************/
void Sandbag::BattleModeInitilize()
{
	Hx::Debug()->Log("BattleInitilize");
	battleActionID = BATTLEACTION::CONFRONTACTION;
	battleActionInitilize[battleActionID]();
}

void Sandbag::BattleModeUpdate()
{
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();

	forward = XMVector3Normalize(gameObject->mTransform->Forward());
	playerVec = playerPos - gameObject->mTransform->WorldPosition();
	forward.y = 0.0f;
	playerVec.y = 0.0f;
	view = acos(XMVector3Dot(forward, XMVector3Normalize(playerVec)).x);
	if (XMVector3Length(forward - XMVector3Normalize(playerVec)).x < 0.01f)view = 0.0f;

	if (XMVector3Length(playerVec).x > trackingRange) {
		actionModeFinalize[actionModeID]();
		return;
	}
	
	//bp.actionDecideTime += Hx::DeltaTime()->GetDeltaTime();
	//
	//if (bp.beforeActionDecideTime != (int)bp.actionDecideTime) {
	//	if (bp.battleAction == BATTLEACTION::CONFRONTACTION) {
	//		if (rand() % 10 >= 5) {
	//			bp.battleAction = BATTLEACTION::ATTACKACTION;
	//		}
	//		else {
	//			bp.battleAction = BATTLEACTION::GUARDACTION;
	//		}
	//	}
	//
	//	bp.beforeActionDecideTime = (int)bp.actionDecideTime;
	//}

	

	battleActionUpdate[battleActionID]();
}

void Sandbag::BattleModeFinalize()
{
	Hx::Debug()->Log("BattleFinalize");
	actionModeID = ACTIONMODE::TRACKINGMODE;
	bp.battleAction = BATTLEACTION::CONFRONTACTION;
	bp.actionDecideTime = 0.0f;
	bp.beforeActionDecideTime = 0;
	bp.guardTimeCount = 0;

	actionModeInitilize[actionModeID]();
	actionModeUpdate[actionModeID]();
}

void Sandbag::ConfrontModeInitilize()
{
	Hx::Debug()->Log("CONFRONT");
}

void Sandbag::ConfrontModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	if(XMVector3Length(playerVec).x <= battleRange){
		battleActionFinalize[BATTLEACTION::CONFRONTACTION]();
		vec -= forward * trackingSpeed;
	}
	else {
		vec += forward * trackingSpeed;
	}

	auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
	auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
}

void Sandbag::ConfrontModeFinalize()
{
	int randam = rand() % 100;
	if (randam >= 0) {
		battleActionID = BATTLEACTION::APPROACHACTION;
	}
	else if (randam > 70) {
		battleActionID = BATTLEACTION::BACKSTEPACTION;
	}
	else if (randam > 30) {
		battleActionID = BATTLEACTION::GUARDACTION;
	}
	else if (randam > 0) {
		battleActionID = BATTLEACTION::ATTACKACTION;
	}

	battleActionInitilize[battleActionID]();
	battleActionUpdate[battleActionID]();
}

void Sandbag::ApproachModeInitilize()
{
	Hx::Debug()->Log("APPROACH");
}

void Sandbag::ApproachModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();
	auto trans = XMMatrixTranslationFromVector(XMVector3Normalize(gameObject->mTransform->WorldPosition() - playerPos) * battleRange);
	auto rot = XMMatrixRotationY(speed * Hx::DeltaTime()->GetDeltaTime());
	auto pos = XMMatrixMultiply(XMMatrixMultiply(trans, rot),XMMatrixTranslationFromVector(playerPos));
	
	vec += XMVector3Normalize(pos.r[3] - gameObject->mTransform->WorldPosition()) * speed;
	
	//if (XMVector3Length(playerVec).x <= battleRange) {
	//	battleActionFinalize[BATTLEACTION::APPROACHACTION]();
	//	vec -= forward * trackingSpeed;
	//}
	//else {
	//	vec += forward * trackingSpeed;
	//}

	auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
	auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
}

void Sandbag::ApproachModeFinalize()
{
}

void Sandbag::AttackModeInitilize()
{
	Hx::Debug()->Log("ATTACK");
}

void Sandbag::AttackModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	AnimChange(ANIM_ID::ANIM_ATTACK_LONGITUDINAL, 5.0f, false);
	if (anim->IsAnimationEnd(animparam.beforeAnimId)) {
		bp.battleAction = BATTLEACTION::CONFRONTACTION;
		bp.actionDecideTime = 0.0f;
	};
}

void Sandbag::AttackModeFinalize()
{
}

void Sandbag::JumpAttackModeInitilize()
{
}

void Sandbag::JumpAttackModeUpdate()
{
}

void Sandbag::JumpAttackModeFinalize()
{
}

void Sandbag::GuardModeInitilize()
{
	Hx::Debug()->Log("GUARD");
}

void Sandbag::GuardModeUpdate()
{
	if (XMVector3Length(playerVec).x > battleRange) {
		vec += forward * trackingSpeed;
	}
	else {
		vec -= forward * trackingSpeed;
	}

	auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
	auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));

	AnimChange(ANIM_ID::ANIM_GUARD, 5.0f);
	bp.guardTimeCount += Hx::DeltaTime()->GetDeltaTime();
	if (bp.guardTimeCount > 3.0f) {
		bp.guardTimeCount = 0.0f;
		bp.actionDecideTime = 0.0f;
		bp.battleAction = BATTLEACTION::CONFRONTACTION;
	}
}

void Sandbag::GuardModeFinalize()
{
}

void Sandbag::BackStepModeInitilize()
{
	Hx::Debug()->Log("BACKSTEP");
}

void Sandbag::BackStepModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	AnimChange(ANIM_ID::ANIM_BACKSTEP, 5.0f, false);
	if (GetNowAnimTime() < 25) {
		vec -= forward * 10.0f;
	}
	else if (anim->IsAnimationEnd(animparam.beforeAnimId)) {
		bp.battleAction = BATTLEACTION::CONFRONTACTION;
		bp.actionDecideTime = 0.0f;
	}
}

void Sandbag::BackStepModeFinalize()
{
}
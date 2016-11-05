#include "Sandbag.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Sandbag::Initialize(){
	mGravity = XMVectorSet(0, -9.81f, 0, 1);
	moveCount = 0;
	moveCountUp = true;

	actionModeID = ACTIONMODE::TRACKINGMODE;
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;

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

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Sandbag::AttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Sandbag::AttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Sandbag::AttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Sandbag::JumpAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Sandbag::JumpAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Sandbag::JumpAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::GUARDACTION] = std::bind(&Sandbag::GuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::GUARDACTION] = std::bind(&Sandbag::GuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::GUARDACTION] = std::bind(&Sandbag::GuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Sandbag::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&Sandbag::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Sandbag::BackStepModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&Sandbag::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&Sandbag::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&Sandbag::WinceModeFinalize, this);

	actionModeInitilize[actionModeID]();
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）s
void Sandbag::Start(){
}

//毎フレーム呼ばれます
void Sandbag::Update(){
	vec = XMVectorZero();

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

///***********************************************鼻が壁に当たった時の処理***************************************************/
//void Sandbag::SetHitWall(bool hitWall)
//{
//	if (actionModeID == ACTIONMODE::TRACKINGMODE){
//		walkReturnFlag = false;
//		if (!changeVec) {
//			changeVec = true;
//			int random = rand();
//			maxAngle = rand() % 90;
//			if (random % 2 == 0) {
//				maxAngle *= -1;
//			}
//		}
//	}
//}


/****************************************************ダメージの処理********************************************************/
void Sandbag::Damage(int damage)
{
	hp -= damage;
	ChangeBattleAction(BATTLEACTION::WINCEACTION);
}

/**************************************************アニメーションの処理****************************************************/
void Sandbag::AnimChange(int id, float speed, bool roop)
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

/********************************************アクションの変更、戦闘時の変更************************************************/
void Sandbag::ChangeActionMode(ACTIONMODE nextActionMode){
	actionModeID = nextActionMode;
	actionModeInitilize[actionModeID]();
	actionModeUpdate[actionModeID]();
}

void Sandbag::ChangeBattleAction(BATTLEACTION nextBattleAction){
	battleModeParam.beforeBattleActionID = battleModeParam.battleActionID;
	battleModeParam.battleActionID = nextBattleAction;
	battleActionInitilize[battleModeParam.battleActionID]();
	battleActionUpdate[battleModeParam.battleActionID]();
}


void Sandbag::ChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability){
	if (XMVector3Length(playerVec).x > offBattleRange && battleModeParam.battleActionID != BATTLEACTION::BACKSTEPACTION){
		ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
		return;
	}

	int randam = rand() % 100;

	int guardProbability_, approachProbability_, backstepProbability_, attackProbability_, jumpAttackProbability_;

	guardProbability_ = 100 - guardProbability;
	approachProbability_ = guardProbability_ - approachProbability;
	backstepProbability_ = approachProbability_ - backstepProbability;
	attackProbability_ = backstepProbability_ - attackProbability;
	jumpAttackProbability_ = attackProbability_ - jumpAttackProbability;

	if (randam > guardProbability_) {
		ChangeBattleAction(BATTLEACTION::GUARDACTION);
	}
	else if (randam > approachProbability_) {
		ChangeBattleAction(BATTLEACTION::APPROACHACTION);
	}
	else if (randam > backstepProbability_) {
		ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
	}
	else if (randam > attackProbability_){
		ChangeBattleAction(BATTLEACTION::ATTACKDOWNACTION);
	}
	else if (randam > jumpAttackProbability_){
		ChangeBattleAction(BATTLEACTION::JUMPATTACKACTION);
	}

}

/****************************************************捜索時の処理**********************************************************/
void Sandbag::TrackingModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("MODE	TRACKING");
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

	navi->Move(trackingSpeed * Hx::DeltaTime()->GetDeltaTime());

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

	vec += forward * trackingSpeed;
}

void Sandbag::TrackingModeFinalize()
{
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	ChangeActionMode(ACTIONMODE::BATTLEMODE);
}

/****************************************************戦闘時の処理**********************************************************/
void Sandbag::BattleModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("MODE	BATTLE");
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	battleActionInitilize[battleModeParam.battleActionID]();
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
	battleActionUpdate[battleModeParam.battleActionID]();
}

void Sandbag::BattleModeFinalize()
{
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	ChangeActionMode(ACTIONMODE::TRACKINGMODE);
}

void Sandbag::ConfrontModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("BATTLE	CONFRONT");
}

void Sandbag::ConfrontModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	if (XMVector3Length(playerVec).x <= onBattleRange){
		battleActionFinalize[BATTLEACTION::CONFRONTACTION]();
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
	ChangeBattleAction(30, 30, 20, 20, 0);
}

void Sandbag::ApproachModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("BATTLE	APPROACH");
	battleModeParam.count = 0.0f;
	battleModeParam.rotateVecPlus = !battleModeParam.rotateVecPlus;
}

void Sandbag::ApproachModeUpdate()
{
	AnimChange(ANIM_ID::ANIM_WALK_FORWARD, 5.0f);
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();
	auto trans = XMMatrixTranslationFromVector(XMVector3Normalize(gameObject->mTransform->WorldPosition() - playerPos) * onBattleRange);
	auto rot = XMMatrixRotationY(aproachRotateSpeed * Hx::DeltaTime()->GetDeltaTime() * (battleModeParam.rotateVecPlus == true ? 1.0f : -1.0f));
	auto pos = XMMatrixMultiply(XMMatrixMultiply(trans, rot), XMMatrixTranslationFromVector(playerPos));

	vec += XMVector3Normalize(pos.r[3] - gameObject->mTransform->WorldPosition()) * trackingSpeed;

	auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
	auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));

	battleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (battleModeParam.count > APPROACHTIME){
		battleActionFinalize[battleModeParam.battleActionID]();
	}
}

void Sandbag::ApproachModeFinalize()
{
	ChangeBattleAction(30, 0, 40, 30, 0);
}

void Sandbag::AttackModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("BATTLE	ATTACK");
}

void Sandbag::AttackModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f){
		auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
		auto trackingNowAngle = correctionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (view < correctionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	}

	AnimChange(ANIM_ID::ANIM_ATTACK_DOWN, 5.0f, false);
	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		battleActionFinalize[battleModeParam.battleActionID]();
	};
}

void Sandbag::AttackModeFinalize()
{
	ChangeBattleAction(40, 40, 20, 0, 0);
}

void Sandbag::JumpAttackModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("BATTLE	JUMPATTACK");
}

void Sandbag::JumpAttackModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	AnimChange(ANIM_ID::ANIM_JUMPSRASH, 5.0f, false);
	if (GetNowAnimTime() < 12.5f) {
		vec += forward * 20.0f;
	}
	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		battleActionFinalize[battleModeParam.battleActionID]();
	};
}

void Sandbag::JumpAttackModeFinalize()
{
	ChangeBattleAction(40, 20, 10, 30, 0);
	//ChangeBattleAction(0, 0, 100, 0, 0);
}

void Sandbag::GuardModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("BATTLE	GUARD");
	battleModeParam.count = 0.0f;
	if (rand() % 2 == 0){
		battleModeParam.rotateVecPlus = true;
	}
	else{
		battleModeParam.rotateVecPlus = false;
	}
}

void Sandbag::GuardModeUpdate()
{
	if (!player)return;
	XMVECTOR playerPos = player->mTransform->WorldPosition();
	auto trans = XMMatrixTranslationFromVector(XMVector3Normalize(gameObject->mTransform->WorldPosition() - playerPos) * onBattleRange);
	auto rot = XMMatrixRotationY(aproachRotateSpeed * Hx::DeltaTime()->GetDeltaTime() * (battleModeParam.rotateVecPlus == true ? 1.0f : -1.0f));
	auto pos = XMMatrixMultiply(XMMatrixMultiply(trans, rot), XMMatrixTranslationFromVector(playerPos));

	vec += XMVector3Normalize(pos.r[3] - gameObject->mTransform->WorldPosition()) * trackingSpeed;

	auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
	auto trackingNowAngle = trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
	if (view < trackingRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
	auto qua = gameObject->mTransform->Quaternion();
	gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));

	AnimChange(ANIM_ID::ANIM_GUARD, 5.0f);

	battleModeParam.count += Hx::DeltaTime()->GetDeltaTime();
	if (battleModeParam.count > APPROACHTIME){
		battleActionFinalize[battleModeParam.battleActionID]();
	}
}

void Sandbag::GuardModeFinalize()
{
	ChangeBattleAction(20, 30, 20, 30, 0);
}

void Sandbag::BackStepModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("BATTLE	BACKSTEP");
}

void Sandbag::BackStepModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (GetNowAnimTime() < 1.0f){
		auto cross = XMVector3Normalize(XMVector3Cross(forward, XMVector3Normalize(playerVec)));
		auto trackingNowAngle = correctionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime();
		if (view < correctionRotateSpeed * 3.14f / 180.0f * Hx::DeltaTime()->GetDeltaTime())trackingNowAngle = view;
		auto qua = gameObject->mTransform->Quaternion();
		gameObject->mTransform->WorldQuaternion(XMQuaternionMultiply(qua, XMQuaternionRotationAxis(cross, trackingNowAngle)));
	}

	AnimChange(ANIM_ID::ANIM_BACKSTEP, 5.0f, false);
	if (GetNowAnimTime() < 12.5f) {
		vec -= forward * 20.0f;
	}
	else if (GetNowAnimTime() < 20.0f) {
		battleActionFinalize[battleModeParam.battleActionID]();
	}
}

void Sandbag::BackStepModeFinalize()
{
	ChangeBattleAction(30, 0, 0, 0, 70);
}

void Sandbag::WinceModeInitilize(){
	if (drawLog)
		Hx::Debug()->Log("BATTLE	WINCE");
}

void Sandbag::WinceModeUpdate(){
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	AnimChange(ANIM_ID::ANIM_ATTACK_SIDE, 5.0f, false);
	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		battleActionFinalize[battleModeParam.battleActionID]();
	};
}

void Sandbag::WinceModeFinalize(){
	ChangeBattleAction(40, 0, 40, 20, 0);
}

void Sandbag::HitInGuardModeInitilize(){

}

void Sandbag::HitInGuardModeUpdate(){

}

void Sandbag::HitInGuardModeFinalize(){

}

void Sandbag::AttackMonckeyModeInitilize(){

}

void Sandbag::AttackMonckeyModeUpdate(){

}

void Sandbag::AttackMonckeyModeFinalize(){

}
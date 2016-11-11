#include "Enemy.h"

#include "../h_standard.h"
#include "../h_component.h"
#include "Game/Component/CharacterControllerComponent.h"
#include "PlayerController.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void Enemy::Initialize() {
	mGravity = XMVectorSet(0, -9.81f, 0, 1);
	moveCount = 0;
	moveCountUp = true;

	childTranckingSpeed = 1.0f;

	actionModeID = ACTIONMODE::TRACKINGMODE;
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;

	actionModeInitilize[ACTIONMODE::TRACKINGMODE] = std::bind(&Enemy::TrackingModeInitilize, this/*,std::placeholders::_1*/);
	actionModeUpdate[ACTIONMODE::TRACKINGMODE] = std::bind(&Enemy::TrackingModeUpdate, this);
	actionModeFinalize[ACTIONMODE::TRACKINGMODE] = std::bind(&Enemy::TrackingModeFinalize, this);

	actionModeInitilize[ACTIONMODE::BATTLEMODE] = std::bind(&Enemy::BattleModeInitilize, this);
	actionModeUpdate[ACTIONMODE::BATTLEMODE] = std::bind(&Enemy::BattleModeUpdate, this);
	actionModeFinalize[ACTIONMODE::BATTLEMODE] = std::bind(&Enemy::BattleModeFinalize, this);

	battleActionInitilize[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeInitilize, this);
	battleActionUpdate[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeUpdate, this);
	battleActionFinalize[BATTLEACTION::CONFRONTACTION] = std::bind(&Enemy::ConfrontModeFinalize, this);

	battleActionInitilize[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeInitilize, this);
	battleActionUpdate[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeUpdate, this);
	battleActionFinalize[BATTLEACTION::APPROACHACTION] = std::bind(&Enemy::ApproachModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKDOWNACTION] = std::bind(&Enemy::AttackDownModeFinalize, this);

	battleActionInitilize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeInitilize, this);
	battleActionUpdate[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeUpdate, this);
	battleActionFinalize[BATTLEACTION::JUMPATTACKACTION] = std::bind(&Enemy::JumpAttackModeFinalize, this);

	battleActionInitilize[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::GUARDACTION] = std::bind(&Enemy::GuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeInitilize, this);
	battleActionUpdate[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeUpdate, this);
	battleActionFinalize[BATTLEACTION::BACKSTEPACTION] = std::bind(&Enemy::BackStepModeFinalize, this);

	battleActionInitilize[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeInitilize, this);
	battleActionUpdate[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeUpdate, this);
	battleActionFinalize[BATTLEACTION::WINCEACTION] = std::bind(&Enemy::WinceModeFinalize, this);

	battleActionInitilize[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeInitilize, this);
	battleActionUpdate[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeUpdate, this);
	battleActionFinalize[BATTLEACTION::HITINGUARDACTION] = std::bind(&Enemy::HitInGuardModeFinalize, this);

	battleActionInitilize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeInitilize, this);
	battleActionUpdate[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeUpdate, this);
	battleActionFinalize[BATTLEACTION::ATTACKMONCKEYACTION] = std::bind(&Enemy::AttackMonckeyModeFinalize, this);

	battleActionInitilize[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeInitilize, this);
	battleActionUpdate[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeUpdate, this);
	battleActionFinalize[BATTLEACTION::DEADACTION] = std::bind(&Enemy::DeadModeFinalize, this);

	actionModeInitilize[actionModeID]();

	if (!player) {
		player = Hx::FindActor("Player");
	}

	if (drawLog){
		Hx::Debug()->Log("捜索時の歩くスピード：" + std::to_string(trackingSpeed));
		Hx::Debug()->Log("捜索時の視界の距離：" + std::to_string(trackingRange));
		Hx::Debug()->Log("捜索時の視界の範囲：" + std::to_string(trackingAngle) + "度");
		Hx::Debug()->Log("捜索時の回転スピード：" + std::to_string(trackingRotateSpeed));
		Hx::Debug()->Log("攻撃をする範囲に入る距離：" + std::to_string(onBattleRange));
		Hx::Debug()->Log("攻撃をする範囲から抜ける距離：" + std::to_string(offBattleRange));
		Hx::Debug()->Log("体力：" + std::to_string(hp));
		Hx::Debug()->Log("周りをうろつく時の回転スピード：" + std::to_string(aproachRotateSpeed));
		Hx::Debug()->Log("攻撃時の回転の補正時の回転スピード：" + std::to_string(correctionRotateSpeed));
		Hx::Debug()->Log("ログを表示するかどうか：" + drawLog ? "TRUE" : "FALSE");
		Hx::Debug()->Log("ガードした時に反撃するまでの回数の最低：" + std::to_string(hitInGuardMinCount) + "回から");
		Hx::Debug()->Log("ガードした時に反撃するまでの回数の最高：" + std::to_string(hitInGuardMaxCount) + "回までをランダムで選択します");
		Hx::Debug()->Log("攻撃を受けた際絶対回避する確率：" + std::to_string(absolutelyAvoidInHitAttackProbability) + "%");
		Hx::Debug()->Log("playerにはplayerのGameObjectを入れてください");
		Hx::Debug()->Log("modelObjectにはenemyのモデルを表示しているのGameObject(例:子に入っているRezardMan)を入れてください");
		Hx::Debug()->Log("movePointsには敵を動かしたい地点を子にまとめた親のGameObject(例:enemyMovePoint)を入れてください");
	}
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）s
void Enemy::Start(){
}

//毎フレーム呼ばれます
void Enemy::Update(){
	vec = XMVectorZero();

	if (Input::Trigger(KeyCode::Key_1)) {
		Damage(1.0f);
	}

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
void Enemy::Finish(){

}

//コライダーとのヒット時に呼ばれます
void Enemy::OnCollideBegin(GameObject target) {
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void Enemy::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void Enemy::OnCollideExit(GameObject target){
	(void)target;
}

/****************************************************ダメージの処理********************************************************/
void Enemy::Damage(float damage_)
{
	damage = damage_;
	if (actionModeID == ACTIONMODE::BATTLEMODE && battleModeParam.battleActionID != BATTLEACTION::DEADACTION){
		if (rand() % 100 > (99 - absolutelyAvoidInHitAttackProbability) &&
			battleModeParam.battleActionID != BATTLEACTION::HITINGUARDACTION &&
			battleModeParam.battleActionID != BATTLEACTION::WINCEACTION	&&
			battleModeParam.battleActionID != BATTLEACTION::BACKSTEPACTION &&
			battleModeParam.battleActionID != BATTLEACTION::JUMPATTACKACTION){
			if (drawLog){
				Hx::Debug()->Log("避けた");
			}
			ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
		}
		else{
			if (battleModeParam.battleActionID == BATTLEACTION::GUARDACTION || battleModeParam.battleActionID == BATTLEACTION::HITINGUARDACTION){
				ChangeBattleAction(BATTLEACTION::HITINGUARDACTION);
			}
			else{
				ChangeBattleAction(BATTLEACTION::WINCEACTION);
			}
		}
	}
}

void Enemy::Attack(GameObject player_)
{
	if (drawLog)
		Hx::Debug()->Log("何かにに武器が当たった");
	if (battleModeParam.battleActionID == BATTLEACTION::ATTACKDOWNACTION ||
		battleModeParam.battleActionID == BATTLEACTION::ATTACKMONCKEYACTION ||
		battleModeParam.battleActionID == BATTLEACTION::JUMPATTACKACTION) {
		if (drawLog)
			Hx::Debug()->Log("playerに攻撃がHit");
		if (!player_)return;
		auto playerScript = player_->GetScript<PlayerController>();
		if (!playerScript)return;

		if (battleModeParam.battleActionID == BATTLEACTION::ATTACKDOWNACTION)
			playerScript->Damage(attackDamage, XMVector3Normalize(player_->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()),PlayerController::KnockBack::Low);
		else if(battleModeParam.battleActionID == BATTLEACTION::ATTACKMONCKEYACTION)
			playerScript->Damage(attackDamage, XMVector3Normalize(player_->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
		else if (battleModeParam.battleActionID == BATTLEACTION::JUMPATTACKACTION)
			playerScript->Damage(attackDamage, XMVector3Normalize(player_->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()), PlayerController::KnockBack::Down);
	}
}

/**************************************************アニメーションの処理****************************************************/
void Enemy::AnimChange(int id, float speed, bool roop, bool forcingChange)
{
	if (forcingChange){
		if (drawLog)
			Hx::Debug()->Log("強制的にアニメーションを切り替えた");

		AnimeParam ap;
		if (!modelObject)return;
		auto anim = modelObject->GetComponent<AnimationComponent>();
		if (!anim)return;

		ap = anim->GetAnimetionParam(animparam.beforeAnimId);
		ap.mWeight = 0.0f;
		ap.mLoop = animparam.beforeAnimLoop;
		anim->SetAnimetionParam(animparam.beforeAnimId, ap);

		ap = anim->GetAnimetionParam(animparam.nowAnimId);
		ap.mWeight = 0.0f;
		ap.mLoop = animparam.afterAnimLoop;
		anim->SetAnimetionParam(animparam.nowAnimId, ap);

		animparam.nowAnimId = id;
		animparam.afterAnimId = id;
		animparam.beforeAnimId = id;
		animparam.lerpSpeed = speed;
		animparam.afterAnimLoop = roop;

		animparam.nowLerpTime = 0.0f;
		animparam.animLerpFlag = false;
		animparam.beforeAnimLoop = animparam.afterAnimLoop;

		ap = anim->GetAnimetionParam(animparam.nowAnimId);
		ap.mTime = 0.0f;

		ap.mWeight = 1.0f;
		ap.mLoop = animparam.beforeAnimLoop;
		anim->SetAnimetionParam(animparam.nowAnimId, ap);

		return;
	}
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

void Enemy::AnimLerp()
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

float Enemy::GetNowAnimTime(){
	if (!modelObject)return 0;
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return 0;

	AnimeParam ap;
	ap = anim->GetAnimetionParam(animparam.nowAnimId);

	return ap.mTime;
}

/********************************************アクションの変更、戦闘時の変更************************************************/
void Enemy::ChangeActionMode(ACTIONMODE nextActionMode){
	if (battleModeParam.battleActionID != BATTLEACTION::DEADACTION) {
		actionModeID = nextActionMode;
		actionModeInitilize[actionModeID]();
		actionModeUpdate[actionModeID]();
	}
}

void Enemy::ChangeBattleAction(BATTLEACTION nextBattleAction){
	if (battleModeParam.battleActionID != BATTLEACTION::DEADACTION) {
		if (battleModeParam.battleActionID == nextBattleAction) {
			battleModeParam.sameActionCount++;
			battleModeParam.firstInSameAction = false;
		}
		else {
			battleModeParam.sameActionCount = 0;
			battleModeParam.firstInSameAction = true;
		}

		battleModeParam.beforeBattleActionID = battleModeParam.battleActionID;
		battleModeParam.battleActionID = nextBattleAction;
		battleActionInitilize[battleModeParam.battleActionID]();
		battleActionUpdate[battleModeParam.battleActionID]();
	}
}

void Enemy::ChangeBattleAction(int guardProbability, int approachProbability, int backstepProbability, int attackProbability, int jumpAttackProbability){
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
void Enemy::TrackingModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("捜索モードへ移行");
	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;

	GameObject movePoint;
	if (!child) {
		if (!movePoints)return;
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
		movePoint = *movePointList.begin();
		i = 0;
		for (auto itr = movePointList.begin(); i <= moveCount; ++itr, ++i) {
			movePoint = *itr;
		}
	}
	else {
		movePoint = movePoints;
		trackingRotateSpeed *= 2;
	}
	navi->RootCreate(gameObject, movePoint);
	moveCountUp = true;

	Hx::PhysX()->Raycast(gameObject->mTransform->WorldPosition(),
		XMVector3Normalize(movePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()),
		3.0f);
}

void Enemy::TrackingModeUpdate()
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

	if (child) {
		if (XMVector3Length(movePoints->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x > 3.0f) {
			childTranckingSpeed = 1.5f;
		}
		else {
			childTranckingSpeed = 1.0f;
		}
	}

	auto navi = gameObject->GetComponent<NaviMeshComponent>();
	if (!navi)return;
	if (navi->IsMoveEnd()) {
		GameObject movePoint;
		if (!child) {
			if (!movePoints)return;
			auto movePointList = movePoints->mTransform->Children();
			int maxMoveCount = movePointList.size();
			if (maxMoveCount == 0)return;
			maxMoveCount--;
			int i = 0;
			movePoint = *movePointList.begin();
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
		else {
			movePoint = movePoints;
			if (XMVector3Length(movePoint->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x < 1.0f) {
				childTranckingSpeed = 0.0f;
			}
			else {
				childTranckingSpeed = 1.0f;
				navi->RootCreate(gameObject, movePoint);
			}
		}
	}
	
	navi->Move(trackingSpeed * childTranckingSpeed * Hx::DeltaTime()->GetDeltaTime());

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

	vec += forward * trackingSpeed * childTranckingSpeed;
}

void Enemy::TrackingModeFinalize()
{
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	ChangeActionMode(ACTIONMODE::BATTLEMODE);
}

/****************************************************戦闘時の処理**********************************************************/
void Enemy::BattleModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("戦闘モードへ移行");
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	battleActionInitilize[battleModeParam.battleActionID]();
}

void Enemy::BattleModeUpdate()
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

void Enemy::BattleModeFinalize()
{
	battleModeParam.battleActionID = BATTLEACTION::CONFRONTACTION;
	ChangeActionMode(ACTIONMODE::TRACKINGMODE);
}

void Enemy::ConfrontModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("近づく");
}

void Enemy::ConfrontModeUpdate()
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

void Enemy::ConfrontModeFinalize()
{
	ChangeBattleAction(30, 30, 20, 20, 0);
}

void Enemy::ApproachModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("周りをうろつく");
	battleModeParam.count = 0.0f;
	battleModeParam.rotateVecPlus = !battleModeParam.rotateVecPlus;
}

void Enemy::ApproachModeUpdate()
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

void Enemy::ApproachModeFinalize()
{
	ChangeBattleAction(30, 0, 40, 30, 0);
}

void Enemy::AttackDownModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("縦切り");
}

void Enemy::AttackDownModeUpdate()
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

void Enemy::AttackDownModeFinalize()
{
	ChangeBattleAction(40, 40, 20, 0, 0);
}

void Enemy::JumpAttackModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("ジャンプ切り");
}

void Enemy::JumpAttackModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	AnimChange(ANIM_ID::ANIM_JUMPATTACK, 5.0f, false);
	if (GetNowAnimTime() < 12.5f) {
		vec += forward * 20.0f;
	}
	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		battleActionFinalize[battleModeParam.battleActionID]();
	};
}

void Enemy::JumpAttackModeFinalize()
{
	ChangeBattleAction(40, 20, 10, 30, 0);
	//ChangeBattleAction(0, 0, 100, 0, 0);
}

void Enemy::GuardModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("ガード");
	battleModeParam.count = 0.0f;
	if (rand() % 2 == 0){
		battleModeParam.rotateVecPlus = true;
	}
	else{
		battleModeParam.rotateVecPlus = false;
	}
}

void Enemy::GuardModeUpdate()
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

void Enemy::GuardModeFinalize()
{
	ChangeBattleAction(20, 30, 20, 30, 0);
}

void Enemy::BackStepModeInitilize()
{
	if (drawLog)
		Hx::Debug()->Log("バックステップ");
}

void Enemy::BackStepModeUpdate()
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

void Enemy::BackStepModeFinalize()
{
	ChangeBattleAction(30, 0, 0, 0, 70);
}

void Enemy::WinceModeInitilize(){
	hp -= damage;
	if (drawLog)
		Hx::Debug()->Log(std::to_string(damage) + "ダメージ喰らった。残りの体力は" + std::to_string(hp));
	if (hp <= 0) {
		ChangeBattleAction(BATTLEACTION::DEADACTION);
		return;
	}
	AnimChange(ANIM_ID::ANIM_WINCE, 5.0f, false, true);
}

void Enemy::WinceModeUpdate(){
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;
	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		battleActionFinalize[battleModeParam.battleActionID]();
	};
}

void Enemy::WinceModeFinalize(){
	ChangeBattleAction(40, 0, 40, 20, 0);
}

void Enemy::HitInGuardModeInitilize(){
	if (drawLog)
		Hx::Debug()->Log("防いだ");
	AnimChange(ANIM_ID::ANIM_HITINGUARD, 5.0f, false, true);

	if (battleModeParam.firstInSameAction){
		PatienceInThisTime = ((rand() % hitInGuardMaxCount) + hitInGuardMinCount);
		if (drawLog)
			Hx::Debug()->Log(std::to_string(PatienceInThisTime + 1) + "回防いだらモンキーアタックします");
	}
}

void Enemy::HitInGuardModeUpdate(){
	if (PatienceInThisTime > battleModeParam.sameActionCount){
		auto anim = modelObject->GetComponent<AnimationComponent>();
		if (!anim)return;
		if (anim->IsAnimationEnd(animparam.nowAnimId)) {
			battleActionFinalize[battleModeParam.battleActionID]();
		};
	}
	else{
		ChangeBattleAction(BATTLEACTION::ATTACKMONCKEYACTION);
	}
}

void Enemy::HitInGuardModeFinalize(){
	ChangeBattleAction(40, 0, 40, 20, 0);
}

void Enemy::AttackMonckeyModeInitilize(){
	if (drawLog)
		Hx::Debug()->Log("モンキーアタック");
}

void Enemy::AttackMonckeyModeUpdate(){
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	AnimChange(ANIM_ID::ANIM_ATTACK_MONCKEY, 5.0f, false);
	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		battleActionFinalize[battleModeParam.battleActionID]();
	};
}

void Enemy::AttackMonckeyModeFinalize(){
	ChangeBattleAction(40, 40, 20, 0, 0);
}

void Enemy::DeadModeInitilize()
{
	if (drawLog) {
		Hx::Debug()->Log("死んだ");
	}

	AnimChange(ANIM_ID::ANIM_PROVOCATION, 5.0f, false, true);
}

void Enemy::DeadModeUpdate()
{
	auto anim = modelObject->GetComponent<AnimationComponent>();
	if (!anim)return;

	if (anim->IsAnimationEnd(animparam.nowAnimId)) {
		if (!myWeapon)return;
		//gameObject->Enable();
		gameObject->RemoveComponent<CharacterControllerComponent>();
		Hx::DestroyObject(myWeapon);
		Hx::DestroyObject(gameObject);
	};
}

void Enemy::DeadModeFinalize()
{
}

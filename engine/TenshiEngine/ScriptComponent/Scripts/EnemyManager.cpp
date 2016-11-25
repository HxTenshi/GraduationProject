#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"

#include "Enemy.h"

//生成時に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Initialize(){

}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void EnemyManager::Start(){
	EnemysIntoEnemyContainer();
	for (auto i : m_EnemyContainer) {
		for (auto j : i.enemyOneVec) {
			auto jScript = Enemy::GetEnemy(j.enemyGameObject);
			if (!jScript)return;
			auto eap = jScript->GetEnemyAllParameter(false);
			if (eap.actionMode == ACTIONMODE::TRACKINGMODE) {
				jScript->ChangeActionAndTrackingAction(eap.actionMode,eap.trackingModeParameter.id);
			}
			else {
				jScript->ChangeActionAndBattleAction(eap.actionMode,eap.battleModeParameter.id);
			}
		}
	}
}

//毎フレーム呼ばれます
void EnemyManager::Update(){
	//敵の入れ物を全部回す
	for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end();) {
		bool deleteTeam = false;
		//チームごとに回している
		for (auto j = i->enemyOneVec.begin(); j != i->enemyOneVec.end();) {
			//死んだら
			if (j->enemyGameObject.Get()->EndFinish()) {
				//親なら親死んだフラグにセット
				if (!j->enemyParameter.child)i->enemyTeamParameter.parentAlive = false;
				//vectorから削除
				j = i->enemyOneVec.erase(j);
				//チームの人数を減らす
				i->enemyTeamParameter.teamCount -= 1;
				if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(i->enemyTeamParameter.teamID) + " : " + std::to_string(i->enemyTeamParameter.teamCount) + "人");
				//もしチーム人数が０になったら
				if (i->enemyOneVec.size() == 0) {
					if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(i->enemyTeamParameter.teamID) + " : " + "チームが壊滅した");
					//vectorから削除
					i = m_EnemyContainer.erase(i);
					deleteTeam = true;
					break;
				}
				else {
					continue;
				}
			}

			auto jScript = Enemy::GetEnemy(j->enemyGameObject);
			if (!jScript)return;
			auto eap = jScript->GetEnemyAllParameter(false);
			if (eap.actionMode == ACTIONMODE::TRACKINGMODE) {
				//発見したかどうか
				i->enemyTeamParameter.lostPlayer = false;
				if (!i->enemyTeamParameter.discoveryPlayer) {
					i->enemyTeamParameter.discoveryPlayer = jScript->DiscoveryPlayer();
				}
			}
			else {
				//見失ったかどうか
				i->enemyTeamParameter.discoveryPlayer = false;
				i->enemyTeamParameter.lostPlayer = jScript->LostPlayer();
			}

			j++;
		}
		//チームがなければ次のチームへ
		if (deleteTeam)continue;

		auto playerPos = m_Player->mTransform->WorldPosition();
		//戦闘時の移動先を計算するのに使う(↓3つ全部)
		XMMATRIX moveToBattlePos;
		int howManyPeople = 0;
		float battlePosAngle = 3.14f / (float)(i->enemyTeamParameter.teamCount);
		for (auto j = i->enemyOneVec.begin(); j != i->enemyOneVec.end(); j++,howManyPeople++) {
			auto jScript = Enemy::GetEnemy(j->enemyGameObject);
			if (!jScript)return;
			auto eap = jScript->GetEnemyAllParameter(true);
			//発見したら戦闘モードへ移行
			if (i->enemyTeamParameter.discoveryPlayer && eap.actionMode != ACTIONMODE::BATTLEMODE) {
				jScript->ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
				i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
				if(m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
			}
			//見失ったら捜索モードへ移行
			if (i->enemyTeamParameter.lostPlayer && eap.actionMode != ACTIONMODE::TRACKINGMODE) {
				jScript->ChangeActionMode(ACTIONMODE::TRACKINGMODE);
			}
			//親が生きているかのフラグセット
			jScript->SetParentAlive(i->enemyTeamParameter.parentAlive);
			if (!m_Player)return;
			//戦闘時だったら
			if (eap.actionMode == ACTIONMODE::BATTLEMODE) {
				//移動先を検索
				XMVECTOR battlePos;
				if (howManyPeople == 0) {
					moveToBattlePos = XMMatrixTranslationFromVector(XMVector3Normalize(j->enemyGameObject->mTransform->WorldPosition() - playerPos) * jScript->GetOnBattleRange());
					battlePos = XMMatrixMultiply(moveToBattlePos, XMMatrixTranslationFromVector(playerPos)).r[3];
				}
				else {
					battlePos = XMMatrixMultiply(XMMatrixMultiply(moveToBattlePos, XMMatrixRotationY(battlePosAngle)), XMMatrixTranslationFromVector(playerPos)).r[3];
					if (howManyPeople % 2 == 0) {
						battlePosAngle *= -2.0f;
					}
					else {
						battlePosAngle *= -1.0f;
					}
				}
				jScript->SetBattlePosition(battlePos);

				if (i->enemyTeamParameter.teamCount == 1) {
					//アクションが終了したため次のを決める
					if (eap.battleModeParameter.actionFinish) {
						if (eap.battleModeParameter.id == BATTLEACTION::CONFRONTACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(30, 30, 20, 20, 0, 5));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::APPROACHACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(30, 0, 40, 30, 0, 5));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::ATTACKDOWNACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(40, 40, 20, 0, 0, 5));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::JUMPATTACKACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(40, 20, 0, 30, 0, 5));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::GUARDACTION) {
							if (eap.battleModeParameter.beforeId == BATTLEACTION::BACKSTEPACTION) {
								jScript->ChangeBattleAction(Enemy::ChangeBattleAction(20, 30, 0, 30, 0, 5));
							}
							else {
								jScript->ChangeBattleAction(Enemy::ChangeBattleAction(20, 30, 20, 30, 0, 5));
							}
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::PROVOCATION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(30, 0, 40, 30, 0, 0));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::BACKSTEPACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(15, 0, 0, 0, 70, 15));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::WINCEACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(40, 0, 40, 20, 0, 5));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::HITINGUARDACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(40, 0, 40, 20, 0, 5));
						}
						else if (eap.battleModeParameter.id == BATTLEACTION::ATTACKMONCKEYACTION) {
							jScript->ChangeBattleAction(Enemy::ChangeBattleAction(40, 40, 20, 0, 0, 5));
						}
					}
				}
				else{
					//攻撃
					if (j->enemyParameter.attack) {
						if (eap.battleModeParameter.actionFinish) {
							j->enemyParameter.attack = false;
							i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
							i->enemyTeamParameter.whoAttack++;
						}
					}
					//同時攻撃
					else if (j->enemyParameter.everyoneAttack) {
						//誰か一人でもしていたらできていないやつをできなくする
						if (i->enemyTeamParameter.everyoneAttack) {
							i->enemyTeamParameter.everyoneAttackCountFlag = false;
							i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
						}
						//終わったら
						if (eap.battleModeParameter.id == BATTLEACTION::JUMPATTACKACTION) {
							j->enemyParameter.everyoneAttack = false;
							i->enemyTeamParameter.everyoneAttack = false;
							i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
						}
						else{
							//バックステップが終わったら
							if (eap.battleModeParameter.id == BATTLEACTION::BACKSTEPACTION) {
								if (eap.battleModeParameter.actionFinish) {
									jScript->ChangeBattleAction(BATTLEACTION::JUMPATTACKACTION);
								}
							}
							else {
								i->enemyTeamParameter.everyoneAttackCountFlag = false;
								i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
								jScript->ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
							}
						}
					}
					//アクションが終了したため次のを決める
					else if (eap.battleModeParameter.actionFinish) {
						//自分の番だが攻撃できない場合
						if (eap.battleModeParameter.id != BATTLEACTION::APPROACHACTION &&
							eap.battleModeParameter.id != BATTLEACTION::GUARDACTION &&
							eap.battleModeParameter.id != BATTLEACTION::PROVOCATION &&
							eap.battleModeParameter.id != BATTLEACTION::CONFRONTACTION &&
							eap.battleModeParameter.id != BATTLEACTION::ATTACKDOWNACTION &&
							eap.battleModeParameter.id != BATTLEACTION::JUMPATTACKACTION &&
							i->enemyTeamParameter.whoAttack == howManyPeople) {
							i->enemyTeamParameter.whoAttack++;
						}
			
						//全員が攻撃したら
						if (i->enemyTeamParameter.whoAttack >= i->enemyTeamParameter.teamCount) {
							i->enemyTeamParameter.whoAttack = 0;
							i->enemyTeamParameter.everyoneAttackCountFlag = true;
						}
			
						if (!j->enemyParameter.attack && i->enemyTeamParameter.whoAttack == howManyPeople && !i->enemyTeamParameter.everyoneAttackCountFlag) {
							j->enemyParameter.nextAttackTimeCountFlag = true;
						}
						if (!j->enemyParameter.attack) {
							//自分の番じゃなかったらランダムで指定
							switch (rand() % 3) {
							case 0:
								jScript->ChangeBattleAction(BATTLEACTION::APPROACHACTION);
								break;
							case 1:
								jScript->ChangeBattleAction(BATTLEACTION::GUARDACTION);
								break;
							case 2:
								jScript->ChangeBattleAction(BATTLEACTION::PROVOCATION);
								break;
							default:
								break;
							}
						}
					}
					else if (j->enemyParameter.nextAttackTimeCountFlag) {
						i->enemyTeamParameter.nextAttackTimeCount += Hx::DeltaTime()->GetDeltaTime();
						if (i->enemyTeamParameter.nextAttackTimeCount >= i->enemyTeamParameter.nextAttackTime) {
							if(i->enemyTeamParameter.nextAttackTimeCount < 5 && eap.battleModeParameter.arrival && eap.battleModeParameter.canChangeAttackAction) {
								j->enemyParameter.attack = true;
								j->enemyParameter.nextAttackTimeCountFlag = false;
								i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
								jScript->ChangeBattleAction(BATTLEACTION::ATTACKDOWNACTION);
								if (m_DrawFlag)Hx::Debug()->Log("nextAttackTimeCount" + std::to_string(i->enemyTeamParameter.nextAttackTime));
							}
							else{
								j->enemyParameter.nextAttackTimeCountFlag = false;
								i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
								i->enemyTeamParameter.whoAttack++;
								if (m_DrawFlag)Hx::Debug()->Log("skip");
							}
						}
					}
					else if (i->enemyTeamParameter.everyoneAttackCountFlag) {
						if (i->enemyTeamParameter.nextAttackTimeCount >= i->enemyTeamParameter.nextAttackTime) {
							if (i->enemyTeamParameter.nextAttackTimeCount < 5 && eap.battleModeParameter.canChangeAttackAction) {
								j->enemyParameter.everyoneAttack = true;
								i->enemyTeamParameter.everyoneAttack = true;
								if (m_DrawFlag)Hx::Debug()->Log("nextAttackTimeCount" + std::to_string(i->enemyTeamParameter.nextAttackTime));
							}
						}
					}
				}
			}
		}
		if (i->enemyTeamParameter.everyoneAttackCountFlag) {
			i->enemyTeamParameter.nextAttackTimeCount += Hx::DeltaTime()->GetDeltaTime();
		}
		i++;
	}
}

//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void EnemyManager::Finish(){

}

//コライダーとのヒット時に呼ばれます
void EnemyManager::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void EnemyManager::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void EnemyManager::OnCollideExit(GameObject target){
	(void)target;
}

void EnemyManager::EnemysIntoEnemyContainer(){
	if (!m_Enemys)return;
	auto enemyTeams = m_Enemys->mTransform->Children();
	if (enemyTeams.size() == 0)return;
	for (auto newEnemyTeam : enemyTeams) {
		if (!newEnemyTeam)return;
		newEnemyTeam->Enable();
		EnemyOne eo;
		EnemyTeam et;
		int count = 0;
		if (Enemy::GetEnemy(newEnemyTeam)) {
			count++;
			auto iScript = Enemy::GetEnemy(newEnemyTeam);
			eo.enemyGameObject = newEnemyTeam;
			eo.enemyParameter.child = false;
			iScript->SetActionID(ACTIONMODE::TRACKINGMODE);
			iScript->SetTrackingID(TRACKINGACTION::PARENTTRACKING);
			et.enemyOneVec.insert(et.enemyOneVec.begin(), eo);
		}
		else {
			auto enemyTeamChild = newEnemyTeam->mTransform->Children();
			if (enemyTeamChild.size() == 0)return;

			for (auto i : enemyTeamChild) {
				count++;
				if (!i)return;
				auto iScript = Enemy::GetEnemy(i);
				if (!iScript)return;
				eo.enemyGameObject = i;
				iScript->SetPlayer(m_Player);
				if (iScript->GetChildFlag()) {
					eo.enemyParameter.child = true; 
					iScript->SetActionID(ACTIONMODE::TRACKINGMODE);
					iScript->SetTrackingID(TRACKINGACTION::CHILDTRACKING);
					et.enemyOneVec.push_back(eo);
				}
				else {
					eo.enemyParameter.child = false;
					iScript->SetActionID(ACTIONMODE::TRACKINGMODE);
					iScript->SetTrackingID(TRACKINGACTION::PARENTTRACKING);
					et.enemyOneVec.insert(et.enemyOneVec.begin(), eo);
				}
			}
		}

		et.enemyTeamParameter.teamCount = count;

		count = 0;
		for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end(); i++) {
			if (i->enemyTeamParameter.teamID == count) {
				count++;
				i = m_EnemyContainer.begin();
			}
		}
		et.enemyTeamParameter.teamID = count;
		if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(et.enemyTeamParameter.teamID) + " : " + std::to_string(et.enemyTeamParameter.teamCount) + "人");
		m_EnemyContainer.push_back(et);
	}
}
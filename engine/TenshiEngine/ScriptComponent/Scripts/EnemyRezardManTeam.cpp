#include "EnemyRezardManTeam.h"
#include "Enemy.h"
#include "EnemyRezardMan.h"
#include "EnemyArcher.h"

#include "../h_standard.h"
#include "../h_component.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyRezardManTeam::TeamInitialize(){
	auto enemyTeamChild = gameObject->mTransform->Children();
	if (enemyTeamChild.size() == 0)return;
	for (auto& i : enemyTeamChild) {
		if (!i)return;
		auto iScript = Enemy::GetEnemy(i);
		if (!iScript)return;
		EnemyOne eo;
		eo.enemyParameter.enemy_type = iScript->GetEnemyType();
		if (iScript->GetEnemyType() == ENEMY_TYPE::CHILD) {
			iScript->ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::CHILDTRACKING);
			eo.enemyGameObject = i; 
			teamMember.push_back(eo);
			}
		else if (iScript->GetEnemyType() == ENEMY_TYPE::PARENT) {
			eo.enemyGameObject = i;
			iScript->ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::PARENTTRACKING);
			teamMember.insert(teamMember.begin(),eo);
			}
		else if (iScript->GetEnemyType() == ENEMY_TYPE::CHILD_ARCHER) {
			eo.enemyGameObject = i;
			archerCount++;
			iScript->ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::CHILDTRACKING);
			teamMember.push_back(eo);
		}
	}
	m_DiscoveryPlayer = false;
}

bool EnemyRezardManTeam::Alive()
{
	//�`�[�����Ƃɉ񂵂Ă���
	for (auto j = teamMember.begin(); j != teamMember.end();) {
		//���񂾂�
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return false;
		if (jScript->IsEnd()) {
			//�e�Ȃ�e���񂾃t���O�ɃZ�b�g
			if (j->enemyParameter.enemy_type == ENEMY_TYPE::PARENT)parentAlive = false;
			if (j->enemyParameter.enemy_type == ENEMY_TYPE::CHILD_ARCHER)archerCount--;
			//vector����폜
			Enemy::GetEnemy(j->enemyGameObject)->ChildFinalize();
			j = teamMember.erase(j);
			if (m_DrawFlag)Hx::Debug()->Log("RezardManTeam : " + std::to_string(teamMember.size()) + "�l");
			battlePosFirst = 0;
			if (teamMember.size() == 0) {
				if (m_DrawFlag)Hx::Debug()->Log("RezardManTeam����ł���");
				return false;
			}
			continue;
		}
		j++;
	}

	return true;
}

void EnemyRezardManTeam::DiscoveryOrLostPlayerSet()
{
	bool lostPlayer = true;
	bool discoveryPlayer = false;
	for (auto j = teamMember.begin(); j != teamMember.end(); j++) {
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return;
		auto eap = jScript->GetEnemyAllParameter(false);
		if (eap.actionMode == ACTIONMODE::TRACKINGMODE) {
			//�����������ǂ���
			lostPlayer = false;
			if (!discoveryPlayer) {
				discoveryPlayer = jScript->DiscoveryPlayer();
			}
		}
		else {
			//�����������ǂ���
			discoveryPlayer = false;
			if (lostPlayer) {
				lostPlayer = jScript->LostPlayer();
			}
		}

		if (m_DiscoveryPlayer && lostPlayer && !j->enemyParameter.everyoneAttack) {
			m_DiscoveryPlayer = false;
		}
		else if (!m_DiscoveryPlayer && discoveryPlayer) {
			whoAttack = 0;
			m_DiscoveryPlayer = true;
		}
		if (jScript->GetWasAttacked()) {
			//�����������ǂ���
			lostPlayer = false;
			nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
			if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
			wasAttacked = true;
			m_DiscoveryPlayer = true;
		}
	}
}

void EnemyRezardManTeam::TeamUpdate()
{
	if (!m_Player)return;
	auto playerPos = m_Player->mTransform->WorldPosition();
	//�퓬���̈ړ�����v�Z����̂Ɏg��(��3�S��)
	XMVECTOR parentMoveVec;
	int howManyPeople = 0;
	float battlePosAngle = 3.14f / (float)(teamMember.size() - archerCount);

	XMVECTOR firstBattlePos;
	auto firstScript = Enemy::GetEnemy(teamMember[battlePosFirst].enemyGameObject);
	if (!firstScript) {
		return;
	}
	parentMoveVec = XMVector3Normalize(teamMember[battlePosFirst].enemyGameObject->mTransform->WorldPosition() - playerPos);
	auto moveToBattlePos = XMMatrixTranslationFromVector(parentMoveVec * firstScript->GetOnBattleRange());
	firstBattlePos = XMMatrixMultiply(moveToBattlePos, XMMatrixTranslationFromVector(playerPos)).r[3];

	std::vector<XMVECTOR> battlePos;
	int firstSearch = 0;
	int aftersCount = 0;
	for (auto& j : teamMember) {
		if (j.enemyParameter.enemy_type == ENEMY_TYPE::CHILD_ARCHER)continue;
		auto jScript = Enemy::GetEnemy(j.enemyGameObject);
		if (!jScript)return;
		if (firstSearch == battlePosFirst) {
			battlePos.push_back(firstBattlePos);
		}
		else {
			auto moveToBattlePos = XMMatrixTranslationFromVector(parentMoveVec * jScript->GetOnBattleRange());
			battlePos.push_back(XMMatrixMultiply(XMMatrixMultiply(moveToBattlePos, XMMatrixRotationY(battlePosAngle)), XMMatrixTranslationFromVector(playerPos)).r[3]);
			aftersCount++;
			if (aftersCount % 2 == 0) {
				battlePosAngle *= -2.0f;
			}
			else {
				battlePosAngle *= -1.0f;
			}
		}
		firstSearch++;
	}

	for (auto& j : teamMember) {
		auto jScript = Enemy::GetEnemy(j.enemyGameObject);
		if (!jScript)return;
		auto eap = jScript->GetEnemyAllParameter(true);
		//����������퓬���[�h�ֈڍs
		if (m_DiscoveryPlayer && eap.actionMode != ACTIONMODE::BATTLEMODE) {
			jScript->ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
			nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
			if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
		}
		//����������{�����[�h�ֈڍs
		if (!m_DiscoveryPlayer && 
			eap.actionMode != ACTIONMODE::TRACKINGMODE &&
			eap.battleModeParameter.id != BATTLEACTION::DEADACTION &&
			eap.battleModeParameter.actionFinish &&
			!j.enemyParameter.everyoneAttack &&
			!wasAttacked) {
			whoAttack = 0;
			jScript->ChangeActionMode(ACTIONMODE::TRACKINGMODE);
		}

		//�e�������Ă��邩�̃t���O�Z�b�g
		jScript->SetParentAlive(parentAlive);
		if (!m_Player)return;

		if (wasAttacked && (eap.battleModeParameter.actionFinish || eap.actionMode == ACTIONMODE::TRACKINGMODE)) {
			if (eap.battleModeParameter.id == BATTLEACTION::CONFRONTACTION) {
				m_DiscoveryPlayer = true;
				wasAttacked = false;
				whoAttack = 0;
			}
			else {
				jScript->ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
			}
		}
		//�퓬����������
		else if (eap.actionMode == ACTIONMODE::BATTLEMODE) {	
			if (j.enemyParameter.enemy_type == ENEMY_TYPE::CHILD_ARCHER) {
				if (teamMember.size() == 1 || archerCount == teamMember.size()) {
					//�A�N�V�������I���������ߎ��̂����߂�
					if (eap.battleModeParameter.actionFinish) {
						jScript->SoloAction();
					}
				}
				else {
					if (j.enemyParameter.m_ArcharAttackStart) {
						j.enemyParameter.m_ArcharAttackStart = false;
						jScript->ChangeBattleAction(BATTLEACTION::SHOTACTION);
					}
					if (eap.battleModeParameter.actionFinish) {
						jScript->ChangeBattleAction(BATTLEACTION::CONFRONTACTION);
					}
				}
			}
			else {
				jScript->SetBattlePosition(battlePos[howManyPeople]);
				if (teamMember.size() == 1){
					//�A�N�V�������I���������ߎ��̂����߂�
					if (eap.battleModeParameter.actionFinish) {
						jScript->SoloAction();
					}
				}
				else {
					//�U��
					if (j.enemyParameter.attack) {
						if (eap.battleModeParameter.id == BATTLEACTION::WINCEACTION ||
							eap.battleModeParameter.id == BATTLEACTION::UPPERDOWNACTION ||
							eap.battleModeParameter.id == BATTLEACTION::BEATDOWNACTION ||
							eap.battleModeParameter.id == BATTLEACTION::DOWNACTION ||
							eap.battleModeParameter.actionFinish) {
							j.enemyParameter.attack = false;
							whoAttack++;
							nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
						}
					}
					//�����U��
					else if (j.enemyParameter.everyoneAttack) {
						//�N����l�ł����Ă�����ł��Ă��Ȃ�����ł��Ȃ�����
						if (everyoneAttack) {
							everyoneAttackCountFlag = false;
							nextAttackTimeCount = 0.0f;
							battlePosFirst++;
							if (battlePosFirst >= teamMember.size() - archerCount) {
								battlePosFirst = 0;
							}
						}
						//�I�������
						if (eap.battleModeParameter.id == BATTLEACTION::WINCEACTION ||
							eap.battleModeParameter.id == BATTLEACTION::UPPERDOWNACTION ||
							eap.battleModeParameter.id == BATTLEACTION::BEATDOWNACTION ||
							eap.battleModeParameter.id == BATTLEACTION::DOWNACTION ||
							eap.battleModeParameter.id == BATTLEACTION::ATTACK2ACTION) {
							j.enemyParameter.everyoneAttack = false;
							everyoneAttack = false;
							nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
						}
						else {
							//�o�b�N�X�e�b�v���I�������
							if (eap.battleModeParameter.id == BATTLEACTION::BACKSTEPACTION) {
								if (eap.battleModeParameter.actionFinish) {
									jScript->ChangeBattleAction(BATTLEACTION::ATTACK2ACTION);
								}
							}
							else {
								everyoneAttackCountFlag = false;
								nextAttackTimeCount = 0.0f;
								jScript->ChangeBattleAction(BATTLEACTION::BACKSTEPACTION);
							}
						}
					}
					//�A�N�V�������I���������ߎ��̂����߂�
					else if (eap.battleModeParameter.actionFinish) {
						////�����̔Ԃ����U���ł��Ȃ��ꍇ
						//if (!eap.battleModeParameter.canChangeAttackAction &&
						//	whoAttack == howManyPeople) {
						//	whoAttack++;
						//}

						//�S�����U��������
						if (whoAttack >= teamMember.size() - archerCount) {
							whoAttack = 0;
							everyoneAttackCountFlag = true;
						}
						if (!j.enemyParameter.attack && whoAttack == howManyPeople && !everyoneAttackCountFlag) {
							j.enemyParameter.nextAttackTimeCountFlag = true;
						}
						if (!j.enemyParameter.attack) {
							//�����̔Ԃ���Ȃ������烉���_���Ŏw��
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
					else if (j.enemyParameter.nextAttackTimeCountFlag) {
						nextAttackTimeCount += Hx::DeltaTime()->GetDeltaTime();
						if (nextAttackTimeCount >= nextAttackTime) {
							if (nextAttackTimeCount < 5 && eap.battleModeParameter.arrival && eap.battleModeParameter.canChangeAttackAction) {
								j.enemyParameter.attack = true;
								j.enemyParameter.nextAttackTimeCountFlag = false;
								nextAttackTimeCount = 0.0f;
								jScript->ChangeBattleAction(BATTLEACTION::ATTACK1ACTION);
								for (auto& archerSearch : teamMember) {
									if (archerSearch.enemyParameter.enemy_type == ENEMY_TYPE::CHILD_ARCHER) {
										archerSearch.enemyParameter.m_ArcharAttackStart = true;
									}
								}
								if (m_DrawFlag)Hx::Debug()->Log("nextAttackTimeCount" + std::to_string(nextAttackTime));
							}
							else {
								j.enemyParameter.nextAttackTimeCountFlag = false;
								nextAttackTimeCount = 0.0f;
								nextAttackTime = 0.0f;
								whoAttack++;
								if (m_DrawFlag)Hx::Debug()->Log("skip");
							}
						}
					}
					else if (everyoneAttackCountFlag) {
						if (nextAttackTimeCount >= nextAttackTime) {
							if (nextAttackTimeCount < 5 && eap.battleModeParameter.canChangeAttackAction) {
								j.enemyParameter.everyoneAttack = true;
								everyoneAttack = true;
								if (m_DrawFlag)Hx::Debug()->Log("nextAttackTimeCount" + std::to_string(nextAttackTime));
							}
						}
					}
				}
			}
		}

		if(j.enemyParameter.enemy_type != ENEMY_TYPE::CHILD_ARCHER)
		howManyPeople++;
	}
	if (everyoneAttackCountFlag) {
		nextAttackTimeCount += Hx::DeltaTime()->GetDeltaTime();
	}
}
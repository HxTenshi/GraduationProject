#include "EnemyRezardManTeam.h"
#include "Enemy.h"
#include "EnemyRezardMan.h"

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
		iScript->SetPlayer(m_Player);
		if (iScript->GetChildFlag()) {
			iScript->ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::CHILDTRACKING);
			EnemyOne eo;
			eo.enemyGameObject = i;
			teamMember.push_back(eo);
		}
		else {
			EnemyOne eo;
			eo.enemyGameObject = i;
			iScript->ChangeActionAndTrackingAction(ACTIONMODE::TRACKINGMODE, TRACKINGACTION::PARENTTRACKING);
			teamMember.insert(teamMember.begin(),eo);
		}
	}
	lostPlayer = false;
	discoveryPlayer = false;
}

bool EnemyRezardManTeam::Alive()
{
	bool alive = true;
	//�`�[�����Ƃɉ񂵂Ă���
	for (auto j = teamMember.begin(); j != teamMember.end();) {
		//���񂾂�
		auto jScript = Enemy::GetEnemy(j->enemyGameObject);
		if (!jScript)return true;
		if (jScript->IsEnd()) {
			//�e�Ȃ�e���񂾃t���O�ɃZ�b�g
			if (!jScript->GetChildFlag())parentAlive = false;
			//vector����폜
			j = teamMember.erase(j);
			if (m_DrawFlag)Hx::Debug()->Log("RezardManTeam : " + std::to_string(teamMember.size()) + "�l");
			//�����`�[���l�����O�ɂȂ�����
			if (teamMember.size() == 0) {
				if (m_DrawFlag)Hx::Debug()->Log("RezardManTeam����ł���");
				alive = false;
				break;
			}
			continue;
		}
		j++;
	}
	return alive;
}

void EnemyRezardManTeam::DiscoveryOrLostPlayerSet()
{
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
			lostPlayer = jScript->LostPlayer();
		}
	}
}

void EnemyRezardManTeam::TeamUpdate()
{
	auto playerPos = m_Player->mTransform->WorldPosition();
	//�퓬���̈ړ�����v�Z����̂Ɏg��(��3�S��)
	XMMATRIX moveToBattlePos;
	int howManyPeople = 0;
	float battlePosAngle = 3.14f / (float)(teamMember.size());
	for (auto& j : teamMember) {
		auto jScript = Enemy::GetEnemy(j.enemyGameObject);
		if (!jScript)return;
		auto eap = jScript->GetEnemyAllParameter(true);
		//����������퓬���[�h�ֈڍs
		if (discoveryPlayer && eap.actionMode != ACTIONMODE::BATTLEMODE) {
			jScript->ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
			nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
			if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
		}
		//����������{�����[�h�ֈڍs
		if (lostPlayer && eap.actionMode != ACTIONMODE::TRACKINGMODE) {
			jScript->ChangeActionMode(ACTIONMODE::TRACKINGMODE);
		}
		//�e�������Ă��邩�̃t���O�Z�b�g
		jScript->SetParentAlive(parentAlive);
		if (!m_Player)return;
		//�퓬����������
		if (eap.actionMode == ACTIONMODE::BATTLEMODE) {
			//�ړ��������
			XMVECTOR battlePos;
			if (howManyPeople == 0) {
				moveToBattlePos = XMMatrixTranslationFromVector(XMVector3Normalize(j.enemyGameObject->mTransform->WorldPosition() - playerPos) * jScript->GetOnBattleRange());
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
			if (teamMember.size() == 1) {
				//�A�N�V�������I���������ߎ��̂����߂�
				if (eap.battleModeParameter.actionFinish) {
					if (eap.battleModeParameter.id == BATTLEACTION::CONFRONTACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(30, 30, 20, 20, 0, 5));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::APPROACHACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(30, 0, 40, 30, 0, 5));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::ATTACKDOWNACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 40, 20, 0, 0, 5));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::JUMPATTACKACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 20, 0, 30, 0, 5));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::GUARDACTION) {
						if (eap.battleModeParameter.beforeId == BATTLEACTION::BACKSTEPACTION) {
							jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(20, 30, 0, 30, 0, 5));
						}
						else {
							jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(20, 30, 20, 30, 0, 5));
						}
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::PROVOCATION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(30, 0, 40, 30, 0, 0));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::BACKSTEPACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(15, 0, 0, 0, 70, 15));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::WINCEACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 0, 40, 20, 0, 5));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::HITINGUARDACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 0, 40, 20, 0, 5));
					}
					else if (eap.battleModeParameter.id == BATTLEACTION::ATTACKMONCKEYACTION) {
						jScript->ChangeBattleAction(EnemyRezardMan::GetChangeBattleAction(40, 40, 20, 0, 0, 5));
					}
				}
			}
			else {
				//�U��
				if (j.enemyParameter.attack) {
					if (eap.battleModeParameter.actionFinish) {
						j.enemyParameter.attack = false;
						nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
						if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
						whoAttack++;
					}
				}
				//�����U��
				else if (j.enemyParameter.everyoneAttack) {
					//�N����l�ł����Ă�����ł��Ă��Ȃ�����ł��Ȃ�����
					if (everyoneAttack) {
						everyoneAttackCountFlag = false;
						nextAttackTimeCount = 0.0f;
					}
					//�I�������
					if (eap.battleModeParameter.id == BATTLEACTION::JUMPATTACKACTION) {
						j.enemyParameter.everyoneAttack = false;
						everyoneAttack = false;
						nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
						if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(nextAttackTime));
					}
					else {
						//�o�b�N�X�e�b�v���I�������
						if (eap.battleModeParameter.id == BATTLEACTION::BACKSTEPACTION) {
							if (eap.battleModeParameter.actionFinish) {
								jScript->ChangeBattleAction(BATTLEACTION::JUMPATTACKACTION);
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
					//�����̔Ԃ����U���ł��Ȃ��ꍇ
					if (eap.battleModeParameter.id != BATTLEACTION::APPROACHACTION &&
						eap.battleModeParameter.id != BATTLEACTION::GUARDACTION &&
						eap.battleModeParameter.id != BATTLEACTION::PROVOCATION &&
						eap.battleModeParameter.id != BATTLEACTION::CONFRONTACTION &&
						eap.battleModeParameter.id != BATTLEACTION::ATTACKDOWNACTION &&
						eap.battleModeParameter.id != BATTLEACTION::JUMPATTACKACTION &&
						whoAttack == howManyPeople) {
						whoAttack++;
					}

					//�S�����U��������
					if (whoAttack >= teamMember.size()) {
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
							jScript->ChangeBattleAction(BATTLEACTION::ATTACKDOWNACTION);
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTimeCount" + std::to_string(nextAttackTime));
						}
						else {
							j.enemyParameter.nextAttackTimeCountFlag = false;
							nextAttackTimeCount = 0.0f;
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
		howManyPeople++;
	}
	if (everyoneAttackCountFlag) {
		nextAttackTimeCount += Hx::DeltaTime()->GetDeltaTime();
	}
}
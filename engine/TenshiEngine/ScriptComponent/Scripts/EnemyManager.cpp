#include "EnemyManager.h"
#include "../h_standard.h"
#include "../h_component.h"

#include "Enemy.h"

//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Initialize(){

}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
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

//���t���[���Ă΂�܂�
void EnemyManager::Update(){
	//�G�̓��ꕨ��S����
	for (auto i = m_EnemyContainer.begin(); i != m_EnemyContainer.end();) {
		bool deleteTeam = false;
		//�`�[�����Ƃɉ񂵂Ă���
		for (auto j = i->enemyOneVec.begin(); j != i->enemyOneVec.end();) {
			//���񂾂�
			if (j->enemyGameObject.Get()->EndFinish()) {
				//�e�Ȃ�e���񂾃t���O�ɃZ�b�g
				if (!j->enemyParameter.child)i->enemyTeamParameter.parentAlive = false;
				//vector����폜
				j = i->enemyOneVec.erase(j);
				//�`�[���̐l�������炷
				i->enemyTeamParameter.teamCount -= 1;
				if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(i->enemyTeamParameter.teamID) + " : " + std::to_string(i->enemyTeamParameter.teamCount) + "�l");
				//�����`�[���l�����O�ɂȂ�����
				if (i->enemyOneVec.size() == 0) {
					if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(i->enemyTeamParameter.teamID) + " : " + "�`�[������ł���");
					//vector����폜
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
				//�����������ǂ���
				i->enemyTeamParameter.lostPlayer = false;
				if (!i->enemyTeamParameter.discoveryPlayer) {
					i->enemyTeamParameter.discoveryPlayer = jScript->DiscoveryPlayer();
				}
			}
			else {
				//�����������ǂ���
				i->enemyTeamParameter.discoveryPlayer = false;
				i->enemyTeamParameter.lostPlayer = jScript->LostPlayer();
			}

			j++;
		}
		//�`�[�����Ȃ���Ύ��̃`�[����
		if (deleteTeam)continue;

		auto playerPos = m_Player->mTransform->WorldPosition();
		//�퓬���̈ړ�����v�Z����̂Ɏg��(��3�S��)
		XMMATRIX moveToBattlePos;
		int howManyPeople = 0;
		float battlePosAngle = 3.14f / (float)(i->enemyTeamParameter.teamCount);
		for (auto j = i->enemyOneVec.begin(); j != i->enemyOneVec.end(); j++,howManyPeople++) {
			auto jScript = Enemy::GetEnemy(j->enemyGameObject);
			if (!jScript)return;
			auto eap = jScript->GetEnemyAllParameter(true);
			//����������퓬���[�h�ֈڍs
			if (i->enemyTeamParameter.discoveryPlayer && eap.actionMode != ACTIONMODE::BATTLEMODE) {
				jScript->ChangeActionAndBattleAction(ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
				i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
				if(m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
			}
			//����������{�����[�h�ֈڍs
			if (i->enemyTeamParameter.lostPlayer && eap.actionMode != ACTIONMODE::TRACKINGMODE) {
				jScript->ChangeActionMode(ACTIONMODE::TRACKINGMODE);
			}
			//�e�������Ă��邩�̃t���O�Z�b�g
			jScript->SetParentAlive(i->enemyTeamParameter.parentAlive);
			if (!m_Player)return;
			//�퓬����������
			if (eap.actionMode == ACTIONMODE::BATTLEMODE) {
				//�ړ��������
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
					//�A�N�V�������I���������ߎ��̂����߂�
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
					//�U��
					if (j->enemyParameter.attack) {
						if (eap.battleModeParameter.actionFinish) {
							j->enemyParameter.attack = false;
							i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
							i->enemyTeamParameter.whoAttack++;
						}
					}
					//�����U��
					else if (j->enemyParameter.everyoneAttack) {
						//�N����l�ł����Ă�����ł��Ă��Ȃ�����ł��Ȃ�����
						if (i->enemyTeamParameter.everyoneAttack) {
							i->enemyTeamParameter.everyoneAttackCountFlag = false;
							i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
						}
						//�I�������
						if (eap.battleModeParameter.id == BATTLEACTION::JUMPATTACKACTION) {
							j->enemyParameter.everyoneAttack = false;
							i->enemyTeamParameter.everyoneAttack = false;
							i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
						}
						else{
							//�o�b�N�X�e�b�v���I�������
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
					//�A�N�V�������I���������ߎ��̂����߂�
					else if (eap.battleModeParameter.actionFinish) {
						//�����̔Ԃ����U���ł��Ȃ��ꍇ
						if (eap.battleModeParameter.id != BATTLEACTION::APPROACHACTION &&
							eap.battleModeParameter.id != BATTLEACTION::GUARDACTION &&
							eap.battleModeParameter.id != BATTLEACTION::PROVOCATION &&
							eap.battleModeParameter.id != BATTLEACTION::CONFRONTACTION &&
							eap.battleModeParameter.id != BATTLEACTION::ATTACKDOWNACTION &&
							eap.battleModeParameter.id != BATTLEACTION::JUMPATTACKACTION &&
							i->enemyTeamParameter.whoAttack == howManyPeople) {
							i->enemyTeamParameter.whoAttack++;
						}
			
						//�S�����U��������
						if (i->enemyTeamParameter.whoAttack >= i->enemyTeamParameter.teamCount) {
							i->enemyTeamParameter.whoAttack = 0;
							i->enemyTeamParameter.everyoneAttackCountFlag = true;
						}
			
						if (!j->enemyParameter.attack && i->enemyTeamParameter.whoAttack == howManyPeople && !i->enemyTeamParameter.everyoneAttackCountFlag) {
							j->enemyParameter.nextAttackTimeCountFlag = true;
						}
						if (!j->enemyParameter.attack) {
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

//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void EnemyManager::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyManager::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void EnemyManager::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
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
		if (m_DrawFlag)Hx::Debug()->Log("Team" + std::to_string(et.enemyTeamParameter.teamID) + " : " + std::to_string(et.enemyTeamParameter.teamCount) + "�l");
		m_EnemyContainer.push_back(et);
	}
}
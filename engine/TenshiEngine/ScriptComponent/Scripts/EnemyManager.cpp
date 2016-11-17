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
			auto jScript = j.enemyGameObject->GetScript<Enemy>();
			if (!jScript)return;
			if (j.enemyParameter.nowActionMode == ACTIONMODE::TRACKINGMODE) {
				SetActionModeAndTrackingAction(&j,jScript,j.enemyParameter.nowActionMode,j.enemyParameter.trackingModeParameter.trackingActionID);
			}
			else {
				SetActionModeAndBattleAction(&j,jScript,j.enemyParameter.nowActionMode,j.enemyParameter.battleModeParameter.battleActionID);
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

			auto jScript = j->enemyGameObject->GetScript<Enemy>();
			if (!jScript)return;
			if (j->enemyParameter.nowActionMode == ACTIONMODE::TRACKINGMODE) {
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
			auto jScript = j->enemyGameObject->GetScript<Enemy>();
			if (!jScript)return;
			//����������퓬���[�h�ֈڍs
			if (i->enemyTeamParameter.discoveryPlayer && j->enemyParameter.nowActionMode != ACTIONMODE::BATTLEMODE) {
				SetActionModeAndBattleAction(&(*j), jScript,ACTIONMODE::BATTLEMODE, BATTLEACTION::CONFRONTACTION);
				i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
				if(m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
			}
			//����������{�����[�h�ֈڍs
			if (i->enemyTeamParameter.lostPlayer && j->enemyParameter.nowActionMode != ACTIONMODE::TRACKINGMODE) {
				SetActionMode(&(*j), jScript, ACTIONMODE::TRACKINGMODE);
			}
			//�e�������Ă��邩�̃t���O�Z�b�g
			jScript->SetParentAlive(i->enemyTeamParameter.parentAlive);
			if (!m_Player)return;

			//�퓬����������
			if (j->enemyParameter.nowActionMode == ACTIONMODE::BATTLEMODE) {
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

				//�G�̐퓬���̃p�����[�^���擾
				auto bmp = jScript->GetBattleModeParameter();

				if (i->enemyTeamParameter.teamCount == 1) {
					//�A�N�V�������I���������ߎ��̂����߂�
					if (bmp.actionFinish) {
						if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::CONFRONTACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(30, 30, 20, 20, 0, 5));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::APPROACHACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(30, 0, 40, 30, 0, 5));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::ATTACKDOWNACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(40, 40, 20, 0, 0, 5));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::JUMPATTACKACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(40, 20, 0, 30, 0, 5));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::GUARDACTION) {
							if (j->enemyParameter.battleModeParameter.beforeBattleActionID == BATTLEACTION::BACKSTEPACTION) {
								SetBattleAction(&(*j), jScript, ChangeBattleAction(20, 30, 0, 30, 0, 5));
							}
							else {
								SetBattleAction(&(*j), jScript, ChangeBattleAction(20, 30, 20, 30, 0, 5));
							}
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::PROVOCATION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(30, 0, 40, 30, 0, 0));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::BACKSTEPACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(15, 0, 0, 0, 70, 15));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::WINCEACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(40, 0, 40, 20, 0, 5));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::HITINGUARDACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(40, 0, 40, 20, 0, 5));
						}
						else if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::ATTACKMONCKEYACTION) {
							SetBattleAction(&(*j), jScript, ChangeBattleAction(40, 40, 20, 0, 0, 5));
						}
					}
				}
				else{
					//�U��
					if (j->enemyParameter.attack) {
						if (bmp.actionFinish) {
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
						if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::JUMPATTACKACTION) {
							j->enemyParameter.everyoneAttack = false;
							i->enemyTeamParameter.everyoneAttack = false;
							i->enemyTeamParameter.nextAttackTime = ((float)(rand() % (int)((m_NextAttackTimeMax - m_NextAttackTimeMin) * 100))) / 100.0f + m_NextAttackTimeMin;
							if (m_DrawFlag)Hx::Debug()->Log("nextAttackTime" + std::to_string(i->enemyTeamParameter.nextAttackTime));
						}
						else{
							//�o�b�N�X�e�b�v���I�������
							if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::BACKSTEPACTION) {
								if (bmp.actionFinish) {
									SetBattleAction(&(*j), jScript, BATTLEACTION::JUMPATTACKACTION);
								}
							}
							else {
								i->enemyTeamParameter.everyoneAttackCountFlag = false;
								i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
								SetBattleAction(&(*j), jScript, BATTLEACTION::BACKSTEPACTION);
							}
						}
					}
					//�A�N�V�������I���������ߎ��̂����߂�
					else if (bmp.actionFinish) {
						//�����̔Ԃ����U���ł��Ȃ��ꍇ
						if (j->enemyParameter.battleModeParameter.battleActionID != BATTLEACTION::APPROACHACTION &&
							j->enemyParameter.battleModeParameter.battleActionID != BATTLEACTION::GUARDACTION &&
							j->enemyParameter.battleModeParameter.battleActionID != BATTLEACTION::PROVOCATION &&
							j->enemyParameter.battleModeParameter.battleActionID != BATTLEACTION::CONFRONTACTION &&
							j->enemyParameter.battleModeParameter.battleActionID != BATTLEACTION::ATTACKDOWNACTION &&
							j->enemyParameter.battleModeParameter.battleActionID != BATTLEACTION::JUMPATTACKACTION &&
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
								SetBattleAction(&(*j), jScript, BATTLEACTION::APPROACHACTION);
								break;
							case 1:
								SetBattleAction(&(*j), jScript, BATTLEACTION::GUARDACTION);
								break;
							case 2:
								SetBattleAction(&(*j), jScript, BATTLEACTION::PROVOCATION);
								break;
							default:
								break;
							}
						}
					}
					else if (j->enemyParameter.nextAttackTimeCountFlag) {
						i->enemyTeamParameter.nextAttackTimeCount += Hx::DeltaTime()->GetDeltaTime();
						if (i->enemyTeamParameter.nextAttackTimeCount >= i->enemyTeamParameter.nextAttackTime) {
							if(i->enemyTeamParameter.nextAttackTimeCount < 5 && bmp.arrival && bmp.canChangeAttackAction) {
								j->enemyParameter.attack = true;
								j->enemyParameter.nextAttackTimeCountFlag = false;
								i->enemyTeamParameter.nextAttackTimeCount = 0.0f;
								SetBattleAction(&(*j), jScript, BATTLEACTION::ATTACKDOWNACTION);
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
							if (i->enemyTeamParameter.nextAttackTimeCount < 5 && bmp.canChangeAttackAction) {
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
		if (newEnemyTeam->GetScript<Enemy>()) {
			count++;
			eo.enemyGameObject = newEnemyTeam;
			eo.enemyParameter.child = false;
			eo.enemyParameter.nowActionMode = ACTIONMODE::TRACKINGMODE;
			eo.enemyParameter.trackingModeParameter.trackingActionID = TRACKINGACTION::PARENTTRACKING;
			et.enemyOneVec.insert(et.enemyOneVec.begin(), eo);
		}
		else {
			auto enemyTeamChild = newEnemyTeam->mTransform->Children();
			if (enemyTeamChild.size() == 0)return;

			for (auto i : enemyTeamChild) {
				count++;
				if (!i)return;
				auto iScript = i->GetScript<Enemy>();
				if (!iScript)return;
				eo.enemyGameObject = i;
				iScript->SetPlayer(m_Player);
				if (iScript->GetChildFlag()) {
					eo.enemyParameter.child = true;
					eo.enemyParameter.nowActionMode = ACTIONMODE::TRACKINGMODE;
					eo.enemyParameter.trackingModeParameter.trackingActionID = TRACKINGACTION::CHILDTRACKING;
					et.enemyOneVec.push_back(eo);
				}
				else {
					eo.enemyParameter.child = false;
					eo.enemyParameter.nowActionMode = ACTIONMODE::TRACKINGMODE;
					eo.enemyParameter.trackingModeParameter.trackingActionID = TRACKINGACTION::PARENTTRACKING;
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
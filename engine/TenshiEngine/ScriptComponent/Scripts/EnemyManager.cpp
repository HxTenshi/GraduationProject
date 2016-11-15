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

				//�������Ă��Ȃ����̃A�N�V�����������_���Őݒ�
				auto bmp = jScript->GetBattleModeParameter();

				if (i->enemyTeamParameter.everyoneAttacked) {
					if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::JUMPATTACKACTION) {
						i->enemyTeamParameter.everyoneAttacked = false;
					}

					if (i->enemyTeamParameter.everyoneAttacked) {
						if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::BACKSTEPACTION) {
							if (bmp.actionFinish) {
								SetBattleAction(&(*j), jScript, BATTLEACTION::JUMPATTACKACTION);
							}
						}
						else {
							SetBattleAction(&(*j), jScript, BATTLEACTION::BACKSTEPACTION);
						}
					}
				}
				else if (bmp.actionFinish) {
					if (j->enemyParameter.battleModeParameter.battleActionID == BATTLEACTION::ATTACKDOWNACTION) {
						i->enemyTeamParameter.whoAttack++;
					}

					if (i->enemyTeamParameter.whoAttack >= i->enemyTeamParameter.teamCount) {
						i->enemyTeamParameter.whoAttack = 0;
						i->enemyTeamParameter.everyoneAttacked = true;
					}

					if (bmp.arrival && i->enemyTeamParameter.whoAttack == howManyPeople) {
						SetBattleAction(&(*j), jScript, BATTLEACTION::ATTACKDOWNACTION);
					}
					else {
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
			}
			if (j->enemyParameter.child) {
				//�q���̏���

			}
			else {
				//�e���̏���
			}
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
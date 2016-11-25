
#pragma once
#include "main.h"
#include "EnemyManager.h"

struct EnemyParameter {
	//UŒ‚‚·‚é‚©
	bool attack = false;

	//UŒ‚‚ªˆêT‚µ‚½‚©
	bool everyoneAttack = false;

	//Ÿ‚ÌUŒ‚‚Ü‚Å‚ÌŠÔ‚ğƒJƒEƒ“ƒg‚·‚é‚©‚Ç‚¤‚©
	bool nextAttackTimeCountFlag = false;
};

struct EnemyOne {
	//ŒÂX‚Ì“G‚ÌgameObject
	GameObject enemyGameObject;

	//ŒÂX‚Ì“G‚Ìƒpƒ‰ƒ[ƒ^
	EnemyParameter enemyParameter;
};

class EnemyTeam :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	virtual void TeamInitialize() {}
	virtual bool Alive() { return false; };
	virtual void DiscoveryOrLostPlayerSet() {};
	virtual void TeamUpdate() {};


	static EnemyTeam* GetEnemyTeam(GameObject target);
private:
	//ƒƒ“ƒo•Ï”

protected:
	std::vector<EnemyOne> teamMember;
	bool lostPlayer;
	bool discoveryPlayer;
	//’N‚ªUŒ‚‚·‚é‚©
	int whoAttack;

	//Ÿ‚ÌUŒ‚‚Ü‚Å‚ÌŠÔ
	float nextAttackTime;

	//UŒ‚‚ªˆêT‚µ‚½‚©
	bool everyoneAttack;

	//Ÿ‚ÌUŒ‚‚Ü‚Å‚ÌŠÔ‚ğƒJƒEƒ“ƒg‚·‚é
	float nextAttackTimeCount;

	//ˆêÄUŒ‚‚Ü‚Å‚ÌŠÔ‚ğƒJƒEƒ“ƒg‚·‚é‚©‚Ç‚¤‚©
	bool everyoneAttackCountFlag;
	//e‚ª¶‚«‚Ä‚¢‚é‚©‚Ç‚¤‚©
	bool parentAlive;
};
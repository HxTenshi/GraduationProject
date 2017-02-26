
#pragma once
#include "main.h"


class GetEnemy :public IDllScriptComponent{
public:
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	struct MinVect{
		enum Enum {
			left,
			right,
		};
	};

	GameObject GetMinEnemy();
	GameObject GetPointMinEnemy(GameObject currentTarget, MinVect::Enum minVect);

private:
	//ƒƒ“ƒo•Ï

	std::list<GameObject> m_EnemyList;
};
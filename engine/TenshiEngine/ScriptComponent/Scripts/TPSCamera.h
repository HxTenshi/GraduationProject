
#pragma once
#include "main.h"

class CharacterControllerComponent;
class TPSCamera :public IDllScriptComponent{
public:
	TPSCamera();
	void Initialize()override;
	void Start()override;
	void Update()override;
	void Finish()override;
	void OnCollideBegin(GameObject target)override;
	void OnCollideEnter(GameObject target)override;
	void OnCollideExit(GameObject target)override;

	float GetLeft() { return mLeft; }
	void SetLeft(float f) { mLeft = f; }

	float GetUp() { return mUp; }
	void SetUp(float f) { mUp = f; }

	float GetDistance() { return mDistance; }
	void SetDistance(float f) { mDistance = f; }

	float GetLookSpeed() { return mLookSpeed; }
	void SetLookSpeed(float f) { mLookSpeed = f; }

	GameObject GetTarget() { return mTarget; }
	void SetTarget(GameObject target) { mTarget = target; }

	GameObject GetLookTarget() { return mLookTarget; }
	void SetLookTarget(GameObject target) { mLookTarget = target; }

	GameObject GetSaveEnemy() { return mSaveEnemy; }
	void SetSaveEnemy(GameObject saveTarget) { mSaveEnemy = saveTarget; }

	float GetSpringStiffness() { return m_SpringStiffness; }
	void SetSpringStiffness(float f) { m_SpringStiffness = f; }

	float GetSpringDamping() { return m_SpringDamping; }
	void SetSpringDamping(float f) { m_SpringDamping = f; }

private:

	XMVECTOR spring(XMVECTOR position, const XMVECTOR& desiredPositio);


	//メンバ変数
	SERIALIZE
	float mDistance;
	SERIALIZE
	float mUp;
	SERIALIZE
	float mLeft;
	SERIALIZE
	float mLookSpeed;
	SERIALIZE
	GameObject mTarget;
	SERIALIZE
	GameObject mLookTarget;
	GameObject mSaveEnemy;	//ターゲットの敵を保存するよう

	SERIALIZE
	float m_SpringStiffness;
	SERIALIZE
	float m_SpringDamping;

	XMFLOAT2 mRotate;
	XMVECTOR m_SpringVelocity;
	weak_ptr<CharacterControllerComponent> m_CharacterControllerComponent;
};
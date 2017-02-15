#include "reis.h"

#include "UniqueObject.h"

#include "PlayerController.h"

#include "h_standard.h"
#include "h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

#include "ReisCitrusBullet.h"

#include "ReisSonicWave.h"


struct ReisMode {
	enum Enum {
		Idle,
		Move,
		Attack_Bullet,
		Attack_CitrusBullet,
		Attack_SonicWaveV,
		Attack_WarpMelee,
		Attack_v1,
		Attack_v2,
		Attack_v3,
		Count,
	};
};
struct AttackIf
{
	AttackIf(ReisMode::Enum _mode,bool f = true) {
		enable = f;
		mode = _mode;
	}
	bool enable;
	ReisMode::Enum mode;
};

const int melee_num = 3;
const AttackIf mode_melee[] = { 
	AttackIf(ReisMode::Attack_v1),
	AttackIf(ReisMode::Attack_v2),
	AttackIf(ReisMode::Attack_v3),
};
const int mid_num = 4;
const AttackIf mode_mid[] = { 
	AttackIf(ReisMode::Attack_Bullet),
	AttackIf(ReisMode::Attack_CitrusBullet),
	AttackIf(ReisMode::Attack_SonicWaveV),
	AttackIf(ReisMode::Attack_WarpMelee)
};

struct AnimeID {
	enum Enum {
		Idle,
		Move,
		Attack_v1,
		Attack_Bullet,
		Attack_SonicWaveV,
		Attack_WarpMelee,
		Attack_v2,
		Attack_v3,
		Attack_Ch_v1,
		Attack_Ch_v2,
		Attack_Ch_v3,
	};
};


//�������ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void reis::Initialize(){
	m_Player = NULL;
	m_ReisMode = ReisMode::Idle;
	m_CitrusBulletObject = NULL;
	m_CitrusBulletCount = 0;
	m_CitrusBulletCollDown = 0.0f;
	m_ReisLastAttackMode = 0;
	m_SonicWaveObject = NULL;

	m_BoneBackPos = XMVectorZero();

	m_MoveFunc.resize(ReisMode::Count);
}

//initialize��update�̑O�ɌĂ΂�܂��i�G�f�B�^�[�����Ă΂�܂��j
void reis::Start(){

	m_Player = UniqueObject::GetPlayer();

	m_MoveFunc[ReisMode::Idle] = [&](){
		ChangeAnime(AnimeID::Idle);
		m_ReisMode = ReisMode::Move;
	};
	m_MoveFunc[ReisMode::Move] = [&]() {
		ChangeAnime(AnimeID::Move);
		auto v = GetPlayerVect() * m_MoveSpeed * Hx::DeltaTime()->GetDeltaTime();

		Move(v);
		Rotate(GetPlayerVectH());


		{
			for (int i = 0;i<100;i++) {
				//auto r = rand() / (float)RAND_MAX;
				//auto numPar = 1.0f / 4.0f;
				//if (r < numPar) {
				//	m_ReisMode = ReisMode::Attack_CitrusBullet;
				//}
				//else if (r < numPar * 2) {
				//	m_ReisMode = ReisMode::Attack_Bullet;
				//}
				//else if (r<numPar * 3) {
				//	m_ReisMode = ReisMode::Attack_SonicWaveV;
				//}
				//else {
				//	m_ReisMode = ReisMode::Attack_WarpMelee;
				//}

				bool f = false;
				if (GetPlayerLen() < 2.0f) {
					int atk = rand() % melee_num;
					m_ReisMode = mode_melee[atk].mode;
					f = mode_melee[atk].enable;
				}
				if (GetPlayerLen() > 10.0f) {
					int atk = rand() % mid_num;
					m_ReisMode = mode_mid[atk].mode;
					f = mode_mid[atk].enable;
				}

				if (m_ReisLastAttackMode == m_ReisMode || !f) {
					m_ReisMode = ReisMode::Move;
					continue;
				}

				//m_ReisMode = ReisMode::Attack_v1;
				m_ReisLastAttackMode = m_ReisMode;
				break;
			}
		}
	};
	m_MoveFunc[ReisMode::Attack_v1] = [&]() {
		//if (m_AttackStage.stage == 0) {
		//	ChangeAnime(AnimeID::Attack_Ch_v1);
		//	if (IsCurrentAnimeEnd()) {
		//		m_AttackStage.stage++;
		//	}
		//}
		//else if(m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::Attack_v1);
			if (IsCurrentAnimeEnd()) {
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
			}
		//}
		
	};
	m_MoveFunc[ReisMode::Attack_v2] = [&]() {
		if (m_AttackStage.stage == 0) {
			ChangeAnime(AnimeID::Attack_Ch_v2);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
			}
		}
		else if (m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::Attack_v2);
			if (IsCurrentAnimeEnd()) {
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
			}
		}
	};
	m_MoveFunc[ReisMode::Attack_v3] = [&]() {
		if (m_AttackStage.stage == 0) {
			ChangeAnime(AnimeID::Attack_Ch_v3);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
			}
		}
		else if (m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::Attack_v3);
			if (IsCurrentAnimeEnd()) {
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
			}
		}
	};
	m_MoveFunc[ReisMode::Attack_Bullet] = [&]() {
		Rotate(GetPlayerVectH());
		if (m_CurrentAnimeID != AnimeID::Attack_Bullet) {
			if (auto obj = Hx::Instance(m_Bullets)) {
				obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
				obj->mTransform->WorldQuaternion(gameObject->mTransform->WorldQuaternion());
			}
		}

		ChangeAnime(AnimeID::Attack_Bullet);
		if (IsCurrentAnimeEnd()) {
			m_ReisMode = ReisMode::Idle;
		}
	};
	m_MoveFunc[ReisMode::Attack_CitrusBullet] = [&]() {
		Rotate(GetPlayerVectH());
		m_CitrusBulletCollDown = 5.0f;
		if (m_CurrentAnimeID != AnimeID::Attack_Bullet) {
			if (m_CitrusBulletObject = Hx::Instance(m_CitrusBullet)) {
				auto pos = gameObject->mTransform->WorldPosition();
				pos.y += 1.0f;
				m_CitrusBulletObject->mTransform->WorldPosition(pos);
				if (auto scr = m_CitrusBulletObject->GetScript<ReisCitrusBullet>()) {
					scr->SetVector(GetPlayerVectH());
				}
			}
		}

		ChangeAnime(AnimeID::Attack_Bullet);
		if (IsCurrentAnimeEnd()) {

			if (m_CitrusBulletObject) {
				auto pos = m_CitrusBulletObject->mTransform->WorldPosition();
				pos.y += 3.0f;
				Teleport(pos);
				Move(XMVectorSet(0.0f, -5.0f, 0.0f, 1.0f));

				Hx::DestroyObject(m_CitrusBulletObject);
				m_CitrusBulletObject = NULL;
			}

			if (m_CitrusBulletCount >= 2) {
				m_CitrusBulletCount = 0;
				m_ReisMode = ReisMode::Idle;
			}
			else {
				ChangeAnime(AnimeID::Idle);
				m_CitrusBulletCount++;
			}
		}
	};
	m_MoveFunc[ReisMode::Attack_SonicWaveV] = [&]() {
		Rotate(GetPlayerVectH());

		if (m_CurrentAnimeID != AnimeID::Attack_SonicWaveV) {
			WeaponEffect(0, true);
			m_SonicWaveObject = Hx::Instance(m_SonicWaveV);
			m_SonicWaveObject->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
		}

		if (m_Player) {
			auto pc = m_Player->GetScript<PlayerController>();
			if (!(pc->GetPlayerState() == PlayerController::PlayerState::Dodge)) {
				if (m_SonicWaveObject) {
					auto wave = m_SonicWaveObject->GetScript<ReisSonicWave>();
					wave->PlayerLockOn();
				}
			}
		}

		ChangeAnime(AnimeID::Attack_SonicWaveV);
		if (IsCurrentAnimeEnd()) {
			m_SonicWaveObject = NULL;
			m_ReisMode = ReisMode::Idle;

			WeaponEffect(0, false);
		}
	};
	m_MoveFunc[ReisMode::Attack_WarpMelee] = [&]() {
		if (m_CurrentAnimeID != AnimeID::Attack_WarpMelee) {
			if (m_Player) {
				auto pos = m_Player->mTransform->WorldPosition();

				auto agnel = rand() / (float)RAND_MAX * XM_2PI;
				auto add = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), XMMatrixRotationY(agnel));

				pos += add * 2.0f;
				pos.y += 3.0f;
				Teleport(pos);
				Move(XMVectorSet(0.0f, -5.0f, 0.0f, 1.0f));
			}
		}
		Rotate(GetPlayerVectH());

		ChangeAnime(AnimeID::Attack_WarpMelee);
		if (IsCurrentAnimeEnd()) {
			m_ReisMode = ReisMode::Idle;
		}
	};
}

//���t���[���Ă΂�܂�
void reis::Update(){

	m_CitrusBulletCollDown -= Hx::DeltaTime()->GetDeltaTime();

	m_MoveFunc[m_ReisMode]();

	BoneMoveUpdate();
}


//�J�����ɌĂ΂�܂��iInitialize�P��ɑ΂���Finish�P��Ă΂�܂��j�i�G�f�B�^�[�����Ă΂�܂��j
void reis::Finish(){

}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void reis::OnCollideBegin(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃q�b�g���ɌĂ΂�܂�
void reis::OnCollideEnter(GameObject target){
	(void)target;
}

//�R���C�_�[�Ƃ̃��X�g���ɌĂ΂�܂�
void reis::OnCollideExit(GameObject target){
	(void)target;
}


void reis::Teleport(const XMVECTOR & vect)
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	cc->Teleport(vect);
}

void reis::Move(const XMVECTOR & vect)
{
	auto cc = gameObject->GetComponent<CharacterControllerComponent>();
	if (!cc)return;

	cc->Move(vect);
}

void reis::Rotate(const XMVECTOR & vect)
{
	float m_RotateLimit = 360.0f*3;
	if (XMVector3Length(vect).x == 0)return;
	auto rotate = XMMatrixTranspose(XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 1), vect, XMVectorSet(0, 1, 0, 1)));
	auto q = XMQuaternionRotationMatrix(rotate);

	auto f = gameObject->mTransform->Forward();
	float angle = acos(XMVector3Dot(f, vect).x);
	float limit = m_RotateLimit / 180.0f * XM_PI * Hx::DeltaTime()->GetDeltaTime();
	float t = 1.0f;
	if (limit < angle) {
		t = limit / angle;
	}
	auto wq = gameObject->mTransform->WorldQuaternion();
	q = XMQuaternionSlerp(wq, q, t);
	gameObject->mTransform->WorldQuaternion(q);
}

XMVECTOR reis::GetPlayerVect()
{
	if (!m_Player)return XMVectorZero();
	auto v = m_Player->mTransform->WorldPosition()- gameObject->mTransform->WorldPosition();
	if (XMVector3Length(v).x == 0.0f)return XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	return XMVector3Normalize(v);
}
XMVECTOR reis::GetPlayerVectH()
{
	if (!m_Player)return XMVectorZero();
	auto v = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
	v.y = 0.0f;
	if (XMVector3Length(v).x == 0.0f)return XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	return XMVector3Normalize(v);
}

float reis::GetPlayerLen()
{
	if (!m_Player)return 0.0f;
	return XMVector3Length(m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition()).x;
}

float reis::GetPlayerLenH()
{
	if (!m_Player)return 0.0f;
	auto v1 = m_Player->mTransform->WorldPosition();
	auto v2 = gameObject->mTransform->WorldPosition();
	v1.y = 0.0f;
	v2.y = 0.0f;
	return XMVector3Length(v1 - v2).x;
}

float reis::GetPlayerLenV()
{
	if (!m_Player)return 0.0f;
	return abs(m_Player->mTransform->WorldPosition().y - gameObject->mTransform->WorldPosition().y);
}

void reis::ChangeAnime(int id)
{

	if (m_CurrentAnimeID == id) {
		return;
	}

	if (!m_AnimeModel)return;
	auto anime = m_AnimeModel->GetComponent<AnimationComponent>();
	if (!anime)return;

	if (m_CurrentAnimeID >= 0) {
		auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
		p.mWeight = 0.0f;
		p.mTime = 0.0f;
		//p.mTimeScale = 0.0f;
		anime->SetAnimetionParam(m_CurrentAnimeID, p);
	}
	m_CurrentAnimeID = id;
	auto p = anime->GetAnimetionParam(m_CurrentAnimeID);
	p.mTime = 0.0f;
	p.mWeight = 1.0f;
	//p.mTimeScale = 1.0f;
	anime->SetAnimetionParam(m_CurrentAnimeID, p);
}

bool reis::IsCurrentAnimeEnd()
{
	if (!m_AnimeModel)return false;
	if (auto anime = m_AnimeModel->GetComponent<AnimationComponent>()) {
		if (anime->IsAnimationEnd(m_CurrentAnimeID)) {
			return true;
		}
	}
	return false;
}

void reis::WeaponEffect(int id, bool enable)
{
	if (!m_WeaponEffect)return;
	if (auto com = m_WeaponEffect->GetComponent<ParticleComponent>()) {
		if (enable) {
			com->mParticleParam.Param.x = com->mParticleCapacity;
		}
		else {
			com->mParticleParam.Param.x = 0;
		}
	}
}

void reis::BoneMoveUpdate()
{
	//if (m_AnimeModel&&m_BoneMirrorObject) {
	//	auto model = m_AnimeModel->mTransform->WorldPosition();
	//	auto bone = m_BoneMirrorObject->mTransform->WorldPosition();
	//
	//	bool flag = false;
	//	if (XMVector3Length(m_BoneMirrorObject->mTransform->Position()).x > 0.01f) {
	//		flag = true;
	//	}
	//	bone -= model;
	//	bone.y = 0.0f;
	//	m_AnimeModel->mTransform->Position(-bone);
	//	if (flag) {
	//		auto move = bone - m_BoneBackPos;
	//
	//		//Move(move);
	//	}
	//
	//	m_BoneBackPos = bone;
	//}
}

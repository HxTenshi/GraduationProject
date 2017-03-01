#include "reis.h"

#include "UniqueObject.h"

#include "PlayerController.h"

#include "h_standard.h"
#include "h_component.h"
#include "Game/Component/CharacterControllerComponent.h"

#include "ReisCitrusBullet.h"

#include "ReisSonicWave.h"

#include "Enemy.h"

#include "Score.h"
#include "SoundManager.h"


struct ReisMode {
	enum Enum {
		Standby,
		Pop,
		Idle,
		Move,
		Wince,
		Dogde,
		Dead,
		Counter,
		Attack_Bullet,
		Attack_CitrusBullet,
		Attack_SonicWaveV,
		Attack_WarpMelee,
		Attack_v1,
		Attack_v2,
		Attack_v3,
		Attack_Counter,
		Count,
	};
};

struct AttackIf
{
	AttackIf(ReisMode::Enum _mode, reis::MoveMode::Enum _mmode = reis::MoveMode::Idle,float time = 0.0f,bool f = true) {
		enable = f;
		mode = _mode;
		moveMode = _mmode;
		cooldown = time;
	}
	bool enable;
	float cooldown;
	reis::MoveMode::Enum moveMode;
	ReisMode::Enum mode;
};

const int melee_num = 3;
const AttackIf mode_melee[] = { 
	AttackIf(ReisMode::Attack_v1,reis::MoveMode::Out,0.5f),
	AttackIf(ReisMode::Attack_v2,reis::MoveMode::Out,0.5f),
	AttackIf(ReisMode::Attack_v3,reis::MoveMode::Out,0.5f),
};
const int mid_num = 4;
const AttackIf mode_mid[] = { 
	AttackIf(ReisMode::Attack_Bullet		,reis::MoveMode::In		,0.5f),
	AttackIf(ReisMode::Attack_CitrusBullet	,reis::MoveMode::In		,1.0f),
	AttackIf(ReisMode::Attack_SonicWaveV	,reis::MoveMode::Idle	,0.5f),
	AttackIf(ReisMode::Attack_WarpMelee		,reis::MoveMode::Out	,0.5f)
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
		Wince,
		Dead,
		Dogde,
		WarpOut,
		WarpIn,
		Pop,
	};
};


//生成時に呼ばれます（エディター中も呼ばれます）
void reis::Initialize(){
	m_CurrentAnimeID = -1;
	m_Player = NULL;
	m_ReisMode = ReisMode::Idle;
	m_CitrusBulletObject = NULL;
	m_ReisLastAttackMode = 0;
	m_SonicWaveObject = NULL;
	m_SuperArmor = false;
	m_SuperArmorHit = 0;
	m_MoveCooldown = 0.0f;
	if (_m_DefaultStandby) {
		m_ReisMode = ReisMode::Standby;
	}
	else {
		m_ReisMode = ReisMode::Idle;
	}
	m_MoveMode = MoveMode::Idle;
	m_DogdeMode = DogdeMode::Back;
	m_WinceCount = 0;
	m_AutoDogdeMode = false;

	m_DodgeBaseVect = XMVectorZero();
	m_BoneBackPos = XMVectorZero();

	m_MoveFunc.resize(ReisMode::Count);
}

//initializeとupdateの前に呼ばれます（エディター中も呼ばれます）
void reis::Start(){

	Score::AddCountEnemy();
	m_Player = UniqueObject::GetPlayer();
	m_MoveFunc[ReisMode::Standby] = [&]() {
		ChangeAnime(AnimeID::Idle);
	};
	m_MoveFunc[ReisMode::Pop] = [&]() {
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;

			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Warp, gameObject->mTransform->WorldPosition());
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Pop, gameObject->mTransform->WorldPosition());
		}
		ChangeAnime(AnimeID::Pop);
		if (IsCurrentAnimeEnd()) {
			m_ReisMode = ReisMode::Idle;
			m_AttackStage = AttackStage();
		}
	};
	m_MoveFunc[ReisMode::Idle] = [&](){
		ChangeAnime(AnimeID::Idle);
		m_ReisMode = ReisMode::Move;
	};
	m_MoveFunc[ReisMode::Move] = [&]() {
		auto v = XMVectorZero();

		if (MoveMode::Idle == m_MoveMode) {
			ChangeAnime(AnimeID::Idle);
			if (m_MoveCooldown <= 0.0f) {
				m_MoveMode = MoveMode::In;
			}
		}else if (MoveMode::In == m_MoveMode) {
			ChangeAnime(AnimeID::Move);
			v = GetPlayerVect() * m_MoveSpeed * Hx::DeltaTime()->GetDeltaTime();

		}else if (MoveMode::Out == m_MoveMode) {
			ChangeAnime(AnimeID::Move);
			v = -GetPlayerVect() * m_MoveSpeed * Hx::DeltaTime()->GetDeltaTime();
		}

		Move(v);
		Move(XMVectorSet(0.0f, -9.81f, 0.0f, 1.0f)*Hx::DeltaTime()->GetDeltaTime());
		Rotate(GetPlayerVectH());

		m_MoveCooldown -= Hx::DeltaTime()->GetDeltaTime();

		if(m_MoveCooldown<=0.0f){
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
				if (GetPlayerLen() <= 3.0f) {
					int atk = rand() % melee_num;
					m_ReisMode = mode_melee[atk].mode;
					f = mode_melee[atk].enable;
					m_MoveMode = mode_melee[atk].moveMode;
					m_MoveCooldown = mode_melee[atk].cooldown;
				}
				if (GetPlayerLen() > 7.0f) {
					int atk = rand() % mid_num;
					m_ReisMode = mode_mid[atk].mode;
					f = mode_mid[atk].enable;
					m_MoveMode = mode_mid[atk].moveMode;
					m_MoveCooldown = mode_mid[atk].cooldown;
				}
				bool f2 = false;//m_ReisLastAttackMode == m_ReisMode;
				if (f2 || !f) {
					m_ReisMode = ReisMode::Move;
					continue;
				}

				//m_ReisMode = ReisMode::Attack_v1;
				m_ReisLastAttackMode = m_ReisMode;
				break;
			}
		}
	};
	m_MoveFunc[ReisMode::Wince] = [&]() {

		if (m_CurrentAnimeID != AnimeID::Wince) {
			m_WinceCount++;
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Damage, gameObject->mTransform->WorldPosition());
		}
		m_AttackStage = AttackStage();
		ChangeAnime(AnimeID::Wince);

		if (m_WinceCount > 8) {
			m_ReisMode = ReisMode::Dogde;
			m_DogdeMode = DogdeMode::Back;
			m_WinceCount = 0;
		}
		if (IsCurrentAnimeEnd()) {
			m_ReisMode = ReisMode::Idle;
			m_WinceCount = 0;
		}
	};
	m_MoveFunc[ReisMode::Dogde] = [&]() {
		m_AutoDogdeMode = false;
		SetSuperArmor(true);
		if (m_CurrentAnimeID != AnimeID::Dogde) {
			m_AttackStage = AttackStage();
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_BackStep, gameObject->mTransform->WorldPosition());
		}

		ChangeAnime(AnimeID::Dogde);

		if (m_DogdeMode == DogdeMode::Back) {
			if (m_AttackStage.stage == 0) {
				m_DodgeBaseVect = -GetPlayerVectH() * 12.0f;
			}
			m_AttackStage.stage += Hx::DeltaTime()->GetDeltaTime()*1000.0f * 7;
			if (m_AttackStage.stage <= 1*2400) {
				Move(m_DodgeBaseVect * Hx::DeltaTime()->GetDeltaTime());
				Move(XMVectorSet(0.0f, -4.0f, 0.0f, 1.0f));
			}
			if (IsCurrentAnimeEnd()) {
				SetSuperArmor(false);
				m_AttackStage = AttackStage();
				m_ReisMode = ReisMode::Idle;
			}
		}
		else if (m_DogdeMode == DogdeMode::Rotate){
			Rotate(GetPlayerVectH());
			if (m_AttackStage.stage == 0) {

				m_DodgeBaseVect = GetPlayerVectH()*-GetPlayerLenH();
			}
			m_AttackStage.stage += (int)(Hx::DeltaTime()->GetDeltaTime()*1000.0f) * 7;
			if (m_AttackStage.stage <= 1 * 2400) {
				auto ppos = m_Player->mTransform->WorldPosition();
				float t = m_AttackStage.stage / 2400.0f;
				auto q = XMQuaternionRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), t*XM_PI);
				auto addpos = XMVector3Rotate(m_DodgeBaseVect, q);

				auto tpos = ppos + addpos;
				tpos.y += 4.0f;
				Teleport(tpos);
				Move(XMVectorSet(0.0f, -4.0f, 0.0f, 1.0f));
			}
			if (IsCurrentAnimeEnd()) {
				SetSuperArmor(false);
				m_AttackStage = AttackStage();
				m_ReisMode = ReisMode::Attack_Counter;
			}
		}



	};
	m_MoveFunc[ReisMode::Dead] = [&]() {
		if(m_CurrentAnimeID != AnimeID::Dead){
			m_AttackStage = AttackStage();
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Dead, gameObject->mTransform->WorldPosition());
		}
		ChangeAnime(AnimeID::Dead);
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;
			Score::AddScore();
			if (auto obj = Hx::Instance(m_DeadParticle)) {
				obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
			}
			gameObject->RemoveComponent<CharacterControllerComponent>();
		}
		else if (m_AttackStage.stage == 1) {
			if (IsCurrentAnimeEnd()) {
				Hx::DestroyObject(gameObject);
			}
		}
	};
	
	m_MoveFunc[ReisMode::Attack_v1] = [&]() {
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Near1, gameObject->mTransform->WorldPosition());
		}
		if (m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::Attack_Ch_v1);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
				m_AutoDogdeMode = false;
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Near_Swing, gameObject->mTransform->WorldPosition());
			}
		}
		else if(m_AttackStage.stage == 2) {
			SetWeapon(true, 10.0f);
			ChangeAnime(AnimeID::Attack_v1);
			if (IsCurrentAnimeEnd()) {
				SetWeapon(false);
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
			}
		}
		
	};
	m_MoveFunc[ReisMode::Attack_v2] = [&]() {

		Rotate(GetPlayerVectH());
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;
			m_SuperArmorHit = 0;
			SetSuperArmor(true);

			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Chage1, gameObject->mTransform->WorldPosition());
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Chage1, gameObject->mTransform->WorldPosition());

		}
		if (m_AttackStage.stage == 1) {
			auto backt = m_AttackStage.time;
			m_AttackStage.time += Hx::DeltaTime()->GetDeltaTime();
			ChangeAnime(AnimeID::Attack_Ch_v2);
			//const float tt = 40.0f / 30.0f * 2.0f;
			const float t[] = {0.0f,1.0f, 1.5f, 1.75f, 1.9f, 2.0f,2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.5f, 2.6f, 2.6f, 2.7f, 2.7f, 2.7f, 2.75f, 2.75f};
			for (int i = 0; i < 19; i++) {
				if (backt < t[i]&&m_AttackStage.time >= t[i]) {
					if (auto obj = Hx::Instance(m_ChargeParticle)) {
						auto p = gameObject->mTransform->WorldPosition();
						p.y += 1.0f;
						obj->mTransform->WorldPosition(p);
					}
				}
			}

			if (IsCurrentAnimeEnd()) {
				if (auto obj = Hx::Instance(m_BigBangParticle)) {
					auto p = gameObject->mTransform->WorldPosition();
					p.y += 1.0f;
					obj->mTransform->WorldPosition(p);
				}

				SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Chage2, gameObject->mTransform->WorldPosition());
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Chage2, gameObject->mTransform->WorldPosition());
				m_AttackStage.stage++;
			}
		}
		else if (m_AttackStage.stage == 2) {
			SetWeapon(true, 30.0f);
			ChangeAnime(AnimeID::Attack_v2);
			if (IsCurrentAnimeEnd()) {
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
				SetSuperArmor(false);
				SetWeapon(false);
			}
		}
		if (m_SuperArmorHit >= 4) {
			SetSuperArmor(false);
			SetWeapon(false);
		}
	};
	m_MoveFunc[ReisMode::Attack_v3] = [&]() {
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Near2, gameObject->mTransform->WorldPosition());
		}
		if (m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::Attack_Ch_v3);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
				m_AutoDogdeMode = false;
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Near_Swing, gameObject->mTransform->WorldPosition());
			}
		}
		else if (m_AttackStage.stage == 2) {
			SetWeapon(true, 10.0f);
			ChangeAnime(AnimeID::Attack_v3);
			if (IsCurrentAnimeEnd()) {
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
				SetWeapon(false);
			}
		}
	};
	m_MoveFunc[ReisMode::Attack_Counter] = [&]() {
		m_AutoDogdeMode = false;
		Rotate(GetPlayerVectH());
		SetSuperArmor(true);
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Near_Swing, gameObject->mTransform->WorldPosition());
		}
		if (m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::Attack_v1);
			SetWeapon(true, 10.0f);
			if (IsCurrentAnimeEnd()) {
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
				SetWeapon(false);
				SetSuperArmor(false);
			}
		}
	};
	m_MoveFunc[ReisMode::Attack_Bullet] = [&]() {
		Rotate(GetPlayerVectH());
		if (m_CurrentAnimeID != AnimeID::Attack_Bullet) {
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_Shot, gameObject->mTransform->WorldPosition());
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
		SetSuperArmor(true);
		Rotate(GetPlayerVectH());


		int step = 4;
		if (m_AttackStage.stage %step == 0) {
			ChangeAnime(AnimeID::Attack_Bullet);

			m_AttackStage.time += Hx::DeltaTime()->GetDeltaTime();
			if (m_AttackStage.time >= 0.5f) {
				if (m_CitrusBulletObject = Hx::Instance(m_CitrusBullet)) {
					SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_CitrusThrow, gameObject->mTransform->WorldPosition());
					auto pos = gameObject->mTransform->WorldPosition();
					pos.y += 1.0f;
					m_CitrusBulletObject->mTransform->WorldPosition(pos);
					if (auto scr = m_CitrusBulletObject->GetScript<ReisCitrusBullet>()) {
						scr->SetVector(GetPlayerVectH());
					}

					if (m_Weapon) {
						m_Weapon->Disable();
					}
				}
				m_AttackStage.stage++;
				m_AttackStage.time = 0.0f;
			}

		}
		else if (m_AttackStage.stage%step == 1) {
			ChangeAnime(AnimeID::Attack_Bullet);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
			}
		}else if (m_AttackStage.stage%step == 2) {
			ChangeAnime(AnimeID::WarpOut);
			if (IsCurrentAnimeEnd()) {
				if (auto obj = Hx::Instance(m_WarpParticle)) {
					obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
				}
				if (m_CitrusBulletObject) {
					auto pos = m_CitrusBulletObject->mTransform->WorldPosition();
					pos.y += 3.0f;
					Teleport(pos);
					Move(XMVectorSet(0.0f, -5.0f, 0.0f, 1.0f));
					SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Warp, gameObject->mTransform->WorldPosition());

					Hx::DestroyObject(m_CitrusBulletObject);
					m_CitrusBulletObject = NULL;

					if (m_Weapon) {
						m_Weapon->Enable();
					}
				}
				if (auto obj = Hx::Instance(m_WarpParticle)) {
					obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
				}
				m_AttackStage.stage++;
			}
		}
		else if (m_AttackStage.stage%step == 3) {
			ChangeAnime(AnimeID::WarpIn);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
			}
		}

		if (IsCurrentAnimeEnd() && m_AttackStage.stage == step*3) {
			ChangeAnime(AnimeID::Idle);
			m_ReisMode = ReisMode::Idle;
			SetSuperArmor(false);
			m_AttackStage = AttackStage();
		}
	};
	m_MoveFunc[ReisMode::Attack_SonicWaveV] = [&]() {
		Rotate(GetPlayerVectH());

		if (m_CurrentAnimeID != AnimeID::Attack_SonicWaveV) {
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SonicWave1, gameObject->mTransform->WorldPosition());
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
		Rotate(GetPlayerVectH());
		SetSuperArmor(true);
		if (m_AttackStage.stage == 0) {
			m_AttackStage.stage++;
			SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_WarpNear, gameObject->mTransform->WorldPosition());
		}
		if (m_AttackStage.stage == 1) {
			ChangeAnime(AnimeID::WarpOut);
			if (IsCurrentAnimeEnd()) {
				if (auto obj = Hx::Instance(m_WarpParticle)) {
					obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
				}
				if (m_Player) {
					auto pos = m_Player->mTransform->WorldPosition();

					auto agnel = rand() / (float)RAND_MAX * XM_2PI;
					auto add = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), XMMatrixRotationY(agnel));

					pos += add * 2.0f;
					pos.y += 3.0f;
					Teleport(pos);
					Move(XMVectorSet(0.0f, -5.0f, 0.0f, 1.0f));
					SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Warp, gameObject->mTransform->WorldPosition());
				}
				if (auto obj = Hx::Instance(m_WarpParticle)) {
					obj->mTransform->WorldPosition(gameObject->mTransform->WorldPosition());
				}
				m_AttackStage.stage++;
			}
		}
		else if (m_AttackStage.stage == 2) {
			ChangeAnime(AnimeID::WarpIn);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
			}
		}
		else if (m_AttackStage.stage == 3) {
			ChangeAnime(AnimeID::Attack_Ch_v1);
			if (IsCurrentAnimeEnd()) {
				m_AttackStage.stage++;
				SoundManager::PlaySE(SoundManager::SoundSE_ID::Reis_SE_Near_Swing, gameObject->mTransform->WorldPosition());
			}
		}
		else if (m_AttackStage.stage == 4) {
			SetWeapon(true, 10.0f);
			ChangeAnime(AnimeID::Attack_v1);
			if (IsCurrentAnimeEnd()) {

				SetWeapon(false);
				m_ReisMode = ReisMode::Idle;
				m_AttackStage = AttackStage();
				SetSuperArmor(false);
			}
		}

	};
}

//毎フレーム呼ばれます
void reis::Update(){

	OnDamage();

	m_MoveFunc[m_ReisMode]();

	BoneMoveUpdate();
}


//開放時に呼ばれます（Initialize１回に対してFinish１回呼ばれます）（エディター中も呼ばれます）
void reis::Finish(){

}

//コライダーとのヒット時に呼ばれます
void reis::OnCollideBegin(GameObject target){
	(void)target;
}

//コライダーとのヒット中に呼ばれます
void reis::OnCollideEnter(GameObject target){
	(void)target;
}

//コライダーとのロスト時に呼ばれます
void reis::OnCollideExit(GameObject target){
	(void)target;
}

void reis::WeaponCallHit()
{
	if (!m_NowAttack)return;
	if (!m_Player)return;
	{
		float time = Hx::DeltaTime()->GetDeltaTime();
		auto player = m_Player->GetScript<PlayerController>();
		if (player) {
			auto v = m_Player->mTransform->WorldPosition() - gameObject->mTransform->WorldPosition();
			player->Damage(20.0f, XMVector3Normalize(v), PlayerController::KnockBack::Low, false, false);
		}
	}
}


void reis::BattleStart()
{
	if (m_ReisMode == ReisMode::Standby) {
		m_ReisMode = ReisMode::Pop;
		if (m_EnemyBase) {
			if (auto com = m_EnemyBase->GetComponent<PhysXColliderComponent>()) {
				com->Enable();
			}
		}
	}
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

float reis::GetHp()
{
	if (auto enemy = Enemy::GetEnemy(m_EnemyBase)) {
		return enemy->GetEnemyAllParameter(false).hp;
	}
	return 0.0f;
}

void reis::OnDamage()
{
	if (auto enemy = Enemy::GetEnemy(m_EnemyBase)) {
		if (enemy->GetEnemyAllParameter(false).battleModeParameter.id == BATTLEACTION::WINCEACTION ||
			enemy->GetEnemyAllParameter(false).battleModeParameter.id == BATTLEACTION::BEATDOWNACTION ||
			enemy->GetEnemyAllParameter(false).battleModeParameter.id == BATTLEACTION::UPPERDOWNACTION){

			if (auto obj = Hx::Instance(m_DamageParticle)) {
				auto p = gameObject->mTransform->WorldPosition();
				p.y += 1.0f;
				obj->mTransform->WorldPosition(p);
				if (m_Player) {
					auto pp = m_Player->mTransform->WorldPosition();
					auto ep = gameObject->mTransform->WorldPosition();
					pp.y = 0.0f;
					ep.y = 0.0f;
					auto m = XMMatrixLookAtLH(pp, ep, XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f));
					auto q = XMQuaternionRotationMatrix(XMMatrixTranspose(m));
					obj->mTransform->WorldQuaternion(q);
				}
			}

			if (!m_SuperArmor) {
				ChangeAnime(AnimeID::Idle);
				m_ReisMode = ReisMode::Wince;
				m_AttackStage = AttackStage();
				SetWeapon(false);
				WeaponEffect(0, false);
			}
			else {
				m_SuperArmorHit++;
			}
			if (m_AutoDogdeMode) {
				m_ReisMode = ReisMode::Dogde;
				m_DogdeMode = DogdeMode::Rotate;
				SetWeapon(false);
				WeaponEffect(0, false);
			}
		}
		if (enemy->GetEnemyAllParameter(false).battleModeParameter.id == BATTLEACTION::DEADACTION) {
			m_ReisMode = ReisMode::Dead;
		}
	}
}


void reis::SetWeapon(bool enable, float damage)
{
	m_NowAttack = enable;
}

void reis::SetSuperArmor(bool f)
{
	if (auto enemy = Enemy::GetEnemy(m_EnemyBase)) {
		enemy->SetParentAlive(!f);
		m_SuperArmor = f;
	}
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
	if (m_AnimeModel&&m_BoneMirrorObject) {
		auto model = m_AnimeModel->mTransform->WorldPosition();
		auto bone = m_BoneMirrorObject->mTransform->WorldPosition();
	
		bool flag = false;
		if (XMVector3Length(m_BoneMirrorObject->mTransform->Position()).x > 0.01f) {
			flag = true;
		}
		bone -= model;
		bone.y = 0.0f;
		//m_AnimeModel->mTransform->Position(m_BoneMirrorObject->mTransform->Position()*0.02);
		if (flag) {
			auto move = bone - m_BoneBackPos;
	
			Move(move);
		}
	
		m_BoneBackPos = bone;
	}
}

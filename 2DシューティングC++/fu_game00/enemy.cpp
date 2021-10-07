//*****************************************************************************
//
// �G���� [scene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "enemy.h"
#include "boss.h"
#include "normalenemy.h"
#include "game.h"
#include "collision.h"
#include "player.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define BOSS				(CGame::GetBoss())
#define BOSS_DAMAGE			(CBoss::STATE_DAMAGE)
#define BOSS_NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)
#define BOSS_NONE			(CBoss::STATE_NONE)
#define BOSS_GET_LIFE		(CGame::GetBoss()->GetLife())

#define ENEMY_ID			(m_paEnemy[nID])
#define ENEMY_CNT			(m_paEnemy[nCnt])

//-----------------------------------------------------------------------------
// �ÓI�ϐ�
//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CScene2D(OBJ_ENEMY)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	CScene2D::Init(m_pos, m_size);	// �|���S���̐���
	CScene2D::SetSize(m_size);		// �T�C�Y�̐ݒ�
	CScene2D::SetCol(m_col);		// �F�̐ݒ�
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene2D::Uninit();	// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	if (this->CollisionPlayer() == true && GET_PLAYER->GetState() != CPlayer::STATE_KNOCKUP)
	{
		if (this->GetEnemyType() == ENEMY_BLACK)
		{
			GET_PLAYER->SetCollEnemy(true);
			this->Release();
		}
		else if (this->GetEnemyType() == ENEMY_WHITE)
		{
			GET_PLAYER->SetStock();
			this->Release();
		}
	}
	CollisionEnemy();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();	// �`�揈��
}

//=============================================================================
// ��ނ��Ƃ̓����蔻���̏���
//=============================================================================
void CEnemy::CollisionEnemy(void)
{
	if (this->GetBool() == true)
	{
		CBoss *pBoss = BOSS;

		// �G�̎��
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			this->DamegeLife(1);

			if (this->GetLife() == 0)
			{
				this->m_type = ENEMY_WHITE;
			}
			break;

		case ENEMY_TYPE1:
			break;

		case ENEMY_TYPE2:

			if (pBoss->GetState() == BOSS_NONE && pBoss->GetShield() == false)
			{
				pBoss->DamegeLife(1);
				pBoss->SetState(BOSS_DAMAGE);
			}
			else if (pBoss->GetState() == BOSS_NOT_DAMAGE)
			{
				pBoss->SetShield(true);
			}

			if (pBoss->GetLife() == 0)
			{
				pBoss->SetAlive(false);
				pBoss->Release();
			}
			break;
		}
		this->SetBool(false);
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻��
//=============================================================================
bool CEnemy::CollisionPlayer(void)
{
	bool bCollision = false;

	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	bCollision = CCollision::CollisionCycle(m_pos, pos, size.x);

	return bCollision;
}

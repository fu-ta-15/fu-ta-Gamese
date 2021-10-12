//*****************************************************************************
//
// �G���� [enemy.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "game.h"
#include "enemy.h"
#include "normalenemy.h"
#include "boss.h"
#include "player.h"
#include "collision.h"

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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(Priority type) : CScene2D(type)
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
	{// �v���C���[�Ƃ̓����蔻��
		if (this->GetEnemyType() == ENEMY_BLACK)
		{// �G���Â����
			GET_PLAYER->SetCollEnemy(true);
			this->Release();
		}
		else if (this->GetEnemyType() == ENEMY_WHITE)
		{// �G�����邢���
			this->Release();
		}
	}
	// �e�Ƃ̓����蔻��
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
	{// �����蔻�肪TRUE��������
		
		CBoss *pBoss = BOSS;	// �Q�[���V�[������擾

		// �G�̎��
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			this->DamegeLife(1);	// ���C�t����

			if (this->GetLife() == 0)
			{// ���C�t���[����������
				this->m_type = ENEMY_WHITE;
			}
			break;

		case ENEMY_TYPE1:

			this->Release();

			break;

		case ENEMY_TYPE2:

			if (pBoss->GetState() == BOSS_NONE && pBoss->GetShield() == false)
			{// �G���ʏ�̏�Ԃ�������
				pBoss->SetState(BOSS_DAMAGE);
			}
			else if (pBoss->GetState() == BOSS_NOT_DAMAGE)
			{// �_���[�W���󂯂Ă�����
				pBoss->SetShield(true);
			}
			break;
		}
		this->SetBool(false);	// �t���O������
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

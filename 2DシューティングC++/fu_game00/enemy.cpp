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
CEnemy *CEnemy::m_paEnemy[MAX_OBJECT] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CScene2D(OBJ_ENEMY)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (ENEMY_CNT == NULL)
		{// ���g������ۂ̏ꍇ
			ENEMY_CNT = this;	// �����
			break;
		}
	}
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
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{// ���g��NULL�ɂ���
		ENEMY_CNT = NULL;
	}
	CScene2D::Uninit();	// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{

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
void CEnemy::CollisionEnemy(int nID)
{
	// �G�̎��
	switch (ENEMY_ID->m_type)
	{
	case ENEMY_TYPE0:

		ENEMY_ID->DamegeLife(1);

		if (ENEMY_ID->GetLife() == 0)
		{
			ENEMY_ID->m_type = ENEMY_WHITE;
		}
		break;

	case ENEMY_TYPE1:
		//CScene::ObjRelease(OBJ_ENEMY, nID);
		//ENEMY_ID = NULL;
		break;

	case ENEMY_TYPE2:

		if (BOSS->GetState() == BOSS_NONE && BOSS->GetShield() == false)
		{
			BOSS->DamegeLife(1);
			BOSS->SetState(BOSS_DAMAGE);
		}
		else if (BOSS->GetState() == BOSS_NOT_DAMAGE)
		{
			BOSS->SetShield(true);
		}

		if (BOSS_GET_LIFE == 0)
		{
			BOSS->SetAlive(false);
			CScene::ObjRelease(OBJ_ENEMY, nID);
			m_paEnemy[nID] = NULL;
		}
		break;
	}
}

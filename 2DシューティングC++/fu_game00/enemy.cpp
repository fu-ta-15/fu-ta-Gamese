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
// �ÓI�ϐ�
//-----------------------------------------------------------------------------
CEnemy *CEnemy::m_paEnemy[MAX_OBJECT] = {};
CEnemy::ENEMY CEnemy::m_paEnemyType[MAX_OBJECT] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CScene2D(OBJ_ENEMY)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_paEnemy[nCnt] == NULL)
		{// ���g������ۂ̏ꍇ
			m_paEnemy[nCnt] = this;	// �����

			if (m_paEnemyType[nCnt] == ENEMY_NULL)
			{// �^�C�v���Ȃ�������
				m_paEnemyType[nCnt] = m_type;	// �^�C�v��ݒ�
			}
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

	switch (m_type)
	{
	case ENEMY_BLACK:	// �����G
		break;

	case ENEMY_WHITE:	// �����G
		break;

	case ENEMY_BOSS:	// �{�X�G
		break;

	default:
		break;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{// ���g��NULL�ɂ���
		m_paEnemy[nCnt] = NULL;
		m_paEnemyType[nCnt] = ENEMY_NULL;
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
	switch (m_paEnemy[nID]->m_type)
	{
	case ENEMY_BLACK:	// �����G
		m_paEnemy[nID]->DamegeLife(1);
		if (m_paEnemy[nID]->GetLife() == 0)
		{
			m_paEnemy[nID]->m_type = ENEMY_WHITE;
		}
		break;

	case ENEMY_WHITE:	// �����G
		CScene::ObjRelease(OBJ_ENEMY, nID);
		m_paEnemy[nID] = NULL;
		break;

	case ENEMY_BOSS:	// �{�X�G
		CGame::GetBoss()->DamegeLife(1);
		CGame::GetBoss()->SetState(CBoss::STATE_DAMAGE);
		if (CGame::GetBoss()->GetLife() == 0)
		{
			CGame::GetBoss()->SetAlive(false);
			CScene::ObjRelease(OBJ_ENEMY, nID);
			m_paEnemy[nID] = NULL;
		}
		break;

	default:
		break;
	}
}

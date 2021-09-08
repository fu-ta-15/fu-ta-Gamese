//*****************************************************************************
//
// �ʏ�̓G���� [normalenemy.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "normalenemy.h"
#include "collision.h"
#include "move.h"
#include "player.h"
#include "game.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-1.0f, -5.0f, 0.0f))
#define TYPE1_COLOR				(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNormalEnemy::CNormalEnemy()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNormalEnemy::~CNormalEnemy()
{
}

//=============================================================================
// �ʏ�̓G�̐���
//=============================================================================
CNormalEnemy * CNormalEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type)
{
	CNormalEnemy *pNormalEnemy = NULL;

	if (pNormalEnemy == NULL)
	{// NULL�`�F�b�N
		pNormalEnemy = new CNormalEnemy;
		pNormalEnemy->m_move = ENEMY_TYPE1_MOVE;  // �ړ���
		pNormalEnemy->SetPos(pos);				  // �ʒu
		pNormalEnemy->SetSize(size);			  // �T�C�Y
		pNormalEnemy->SetLife(ENEMY_LIFE);		  // �̗�
		pNormalEnemy->SetType(type);			  // ���
		pNormalEnemy->SetCol(TYPE1_COLOR);		  // �F
		pNormalEnemy->Init();					  // ������
	}

	return pNormalEnemy;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CNormalEnemy::Init(void)
{
	CEnemy::Init();	// ������
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNormalEnemy::Uninit(void)
{
	CEnemy::Uninit();	// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CNormalEnemy::Update(void)
{
	// �G�̎�ޕʂ̍X�V
	switch (CEnemy::GetEnemyType())
	{
		// �����G
	case ENEMY_BLACK:
		UpdateBlack();	// �X�V
		break;
		// �����G
	case ENEMY_WHITE:
		UpdateWhite();	// �X�V
		break;
	default:
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNormalEnemy::Draw(void)
{
	CEnemy::Draw();	// �`��
}

//=============================================================================
// �^�C�vBLACK�̍X�V
//=============================================================================
void CNormalEnemy::UpdateBlack(void)
{
	// �ʒu�Ɉړ��ʂ����Z
	m_pos += m_move;
	m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 100.0f, 550.0f);

	CScene2D::SetPos(m_pos);	// �ʒu�̍X�V
}

//=============================================================================
// �^�C�vWHITE�̍X�V
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();

	m_pos = CMove::TargetPosMove(pPlayer->GetPos(), m_pos, 0.026f);

	CScene2D::SetPos(m_pos);	// �ړ��ʂ̍X�V
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

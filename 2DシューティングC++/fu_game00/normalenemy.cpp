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
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-2.5f, -5.0f, 0.0f))
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
		pNormalEnemy->m_bCollision = false;		  // �����蔻��
		pNormalEnemy->SetPos(pos);				  // �ʒu
		pNormalEnemy->SetSize(size);			  // �T�C�Y
		pNormalEnemy->SetLife(ENEMY_LIFE);		  // �̗�
		pNormalEnemy->SetType(type);			  // ���
		pNormalEnemy->SetCol(TYPE1_COLOR);		  // �F
		pNormalEnemy->Init();					  // ������
	}

	return pNormalEnemy;
}

CNormalEnemy * CNormalEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, ENEMYTYPE type, EnemyMove movetype)
{
	CNormalEnemy *pNormalEnemy = NULL;

	if (pNormalEnemy == NULL)
	{// NULL�`�F�b�N
		pNormalEnemy = new CNormalEnemy;
		pNormalEnemy->m_bCollision = false;		 // �����蔻��
		pNormalEnemy->m_move = ENEMY_TYPE1_MOVE;  // �ړ���
		pNormalEnemy->SetPos(pos);				 // �ʒu
		pNormalEnemy->SetSize(size);			 // �T�C�Y
		pNormalEnemy->SetMoveType(movetype);	 // �ړ��̃^�C�v
		pNormalEnemy->SetLife(ENEMY_LIFE);		 // �̗�
		pNormalEnemy->SetType(type);			 // ���
		pNormalEnemy->SetCol(TYPE1_COLOR);		 // �F
		pNormalEnemy->Init();					 // ������
	}

	return pNormalEnemy;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CNormalEnemy::Init(void)
{
	CEnemy::Init();	// ������
	
	switch (CEnemy::GetEnemyType())
	{// �G�̎�ޕʂ̏�����
		
	case ENEMY_BLACK:
		CScene2D::CreateTexture(ENEMY_TEXTURE0);
		break;
		
	case ENEMY_WHITE:
		
		break;
	}

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
	CEnemy::Update();
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
	float fAngle = CMove::AnglePoint(m_pos.x - CGame::GetPlayer()->GetPos().x, m_pos.y - CGame::GetPlayer()->GetPos().y);

	switch (m_MoveType)
	{
	case CNormalEnemy::MOVE_0:
		m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 100.0f, 550.0f, 5.5f);
		m_pos += m_move;	// �ʒu�Ɉړ��ʂ����Z
		break;
	case CNormalEnemy::MOVE_1:
		m_move.x = CMove::HomingMove(sinf(D3DX_PI + fAngle), 2.0f);
		m_move.y = CMove::HomingMove(cosf(D3DX_PI - fAngle), 2.0f);
		m_pos += m_move;	// �ʒu�Ɉړ��ʂ����Z
		break;
	case CNormalEnemy::MOVE_2:
		m_fSinWaveCnt += 0.035f;
		m_pos.x = CMove::CosWave(m_pos.x, 15.0f, 3.5f, m_fSinWaveCnt);
		m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 80.0f, 400.0f, 5.5f);
		m_pos += m_move;	// �ʒu�Ɉړ��ʂ����Z
		break;
	case CNormalEnemy::MOVE_3:
		m_fCosWaveCnt += 0.35f;
		m_fSinWaveCnt += 0.035f;
		m_pos.x = CMove::CosWave(m_pos.x, 15.0f, 3.5f, m_fSinWaveCnt);
		m_pos.y = CMove::SinWave(m_pos.y, 10.0f, 4.0f, m_fSinWaveCnt);
		m_pos.x += m_move.x * 0.5f;	// �ʒu�Ɉړ��ʂ����Z
		break;
	default:
		break;
	}


	CScene2D::SetPos(m_pos);	// �ړ��ʂ̍X�V
}

//=============================================================================
// �^�C�vWHITE�̍X�V
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();

	m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 200.0f, 500.0f, 6.5f);
	m_pos.y += m_move.y;

	CScene2D::SetPos(m_pos);	// �ړ��ʂ̍X�V
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

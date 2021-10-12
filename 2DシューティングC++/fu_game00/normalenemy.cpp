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
#include "boss.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-2.5f, -5.0f, 0.0f))
#define TYPE1_COLOR				(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))
#define PLAYER_GET_POS			(CGame::GetPlayer()->GetPos())


//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
bool CNormalEnemy::m_bHomingTime = false;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNormalEnemy::CNormalEnemy() : CEnemy(OBJ_ENEMY)
{
	this->m_nMoveChangeCnt = 0;
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
		BlackMove();
		break;
		// �����G
	case ENEMY_WHITE:
		UpdateWhite();	// �X�V
		break;
	default:
		break;
	}

	CEnemy::Update();

	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f)
	{// ��ʊO����
		Release();
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
	// �J�E���g�A�b�v
	m_nMoveChangeCnt++;

	if ((m_nMoveChangeCnt % 1200) == 0)
	{// �w��̒l�Ŋ����ė]�肪�O�̏ꍇ
		if (m_bHomingTime == false)
		{// �z�[�~���O�^�C���ł͂Ȃ�������
			m_bHomingTime = true;
		}
		else
		{// �z�[�~���O�^�C����������
			m_bHomingTime = false;
		}
		m_nMoveChangeCnt = 0;	// �J�E���g������
	}

	if (m_bHomingTime == true)
	{// �z�[�~���O�^�C��
		if ((m_nMoveChangeCnt%Rand(60, 20)) == 0)
		{// �w��̒l�Ŋ����ė]�肪�[���̏ꍇ
			float fAngle = CMove::AnglePoint(m_pos.x - PLAYER_GET_POS.x, m_pos.y - PLAYER_GET_POS.y);
			m_move.x = CMove::HomingMove(sinf(D3DX_PI + fAngle), 2.0f);
			m_move.y = CMove::HomingMove(cosf(D3DX_PI - fAngle), 2.0f);
			m_MoveType = MOVE1;
		}
	}
}

//=============================================================================
// �^�C�vBLACK�̈ړ�����
//=============================================================================
void CNormalEnemy::BlackMove(void)
{
	float fAngle = CMove::AnglePoint(m_pos.x - CGame::GetPlayer()->GetPos().x, m_pos.y - CGame::GetPlayer()->GetPos().y);
	CBoss *pBoss = CGame::GetBoss();

	switch (m_MoveType)
	{
	case MOVE0:
		m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 100.0f, 550.0f, 5.5f);
		m_pos += m_move;	// �ʒu�Ɉړ��ʂ����Z
		break;

	case MOVE1:
		m_pos += m_move;	// �ʒu�Ɉړ��ʂ����Z
		break;

	case MOVE2:
		m_fSinWaveCnt += 0.035f;
		m_pos.x = CMove::CosWave(m_pos.x, 15.0f, 3.5f, m_fSinWaveCnt);
		m_move.y = CMove::MoveSnake(m_pos.y, m_move.y, 80.0f, 400.0f, 5.5f);
		m_pos += m_move;	// �ʒu�Ɉړ��ʂ����Z
		break;

	case MOVE3:
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		m_pos.x = CMove::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
		m_pos.y = CMove::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);
		break;

	case MOVE4:
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		m_pos.x += CMove::SinMove(2.5f, 14.0f, m_fSinWaveCnt);
		m_pos.y += CMove::CosMove(2.5f, 14.0f, m_fCosWaveCnt);
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
	m_fSinWaveCnt += 0.035f;

	m_pos.y = CMove::SinWave(m_pos.y, 10.0f, 4.0f, m_fSinWaveCnt);

	CScene2D::SetPos(m_pos);	// �ړ��ʂ̍X�V
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

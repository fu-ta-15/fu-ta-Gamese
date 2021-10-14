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
#define HOMING_TIME1			(1200)
#define HOMING_TIME2			(900)
#define HOMING_SPEED			(3.0f)
#define ANGLE_SIN				(sinf(D3DX_PI + fAngle))
#define ANGLE_COS				(cosf(D3DX_PI - fAngle))
#define ANGLE_POINT_X			(m_pos.x - PLAYER_GET_POS.x)
#define ANGLE_POINT_Y			(m_pos.y - PLAYER_GET_POS.y)

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
	this->m_nHomingTime = HOMING_TIME1;
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
		pNormalEnemy->m_move = ENEMY_TYPE1_MOVE; // �ړ���
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

	// �z�[�~���O�̊J�n�ƏI��
	if ((m_nMoveChangeCnt % m_nHomingTime) == 0)
	{// �w��̒l�Ŋ����ė]�肪�O�̏ꍇ
		if (m_bHomingTime == false)
		{// �z�[�~���O�^�C���ł͂Ȃ�������
			m_bHomingTime = true;
			m_nHomingTime = HOMING_TIME2;
		}
		else
		{// �z�[�~���O�^�C����������
			m_bHomingTime = false;
			m_nHomingTime = HOMING_TIME1;
		}
		m_nMoveChangeCnt = 0;	// �J�E���g������
	}

	if (m_bHomingTime == true)
	{// �z�[�~���O�^�C��
		if ((m_nMoveChangeCnt % Rand(60, 20)) == 0)
		{// �w��̒l�Ŋ����ė]�肪�[���̏ꍇ
			float fAngle = CMove::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);	// �v���C���[�Ƃ̊p�x
			m_move.x = CMove::HomingMove(ANGLE_SIN, HOMING_SPEED);			// �Ǐ]
			m_move.y = CMove::HomingMove(ANGLE_COS, HOMING_SPEED);			// �Ǐ]
			m_MoveType = MOVE1;												// �ړ����@
		}
	}
}

//=============================================================================
// �^�C�vBLACK�̈ړ�����
//=============================================================================
void CNormalEnemy::BlackMove(void)
{
	// �v���C���[�Ƃ̊p�x
	float fAngle = CMove::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);

	// �{�X�̏��
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

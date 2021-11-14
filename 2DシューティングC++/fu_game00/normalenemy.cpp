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
#include "time.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define ENEMY_TYPE1_MOVE		(D3DXVECTOR3(-2.5f, -5.0f, 0.0f))
#define TYPE1_COLOR				(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))
#define PLAYER_GET_POS			(CGame::GetPlayer()->GetPos())
#define HOMING_TIME1			(200)
#define HOMING_SPEED			(3.0f)
#define ANGLE_SIN				(sinf(D3DX_PI + fAngle))
#define ANGLE_COS				(cosf(D3DX_PI - fAngle))
#define ANGLE_POINT_X			(m_pos.x - PLAYER_GET_POS.x)
#define ANGLE_POINT_Y			(m_pos.y - PLAYER_GET_POS.y)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNormalEnemy::CNormalEnemy() : CEnemy(OBJ_ENEMY)
{
	this->m_nMoveChangeCnt = 0;			// �J�E���g
	this->m_bHomingTime = false;		// �z�[�~���O���Ă��邩
	this->m_nHomingTime = HOMING_TIME1;	// �z�[�~���O�^�C��
	this->m_move = ENEMY_TYPE1_MOVE;  	// �����̈ړ���
	this->m_bCollision = false;			// �����蔻��
	this->SetLife(ENEMY_LIFE);			// �̗�
	this->SetCol(TYPE1_COLOR);			// �F
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
	// �N���X�̃|�C���^
	CNormalEnemy *pNormalEnemy = NULL;

	// NULL�`�F�b�N
	if (pNormalEnemy == NULL)
	{
		pNormalEnemy = new CNormalEnemy;
		pNormalEnemy->SetPos(pos);				// �ʒu
		pNormalEnemy->SetSize(size);			// �T�C�Y
		pNormalEnemy->SetMoveType(movetype);	// �ړ��̃^�C�v
		pNormalEnemy->SetType(type);			// ���
		pNormalEnemy->Init();					// ������
	}

	return pNormalEnemy;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CNormalEnemy::Init(void)
{
	// ������
	CEnemy::Init();	
	
	// �e�N�X�`���̐ݒ�
	CScene2D::CreateTexture(ENEMY_TEXTURE0);

	// �ړ��ʂ̎��
	switch (m_MoveType)
	{
	case CNormalEnemy::MOVE_0:

		// �v���C���[�̊p�x�Z�o
		float fAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);

		// �v���C���[�̊p�x�̕����Ɍ������Ĉړ��ʐݒ�
		m_move.x = sinf(fAngle - D3DX_PI) * 2.5f;
		m_move.y = cosf(fAngle + D3DX_PI) * 2.5f;
		break;

	case CNormalEnemy::MOVE_2:

		// �����_���Ȋp�x����
		float fAngle = (float)(rand() % 314) / 100.f;

		// �ݒ肳�ꂽ�p�x�̕����Ɉړ�����ݒ�
		m_move.x = sinf(fAngle - D3DX_PI) * 2.5f;
		m_move.y = cosf(fAngle + D3DX_PI) * 2.5f;
		break;

	default:
		break;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNormalEnemy::Uninit(void)
{
	// �I������
	CEnemy::Uninit();	
}

//=============================================================================
// �X�V����
//=============================================================================
void CNormalEnemy::Update(void)
{
	// �G�̎�ޕʂ̍X�V
	switch (CEnemy::GetEnemyType())
	{
	case ENEMY_BLACK:

		// �X�V
		UpdateBlack();	

		// �ړ�����
		BlackMove();	
		break;

	case ENEMY_WHITE:

		// �X�V
		UpdateWhite();	
		break;

	default:
		break;
	}

	// �X�V����
	CEnemy::Update();

	// ��ʊO����
	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f &&
		m_pos.y - m_size.y > SCREEN_HEIGHT || m_pos.y + m_size.y < 0.0f)
	{
		// �I������
		CEnemy::Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNormalEnemy::Draw(void)
{
	// �`��
	CEnemy::Draw();	
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
			m_nHomingTime = Rand(100, 400);
		}
		else
		{// �z�[�~���O�^�C����������
			m_bHomingTime = false;
			m_nHomingTime = HOMING_TIME1;
		}

		// �J�E���g������
		m_nMoveChangeCnt = 0;	
	}

	if (m_bHomingTime == true)
	{// �z�[�~���O�^�C��
		if ((m_nMoveChangeCnt % Rand(60, 20)) == 0)
		{// �w��̒l�Ŋ����ė]�肪�[���̏ꍇ

			// �v���C���[�Ƃ̊p�x
			float fAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);	

			// �Ǐ]
			m_move.x = Move::HomingMove(ANGLE_SIN, HOMING_SPEED);			
			m_move.y = Move::HomingMove(ANGLE_COS, HOMING_SPEED);			

			// �ړ����@
			m_MoveType = MOVE1;												
		}
	}
}

//=============================================================================
// �^�C�vBLACK�̈ړ�����
//=============================================================================
void CNormalEnemy::BlackMove(void)
{
	// �v���C���[�Ƃ̊p�x
	float fPlayerAngle = Move::AnglePoint(ANGLE_POINT_X, ANGLE_POINT_Y);

	// �{�X�̏��
	CBoss *pBoss = CGame::GetBoss();

	// �ړ��̎��
	switch (m_MoveType)
	{
	case MOVE0:
		// �ʒu�Ɉړ��ʂ����Z
		m_pos += m_move;	
		break;

	case MOVE1:
		// �ʒu�Ɉړ��ʂ����Z
		m_pos += m_move;	
		break;

	case MOVE2:
		// �ʒu�Ɉړ��ʂ����Z
		m_pos += m_move;	
		break;

	case MOVE3:
		// �J�E���g�A�b�v
		m_fCosWaveCnt += 0.035f;
		m_fSinWaveCnt += 0.035f;

		// �ʒu�Ɉړ��̒l���
		m_pos.x = Move::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
		m_pos.y = Move::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);
		break;

	default:
		break;
	}

	// �ʒu�̍X�V
	CScene2D::SetPos(m_pos);	
}

//=============================================================================
// �^�C�vWHITE�̍X�V
//=============================================================================
void CNormalEnemy::UpdateWhite(void)
{
	// �{�X�̏��
	CBoss *pBoss = CGame::GetBoss();

	// �J�E���g�A�b�v
	m_fCosWaveCnt += 0.035f;
	m_fSinWaveCnt += 0.035f;

	// �ʒu�̕ύX
	m_pos.x = Move::SinWave(pBoss->GetPos().x, 140.0f, 14.0f, m_fSinWaveCnt);
	m_pos.y = Move::CosWave(pBoss->GetPos().y, 140.0f, 14.0f, m_fCosWaveCnt);

	// �ʒu�̍X�V
	CScene2D::SetPos(m_pos);	

	// �F�̍X�V
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================================================================
// �G�̔z�u
//=============================================================================
void ENEMY::SetEnemy(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, CEnemy::ENEMYTYPE type, CNormalEnemy::EnemyMove movetype, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		// �G�̐���
		CNormalEnemy::Create(pos, size, type, movetype);
	}
}

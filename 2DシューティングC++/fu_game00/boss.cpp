//*****************************************************************************
//
// �{�X�G���� [boss.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "boss.h"
#include "normalenemy.h"
#include "manager.h"
#include "renderer.h"
#include "move.h"
#include "collision.h"
#include "enemy.h"
#include "time.h"
#include "game.h"
#include "bulletmesh.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")					// �_���[�W��ԃe�N�X�`�������N
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))				// �_���[�W��Ԃ̐F	
#define DAMAGE_ADD_A	(0.065f)										// �_���[�W�̃��l���Z�l
#define SHILED_TEXTUER	("data/TEXTURE/AuroraRing.png")					// �o���A�e�N�X�`�������N
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))				// �o���A�̐F
#define SHILED_SIZE		(m_size * 2.5f)									// �o���A�̃T�C�Y
#define SHILED_ADD_A	(0.04f)											// �o���A�̃��l���Z�l
#define LIFE_POS_X		(SCREEN_WIDTH - 100.0f)							// ���C�t��X���W
#define LIFE_POS_Y		(200.0f - (10 * nCntLife))						// ���C�t��Y���W
#define LIFE_POS		(D3DXVECTOR3(LIFE_POS_X, LIFE_POS_Y, 0.0f))		// ���C�t�̈ʒu
#define LIFE_SIZE		(D3DXVECTOR3(10.0f, 5.0f, 0.0f))				// ���C�t�̃T�C�Y
#define LIFE_TEXTURE	("data/TEXTURE/BossLife.png")					// ���C�t�e�N�X�`�������N
#define LIFE_DOWN		((m_fLife * 0.17f + 0.425f))					// ���C�t�̌����v�Z
#define LIFE_CNT		(nCntLife * 10)									// ���C�t�J�E���g
#define ADD_MOVE_TIME	(0.54f)											// �ړ��̃J�E���g�̉��Z�l
#define DECREASE_A		(0.008f)										// ���l�̌����l
#define ADD_MAX_A		(0.7f)											// ���l�̍ő�l


//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBoss::CBoss() : CEnemy(OBJ_BOSS)
{
	this->m_pDamage = NULL;				// �_���[�W�G�t�F�N�g
	this->m_bDamage = false;			// �_���[�W�󂯂Ă��邩���Ȃ���
	this->m_State = STATE_NONE;			// ���
	this->m_LifeState = LIFE_RATE_0;	// ���C�t�̏��
	this->m_move = ZeroVector3;			// �ړ��ʏ�����
	this->m_bBoss_Alive = true;			// ��������
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBoss::~CBoss()
{
}

//=============================================================================
// �{�X�̐���
//=============================================================================
CBoss * CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nLife)
{
	CBoss *pBoss = NULL;

	if (pBoss == NULL)
	{// NULL�`�F�b�N
		pBoss = new CBoss;				// �C���X�^���X����
		pBoss->SetPos(pos);				// �ʒu
		pBoss->SetSize(size);			// �T�C�Y
		pBoss->m_fLife = (float)nLife;	// ���C�t
		pBoss->SetCol(WhiteColor);		// �F
		pBoss->SetType(ENEMY_TYPE2);	// �^�C�v
		pBoss->Init();					// ������
	}

	return pBoss;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CBoss::Init(void)
{	
	// ������
	CEnemy::Init();

	// �_���[�W�J�E���g
	m_nDamageCnt = 0;	 

	// �ړ��p�J�E���g
	m_fMoveTime = 0.0f;	 

	// �����x���Z�p
	m_fA_Damage = DAMAGE_ADD_A;		

	// ���C�t�ݒ�
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = LIFE_POS;						 // �ʒu
		D3DXVECTOR3 size = LIFE_SIZE;					 // �T�C�Y
		m_pLife[nCntLife] = CScene2D::Create(pos, size); // ����
		m_pLife[nCntLife]->CreateTexture(LIFE_TEXTURE);	 // �e�N�X�`���̐ݒ�
	}

	m_pDamage = CEffect::Create(m_pos, m_size);	   // ����
	m_pDamage->CreateTexture(DAMAGE_TEXTUER);	   // �e�N�X�`���̐ݒ�
	m_pDamage->SetColor(DAMAGE_COLOR);			   // �F�̐ݒ�
	m_StateCol = m_pDamage->GetColor();			   // �F�̎擾


	// �R�A�̐���
	m_pCore[0] = CCore::Create(m_pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f), 0, OBJ_CORE);
	m_pCore[1] = CCore::Create(m_pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f), 50, OBJ_CORE);
	m_pCore[2] = CCore::Create(m_pos, D3DXVECTOR3(25.0f, 25.0f, 0.0f), 100, OBJ_CORE);

	// �V�[���h�̐���
	m_pShiled = CScene2D::Create(m_pos, m_size * 1.5f, OBJ_EFFECT2);
	m_pShiled->CreateTexture("data/TEXTURE/t0003.png");

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBoss::Uninit(void)
{
	// �R�A�̊J��
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pCore[nCnt] != NULL)
		{
			// �I������
			m_pCore[nCnt]->Uninit();

			// NULL����
			m_pCore[nCnt] = NULL;
		}
	}

	// �V�[���h�̊J��
	if (m_pShiled != NULL)
	{
		// �I������
		m_pShiled->Uninit();

		// NULL����
		m_pShiled = NULL;
	}

	// ���g�̊J��
	CEnemy::Uninit();	
}

//=============================================================================
// �X�V����
//=============================================================================
void CBoss::Update(void)
{
	// ���b�V���|���S���̏��擾
	switch (CManager::GetMode())
	{
		// �Q�[�����[�h
	case CManager::MODE_GAME:

		// �X�V
		CEnemy::Update();

		if (m_State == DAMAGE)
		{
			// �_���[�W���}
			m_bDamage = true;
			m_fLife -= LIFE_DOWN;			// �̗͂̌���
											// �����x���Z�p
			m_fA_Damage = DAMAGE_ADD_A;

		}

		// �{�X�̍X�V
		UpdateBoss();		// ���ʂ̍X�V
		DamageBoss();		// �_���[�W��ԍX�V
		StateUpdate();		// ��Ԃ̍X�V
		MoveBoss();			// �ړ�����
		SummonsEnemy();		// �G��������
		break;

	default:
		break;
	}

	m_pShiled->SetPos(m_pos); 	// �ʒu�̍X�V
	CScene2D::SetPos(m_pos);	// �ʒu�̍X�V
	CScene2D::SetSize(m_size);	// �T�C�Y�̍X�V

	if (m_fLife <= 0)
	{// ���C�t���[���̏ꍇ�A���S
		m_bBoss_Alive = false;
		CEnemy::Uninit();	// ���g�̊J��
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBoss::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//=============================================================================
// BOSS�̍X�V
//=============================================================================
void CBoss::UpdateBoss(void)
{
	// ���C�t�u���b�N�̍X�V
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		if (m_fLife < LIFE_CNT)
		{// ���C�t����؂�̒l���z������
			switch (nCntLife)
			{// �c��̗�
			case 12:
				if (m_LifeState == LIFE_RATE_0)
				{
					m_LifeState = LIFE_RATE_2;	// �Q������
					m_fMoveTime = 0.0f;
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						m_pCore[nCnt]->SetUse(true);
					}
				}
				break;

			case 7:
				if (m_LifeState == LIFE_RATE_2)
				{
					m_LifeState = LIFE_RATE_5;	// �T������
					m_fMoveTime = 0.0f;
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						m_pCore[nCnt]->SetUse(true);
					}
				}
				break;

			case 2:
				if (m_LifeState == LIFE_RATE_5)
				{
					m_LifeState = LIFE_RATE_8;	// �W������
					m_fMoveTime = 0.0f;
					for (int nCnt = 0; nCnt < 3; nCnt++)
					{
						m_pCore[nCnt]->SetUse(true);
					}
				}
				break;

			default:
				break;
			}

			if (m_pLife[nCntLife] != NULL)
			{// NULL�`�F�b�N
				m_pLife[nCntLife]->Uninit();  // ���C�t�̃u���b�N�폜
				m_pLife[nCntLife] = NULL;	   // NULL���
				break;
			}
		}
	}
}

//=============================================================================
// �_���[�W���󂯂���
//=============================================================================
void CBoss::DamageBoss(void)
{
	// �_���[�W���󂯂���
	if (m_bDamage == true)
	{
		// ���΂炭�_���[�W���󂯂Ȃ����
		m_State = STATE_NOT_DAMAGE;		
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CBoss::MoveBoss(void)
{
	// �J�E���g���Z
	m_fMoveTime += ADD_MOVE_TIME;

	// ���C�t�̏�Ԃňړ��ύX
	switch (m_LifeState)
	{// �̗͂̏��
	case LIFE_RATE_0:
		m_pos.y = Move::CosWave(HEIGHT_HALF,100.0f, 65.5f, m_fMoveTime);
		m_OldPos = m_pos;
		break;

	case LIFE_RATE_2:
		m_pos.y = Move::CosWave(m_OldPos.y,60.0f, 100.5f, m_fMoveTime);
		m_pos.x += Move::SinMove(-10.0f, 60.0f, m_fMoveTime);
		break;

	case LIFE_RATE_5:
		m_pos = Move::TargetPosMove(D3DXVECTOR3(SCREEN_WIDTH - 150.0f , HEIGHT_HALF + 100.0f, 0.0f), m_pos, 0.025f);
		break;

	case LIFE_RATE_8:
		m_pos = Move::TargetPosMove(D3DXVECTOR3(WIDTH_HALF + 300.0f, HEIGHT_HALF - 50.0f, 0.0f), m_pos, 0.0025f);
		break;

	default:
		break;
	}
	CScene2D::SetPos(m_pos);
}

//=============================================================================
// �G�̏���
//=============================================================================
void CBoss::SummonsEnemy(void)
{
	m_pos = CScene2D::GetPos();		// �ʒu�̎擾
	int nFrame = TIME->GetFrame();	// ���Ԃ̎擾

	// �̗͂̏�ԂœG�̏������@�ύX
	switch (m_LifeState)
	{// �̗͂̏��
	case LIFE_RATE_0:
		if ((nFrame % 15) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
		}
		break;

	case LIFE_RATE_2:
		if ((nFrame % 60) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 25);
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0, 1);
		}
		break;

	case LIFE_RATE_5:
		if ((nFrame % 20) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0, 1);
		}
		if ((nFrame % 10) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
		}
		break;

	case LIFE_RATE_8:
		if ((nFrame % 7) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0, 1);
		}
		if ((nFrame % 5) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3, 1);
		}
		if ((nFrame % 30) == 0)
		{
			ENEMY::SetEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
		}
		if ((nFrame % 100) == 0)
		{
			if (m_pCore[0]->GetUse() == true)
			{
				ENEMY::SetEnemy(m_pCore[0]->GetPos(), ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
			}
			if (m_pCore[1]->GetUse() == true)
			{
				ENEMY::SetEnemy(m_pCore[1]->GetPos(), ENEMY_SIZE, ENEMY_TYPE0, MOVE2, 10);
			}
		}
		break;

	default:
		break;
	}
}

//=============================================================================
// ��ԊǗ�
//=============================================================================
void CBoss::StateUpdate(void)
{
	// �_���[�W���󂯂Ă�����
	if (m_bDamage != false)
	{
		// �J���[�̍X�V
		m_pDamage->SetColor(m_StateCol);

		// �ʒu���X�V
		m_pDamage->SetPos(m_pos);		

		// ���l�̉��Z
		m_StateCol.a += m_fA_Damage;

		// ���l�������z������
		if (m_StateCol.a >= 1.0f)
		{
			// ���Z�̐��l���}�C�i�X��
			m_fA_Damage = -DECREASE_A;	
		}
		else if (m_StateCol.a <= 0.0f)
		{
			// FALSE���
			m_bDamage = false;		

			// ��Ԃ�߂�
			m_State = STATE_NONE;		
		}
	}
}

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
#define CreateEnemy		(CNormalEnemy::Create)							// �G�̐���
#define CreateEffect	(CEffect::Create)								// �G�t�F�N�g�̐���


//-----------------------------------------------------------------------------
// �ÓI�ϐ�
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// �������Ă��邩�ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBoss::CBoss() : CEnemy(OBJ_BOSS)
{
	this->m_pDamage = NULL;				// �_���[�W�G�t�F�N�g
	this->m_pShield = NULL;				// �V�[���h�G�t�F�N�g
	this->m_bShield = false;			// �V�[���h�o�����o���Ȃ���
	this->m_bDamage = false;			// �_���[�W�󂯂Ă��邩���Ȃ���
	this->m_State = STATE_NONE;			// ���
	this->m_LifeState = LIFE_RATE_0;	// ���C�t�̏��
	this->m_move = ZeroVector3;			// �ړ��ʏ�����
	m_bBoss_Alive = true;				// ��������
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
	m_nDamageCnt = 0;	 // �_���[�W�J�E���g
	m_fMoveTime = 0.0f;	 // �ړ��p�J�E���g

	// ���C�t�ݒ�
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = LIFE_POS;						 // �ʒu
		D3DXVECTOR3 size = LIFE_SIZE;					 // �T�C�Y
		m_pLife[nCntLife] = CScene2D::Create(pos, size); // ����
		m_pLife[nCntLife]->CreateTexture(LIFE_TEXTURE);	 // �e�N�X�`���̐ݒ�
	}

	CEnemy::Init();	// ������

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBoss::Uninit(void)
{
	CEnemy::Uninit();	// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CBoss::Update(void)
{

	// ���b�V���|���S���̏��擾
	switch (CManager::GetMode())
	{
		// �`���[�g���A��
	case CManager::MODE_TUTORIAL:


		break;

		// �Q�[��
	case CManager::MODE_GAME:

		// �X�V
		CEnemy::Update();

		if (m_State == DAMAGE)
		{
			// �_���[�W���}
			m_bDamage = true;	
		}
		if (m_fLife <= 0)
		{
			// ���C�t���[���̏ꍇ�A���S
			m_bBoss_Alive = false;	
			Release();				
		}

		// �{�X�̍X�V
		UpdateBoss();		// ���ʂ̍X�V
		DamageBoss();		// �_���[�W��ԍX�V
		NotDamageBoss();	// �_���[�WNO�I��ԍX�V
		StateUpdate();		// ��Ԃ̌��l
		MoveBoss();			// �ړ�����
		SummonsEnemy();		// �G��������
		break;

	default:
		break;
	}

	// �ʒu�̍X�V
	CScene2D::SetPos(m_pos);
	CScene2D::SetSize(m_size);
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
				}
				break;

			case 7:
				if (m_LifeState == LIFE_RATE_2)
				{
					m_LifeState = LIFE_RATE_5;	// �T������
					m_fMoveTime = 0.0f;
				}
				break;

			case 2:
				if (m_LifeState == LIFE_RATE_5)
				{
					m_LifeState = LIFE_RATE_8;	// �W������
					m_fMoveTime = 0.0f;
				}
				break;

			default:
				break;
			}

			if (m_pLife[nCntLife] != NULL)
			{// NULL�`�F�b�N
				m_pLife[nCntLife]->Release();  // ���C�t�̃u���b�N�폜
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
	if (m_bDamage == true)
	{// �_���[�W���
		if (m_pDamage == NULL)
		{// NULL�`�F�b�N
			m_State = STATE_NOT_DAMAGE;		// �_���[�W�󂯕t���Ȃ����
			m_fA_Damage = DAMAGE_ADD_A;		// �����x���Z�p
			m_fLife -= LIFE_DOWN;			// �̗͂̌���

			m_pDamage = CreateEffect(m_pos, m_size);	   // ����
			m_pDamage->CreateTexture(DAMAGE_TEXTUER);	   // �e�N�X�`���̐ݒ�
			m_pDamage->SetColor(DAMAGE_COLOR);			   // �F�̐ݒ�
			m_StateCol = m_pDamage->GetColor();			   // �F�̎擾

		}
		if (m_pDamage != NULL)
		{// NULL����Ȃ�������
			m_pDamage->SetPos(m_pos);	// �ʒu���X�V
		}
	}
}

//=============================================================================
// �_���[�W�󂯕t���Ȃ����
//=============================================================================
void CBoss::NotDamageBoss(void)
{
	if (m_bShield == true)
	{// �V�[���h�W�J��
		if (m_pShield == NULL)
		{// NULL�`�F�b�N
			m_pShield = CreateEffect(m_pos, SHILED_SIZE);	// ����
			m_pShield->CreateTexture(SHILED_TEXTUER);		// �e�N�X�`���ݒ�
			m_pShield->SetColor(SHILED_COLOR);				// �F�̐ݒ�
			m_ShieldCol = m_pShield->GetColor();			// �F�̎擾
			m_fA_Shield = SHILED_ADD_A;						// �����x���Z�p
		}
		if (m_pShield != NULL)
		{// NULL����Ȃ�������
			m_pShield->SetPos(m_pos);	// �ʒu���X�V
		}
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

		m_pos.y = Move::CosWave(HEIGHT_HALF, 50.0f, 65.5f, m_fMoveTime);

		break;

	case LIFE_RATE_8:

		m_move = Move::TargetPosMove(D3DXVECTOR3(WIDTH_HALF + 200.0f, HEIGHT_HALF - 100.0f, 0.0f), m_pos, 0.0025f);

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
		if ((nFrame % 130) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3);

		}
		if ((nFrame % 100) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE4);
		}
		break;

	case LIFE_RATE_2:
		if ((nFrame % 120) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3);
		}
		if ((nFrame % 160) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
		}
		break;

	case LIFE_RATE_5:
		if ((nFrame % 156) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE2);
		}
		if ((nFrame % 150) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE1, MOVE2);
		}
		break;

	case LIFE_RATE_8:
		if ((nFrame % 80) == 0)
		{
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0);
			CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE0);
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
	if (m_pShield != NULL)
	{// �V�[���h�W�J��
		// �J���[�̍X�V
		m_pShield->SetColor(m_ShieldCol);
		m_ShieldCol.a += m_fA_Shield;

		if (m_ShieldCol.a >= ADD_MAX_A)
		{// ���l�������z������
			m_fA_Shield = -DECREASE_A;	// ���Z
		}
		if (m_ShieldCol.a < 0.0f)
		{// ���l�������z������
			m_pShield->Uninit();	// �I������
			m_pShield = NULL;		// NULL���
			m_bShield = false;		// FALSE���
		}
	}
	if (m_pDamage != NULL)
	{// �_���[�W���
		// �J���[�̍X�V
		m_pDamage->SetColor(m_StateCol);
		m_StateCol.a += m_fA_Damage;

		if (m_StateCol.a >= ADD_MAX_A)
		{// ���l�������z������
			m_fA_Damage = -DECREASE_A;	// ���Z
		}
		if (m_StateCol.a <= 0.0f)
		{// ���l�������z������
			m_pDamage->Uninit();	// �I������
			m_pDamage = NULL;		// NULL���
			m_bDamage = false;		// FALSE���
			m_State = STATE_NONE;	// ��Ԃ�߂�
		}
	}
}

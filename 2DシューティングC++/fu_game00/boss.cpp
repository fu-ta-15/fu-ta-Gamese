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

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define DAMAGE			(CBoss::STATE_DAMAGE)				// �_���[�W���
#define NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)			// �_���[�WNO!
#define NONE			(CBoss::STATE_NONE)					// �����Ȃ����
															 
#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")		// �_���[�W��ԃe�N�X�`���̃����N
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))	// �F
															
#define SHILED_TEXTUER	("data/TEXTURE/shield.png")			// �V�[���h�e�N�X�`���̃����N
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))	// �F


//-----------------------------------------------------------------------------
// �ÓI�ϐ�
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// �������Ă��邩�ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBoss::CBoss()
{
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
		pBoss = new CBoss;
		pBoss->m_pDamage = NULL;		// �_���[�W�G�t�F�N�g
		pBoss->m_pShield = NULL;		// �V�[���h�G�t�F�N�g
		pBoss->SetPos(pos);				// �ʒu
		pBoss->SetSize(size);			// �T�C�Y
		pBoss->SetLife(nLife);			// ���C�t
		pBoss->SetCol(WhiteColor);		// �F
		pBoss->SetType(ENEMY_TYPE2);	// �^�C�v
		pBoss->m_State = STATE_NONE;	// ���
		pBoss->m_bShield = false;		// �V�[���h�o�����o���Ȃ���
		pBoss->m_bDamage = false;		// �_���[�W�󂯂Ă��邩���Ȃ���
		m_bBoss_Alive = true;			// ��������
		pBoss->Init();					// ������
	}

	return pBoss;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CBoss::Init(void)
{
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
	// �ʒu�̍X�V
	CScene2D::SetPos(m_pos);	
	CEnemy::Update();
	switch (m_State)
	{// ���݂̏��
	case NONE:
		break;

	case DAMAGE:
		m_bDamage = true;	// �_���[�W���}
		break;

	case NOT_DAMAGE:
		break;
	}

	// �{�X�̍X�V
	UpdateBoss();		// ���ʂ̍X�V
	DamageBoss();		// �_���[�W��ԍX�V
	NotDamageBoss();	// �_���[�WNO�I��ԍX�V
	StateUpdate();		// ��Ԃ̌��l
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
	srand((unsigned)time(NULL));	// ���ݎ����̏��ŏ�����

	m_pos = CScene2D::GetPos();		// �ʒu�̎擾

	int nCntRand = 0;				// �����ۊǗp

	nCntRand = rand() % 60 + 100;	// ��������
	m_nCnt++;						// �J�E���g���Z

	int nRandEne0 = rand() % 250 + nCntRand;
	int nEnemy0 = rand() % 50 + 11;
	int nMoveType = rand() % 4;

	if ((m_nCnt % nRandEne0) == nEnemy0)
	{
		CNormalEnemy::Create(m_pos, ENEMY_SIZE, ENEMY_TYPE0,(CNormalEnemy::EnemyMove)nMoveType);
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
			m_State = STATE_NOT_DAMAGE;					   // �_���[�W�󂯕t���Ȃ����
			m_pDamage = CEffect::Create(m_pos, m_size);	   // �G�t�F�N�g����
			m_pDamage->CreateTexture(DAMAGE_TEXTUER);	   // �e�N�X�`���̐ݒ�
			m_pDamage->SetColor(DAMAGE_COLOR);			   // �F�̐ݒ�
			m_StateCol = m_pDamage->GetColor();			   // �F�̎擾
			m_fA_Damage = 0.065f;						   // �����x���Z�p
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
			m_pShield = CEffect::Create(m_pos, m_size);	  // �G�t�F�N�g�̐���
			m_pShield->CreateTexture(SHILED_TEXTUER);	  // �e�N�X�`���ݒ�
			m_pShield->SetColor(SHILED_COLOR);			  // �F�̐ݒ�
			m_ShieldCol = m_pShield->GetColor();		  // �F�̎擾
			m_fA_Shield = 0.04f;						  // �����x���Z�p
		}
	}
}

//=============================================================================
// ��ԊǗ�
//=============================================================================
void CBoss::StateUpdate(void)
{
	if (m_pShield != NULL)
	{
		// �J���[�̍X�V
		m_pShield->SetColor(m_ShieldCol);

		m_ShieldCol.a += m_fA_Shield;

		if (m_ShieldCol.a >= 0.7f)
		{
			m_fA_Shield = -0.1f;
		}
		if (m_ShieldCol.a < 0.0f)
		{
			m_pShield->Uninit();
			m_pShield = NULL;
			m_bShield = false;
		}
	}
	if (m_pDamage != NULL)
	{
		// �J���[�̍X�V
		m_pDamage->SetColor(m_StateCol);

		m_StateCol.a += m_fA_Damage;

		if (m_StateCol.a >= 0.7f)
		{
			m_fA_Damage = -0.005f;
		}
		if (m_StateCol.a <= 0.0f)
		{
			m_pDamage->Uninit();
			m_pDamage = NULL;
			m_State = STATE_NONE;
			m_bDamage = false;
		}
	}
}

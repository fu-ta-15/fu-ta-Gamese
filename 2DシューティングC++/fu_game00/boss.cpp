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
#define DAMAGE			(CBoss::STATE_DAMAGE)
#define NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)
#define NONE			(CBoss::STATE_NONE)

#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))

#define SHILED_TEXTUER	("data/TEXTURE/shield.png")
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))


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

	switch (m_State)
	{
	case NONE:
		NoneBoss();
		break;

	case DAMAGE:
		m_bDamage = true;
		break;

	case NOT_DAMAGE:
		break;
	}

	// �{�X�̍X�V
	UpdateBoss();
	DamageBoss();
	NotDamageBoss();
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

	if ((m_nCnt % nRandEne0) == nEnemy0)
	{
		CNormalEnemy::Create(m_pos, ENEMY_SIZE, ENEMY_TYPE0);
	}
}

//=============================================================================
// �{�X�̒ʏ���
//=============================================================================
void CBoss::NoneBoss(void)
{



}

//=============================================================================
// �_���[�W���󂯂���
//=============================================================================
void CBoss::DamageBoss(void)
{
	if (m_bDamage == true)
	{
		if (m_pDamage == NULL)
		{
			m_State = STATE_NOT_DAMAGE;
			m_pDamage = CEffect::Create(m_pos, m_size);
			m_pDamage->CreateTexture(DAMAGE_TEXTUER);
			m_pDamage->SetColor(DAMAGE_COLOR);
			m_StateCol = m_pDamage->GetColor();
			m_nA_Damage = 0.065f;
			printf("�_���[�W");
		}
		if (m_pDamage != NULL)
		{
			// �J���[�̍X�V
			m_pDamage->SetColor(m_StateCol);

			m_StateCol.a += m_nA_Damage;

			if (m_StateCol.a >= 0.7f)
			{
				m_nA_Damage = -0.005f;
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
}

//=============================================================================
// �_���[�W�󂯕t���Ȃ����
//=============================================================================
void CBoss::NotDamageBoss(void)
{
	if (m_bShield == true)
	{
		if (m_pShield == NULL)
		{
			m_pShield = CEffect::Create(m_pos, m_size);
			m_pShield->CreateTexture(SHILED_TEXTUER);
			m_pShield->SetColor(SHILED_COLOR);
			m_ShieldCol = m_pShield->GetColor();
			m_nA_Shield = 0.04f;
		}

		if (m_pShield != NULL)
		{
			// �J���[�̍X�V
			m_pShield->SetColor(m_ShieldCol);

			m_ShieldCol.a += m_nA_Shield;

			if (m_ShieldCol.a >= 0.7f)
			{
				m_nA_Shield = -0.1f;
			}
			if (m_ShieldCol.a < 0.0f)
			{
				m_pShield->Uninit();
				m_pShield = NULL;
				m_bShield = false;
			}
		}
	}
}

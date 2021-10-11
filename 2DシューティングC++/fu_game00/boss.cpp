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

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define DAMAGE			(CBoss::STATE_DAMAGE)				// �_���[�W���
#define NOT_DAMAGE		(CBoss::STATE_NOT_DAMAGE)			// �_���[�WNO!
#define NONE			(CBoss::STATE_NONE)					// �����Ȃ����
															 
#define DAMAGE_TEXTUER	("data/TEXTURE/stateBoss.png")		// �_���[�W��ԃe�N�X�`���̃����N
#define DAMAGE_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))	// �F
															
#define SHILED_TEXTUER	("data/TEXTURE/AuroraRing.png")			// �V�[���h�e�N�X�`���̃����N
#define SHILED_COLOR	(D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))	// �F
#define SHILED_SIZE		(m_size*2.5f)

//-----------------------------------------------------------------------------
// �ÓI�ϐ�
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// �������Ă��邩�ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBoss::CBoss()
{
	this->m_pDamage = NULL;		// �_���[�W�G�t�F�N�g
	this->m_pShield = NULL;		// �V�[���h�G�t�F�N�g
	this->m_bShield = false;	// �V�[���h�o�����o���Ȃ���
	this->m_bDamage = false;	// �_���[�W�󂯂Ă��邩���Ȃ���
	this->m_State = STATE_NONE;	// ���
	m_bBoss_Alive = true;		// ��������
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
		pBoss->SetPos(pos);				// �ʒu
		pBoss->SetSize(size);			// �T�C�Y
		pBoss->m_fLife = nLife;			// ���C�t
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
	// ���C�t�ݒ�
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH-100.0f, 100.0f + (10 * nCntLife), 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 5.0f, 0.0f);
		m_pLife[nCntLife] = CScene2D::Create(pos, size);
		m_pLife[nCntLife]->CreateTexture("data/TEXTURE/BossLife.png");
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

	CEnemy::Update();

	// ���݂̏��
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

	if (m_fLife <= 0)
	{
		m_bBoss_Alive = false;
		Release();
	}

	// �{�X�̍X�V
	UpdateBoss();		// ���ʂ̍X�V
	DamageBoss();		// �_���[�W��ԍX�V
	NotDamageBoss();	// �_���[�WNO�I��ԍX�V
	StateUpdate();		// ��Ԃ̌��l
	MoveBoss();			// �ړ�����

	// �ʒu�̍X�V
	CScene2D::SetPos(m_pos);
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

	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		if (m_fLife < nCntLife * 10)
		{
			if (m_pLife[nCntLife] != NULL)
			{
				m_pLife[nCntLife]->Release();
				m_pLife[nCntLife] = NULL;
				break;
			}
		}
	}

	int nCntRand = 0;				// �����ۊǗp

	nCntRand = rand() % 60 + 10;	// ��������
	m_nCnt++;						// �J�E���g���Z

	int nRandEne0 = rand() % 80 + nCntRand;
	int nEnemy0 = rand() % 20 + 11;

	if ((m_nCnt % nRandEne0) == nEnemy0)
	{
		CNormalEnemy::Create(m_pos, ENEMY_SIZE, ENEMY_TYPE0, CNormalEnemy::MOVE_3);
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
			m_pDamage = CEffect::Create(m_pos, m_size);	   // ����
			m_pDamage->CreateTexture(DAMAGE_TEXTUER);	   // �e�N�X�`���̐ݒ�
			m_pDamage->SetColor(DAMAGE_COLOR);			   // �F�̐ݒ�
			m_StateCol = m_pDamage->GetColor();			   // �F�̎擾
			m_fA_Damage = 0.065f;						   // �����x���Z�p
			m_fLife -= (m_fLife*0.17f + 0.425f);
			printf("�̗́F%.3f", m_fLife);
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
			m_pShield = CEffect::Create(m_pos, SHILED_SIZE); // ����
			m_pShield->CreateTexture(SHILED_TEXTUER);		 // �e�N�X�`���ݒ�
			m_pShield->SetColor(SHILED_COLOR);				 // �F�̐ݒ�
			m_ShieldCol = m_pShield->GetColor();			 // �F�̎擾
			m_fA_Shield = 0.04f;							 // �����x���Z�p
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
	m_fMoveTime += 0.54f;
	m_pos.y = CMove::CosWave(HEIGHT_HALF, 50.0f, 65.5f, m_fMoveTime);
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
			m_fA_Shield = -0.005f;
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

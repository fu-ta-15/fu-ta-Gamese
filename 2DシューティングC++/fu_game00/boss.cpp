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
#define LIFE_POS		(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 100.0f + (10 * nCntLife), 0.0f))
#define LIFE_SIZE		(D3DXVECTOR3(10.0f, 5.0f, 0.0f))
#define CreateEnemy		(CNormalEnemy::Create)

//-----------------------------------------------------------------------------
// �ÓI�ϐ�
//-----------------------------------------------------------------------------
bool CBoss::m_bBoss_Alive = true;	// �������Ă��邩�ǂ���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBoss::CBoss() : CEnemy(OBJ_ENEMY)
{
	this->m_pDamage = NULL;			// �_���[�W�G�t�F�N�g
	this->m_pShield = NULL;			// �V�[���h�G�t�F�N�g
	this->m_bShield = false;		// �V�[���h�o�����o���Ȃ���
	this->m_bDamage = false;		// �_���[�W�󂯂Ă��邩���Ȃ���
	this->m_State = STATE_NONE;		// ���
	this->m_LifeState = LIFE_RATE_0;// ���C�t�̏��
	m_bBoss_Alive = true;			// ��������
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
	srand((unsigned)time(NULL));	// ���ݎ����̏��ŏ�����

	// ���C�t�ݒ�
	for (int nCntLife = 0; nCntLife < BOSS_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = LIFE_POS;
		D3DXVECTOR3 size = LIFE_SIZE;
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
	SummonsEnemy();

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
		if (m_fLife < nCntLife * 10)
		{// ���C�t����؂�̒l���z������
			switch (nCntLife)
			{
			case 12:
				m_LifeState = LIFE_RATE_2;
				break;
			case 7:
				m_LifeState = LIFE_RATE_5;
				break;
			case 2:
				m_LifeState = LIFE_RATE_8;
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
			m_State = STATE_NOT_DAMAGE;					   // �_���[�W�󂯕t���Ȃ����
			m_fA_Damage = 0.065f;						   // �����x���Z�p
			m_fLife -= (m_fLife * 0.17f + 0.425f);		   // �̗͂̌���

			m_pDamage = CEffect::Create(m_pos, m_size);	   // ����
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

	switch (m_LifeState)
	{
	case CBoss::LIFE_RATE_0:
		m_pos.y = CMove::CosWave(HEIGHT_HALF, 50.0f, 65.5f, m_fMoveTime);
		break;

	case CBoss::LIFE_RATE_2:

		break;

	case CBoss::LIFE_RATE_5:
		
		break;

	case CBoss::LIFE_RATE_8:
		
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

	int nCntRand = 0;				// �����ۊǗp

	nCntRand += Rand(60, 10);		// ��������
	m_nCnt++;						// �J�E���g���Z

	int nEnemy0 = Rand(19, 45);

	if ((m_nCnt % nCntRand) == nEnemy0)
	{
		CreateEnemy(m_pos, ENEMY_SIZE, ENEMY_TYPE0, MOVE3);
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

		if (m_ShieldCol.a >= 0.7f)
		{// ���l�������z������
			m_fA_Shield = -0.005f;	// ���Z
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

		if (m_StateCol.a >= 0.7f)
		{// ���l�������z������
			m_fA_Damage = -0.005f;	// ���Z
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

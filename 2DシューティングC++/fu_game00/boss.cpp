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
		pBoss->m_Effect = NULL;
		pBoss->SetPos(pos);					  // �ʒu
		pBoss->SetSize(size);				  // �T�C�Y
		pBoss->SetLife(nLife);				  // ���C�t
		pBoss->SetCol(WhiteColor);			  // �F
		pBoss->SetType(CEnemy::ENEMY_BOSS);	  // �^�C�v
		pBoss->m_State = STATE_NONE;		  // ���
		m_bBoss_Alive = true;				  // ��������
		pBoss->Init();						  // ������
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
	CScene2D::SetPos(m_pos);	// �ړ��ʂ̍X�V

	switch (m_State)
	{
	case NONE:

		break;

	case DAMAGE:
		DamageBoss();
		break;

	case NOT_DAMAGE:

		break;

	default:
		break;
	}

	// �{�X�̍X�V
	UpdateBoss();
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
// �^�C�vBOSS�̍X�V
//=============================================================================
void CBoss::UpdateBoss(void)
{
	srand((unsigned)time(NULL)); // ���ݎ����̏��ŏ�����

	m_pos = CScene2D::GetPos();

	int nCntRand = 0;

	nCntRand = rand() % 60 + 100;
	m_nCnt++;

	int nRandEne0 = rand() % 250 + nCntRand;
	int nEnemy0 = rand() % 50 + 11;

	if ((m_nCnt % nRandEne0) == nEnemy0)
	{
		CNormalEnemy::Create(m_pos, ENEMY_SIZE, ENEMY_TYPE0);
	}
}

void CBoss::DamageBoss(void)
{
	if (m_Effect == NULL)
	{
		m_Effect = CEffect::Create(m_pos, m_size);
		m_Effect->CreateTexture("data/TEXTURE/stateBoss.png");
	}

	m_nDamageCnt++;

	if ((m_nDamageCnt % 300) == 0)
	{
		m_Effect->Uninit();
		m_Effect = NULL;
		m_State = STATE_NONE;
	}


}

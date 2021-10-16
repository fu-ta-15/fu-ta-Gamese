//*****************************************************************************
//
// �G���� [enemy.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "game.h"
#include "enemy.h"
#include "normalenemy.h"
#include "boss.h"
#include "player.h"
#include "collision.h"
#include "mesh.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define BOSS_DAMAGE					(CBoss::STATE_DAMAGE)
#define BOSS_NOT_DAMAGE				(CBoss::STATE_NOT_DAMAGE)
#define BOSS_NONE					(CBoss::STATE_NONE)
#define BOSS_GET_LIFE				(CGame::GetBoss()->GetLife())
#define ENEMY_ID					(m_paEnemy[nID])
#define ENEMY_CNT					(m_paEnemy[nCnt])

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(Priority type) : CScene2D(type)
{
	m_pDeath = NULL;
	m_pField = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	CScene2D::Init(m_pos, m_size);	// �|���S���̐���
	CScene2D::SetSize(m_size);		// �T�C�Y�̐ݒ�
	CScene2D::SetCol(m_col);		// �F�̐ݒ�
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene2D::Uninit();	// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	if (this->CollisionPlayer() == true && GET_PLAYER->GetState() != CPlayer::STATE_KNOCKUP)
	{// �v���C���[�Ƃ̓����蔻��
		if (this->GetEnemyType() == ENEMY_BLACK)
		{// �G���Â����
			GET_PLAYER->SetCollEnemy(true);
			this->Release();
		}
		else if (this->GetEnemyType() == ENEMY_WHITE)
		{// �G�����邢���
			this->Release();
		}
	}
	// �e�Ƃ̓����蔻��
	CollisionEnemy();
	CollisionField();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();	// �`�揈��
}

//=============================================================================
// ��ނ��Ƃ̓����蔻���̏���
//=============================================================================
void CEnemy::CollisionEnemy(void)
{
	if (this->GetBool() == true)
	{// �����蔻�肪TRUE��������
		
		CBoss *pBoss = BOSS;	// �Q�[���V�[������擾

		// �G�̎��
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			this->DamegeLife(1);	// ���C�t����

			if (this->GetLife() == 0)
			{// ���C�t���[����������
				m_pDeath = CParticle::Create(m_pos, m_size*0.6f, CParticle::TYPE_EXPLOSION);
				m_pDeath->SetTexture("data/TEXTURE/Crystal001.png");
				m_pDeath->SetParticle(60);
				this->Release();
			}
			break;

		case ENEMY_TYPE1:

			this->Release();

			break;

		case ENEMY_TYPE2:

			if (pBoss->GetState() == BOSS_NONE && pBoss->GetShield() == false)
			{// �G���ʏ�̏�Ԃ�������
				pBoss->SetState(BOSS_DAMAGE);
			}
			else if (pBoss->GetState() == BOSS_NOT_DAMAGE)
			{// �_���[�W���󂯂Ă�����
				pBoss->SetShield(true);
			}
			break;
		}
		this->SetBool(false);	// �t���O������
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻��
//=============================================================================
bool CEnemy::CollisionPlayer(void)
{
	bool bCollision = false;

	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	bCollision = CCollision::CollisionCycle(m_pos, pos, size.x);

	return bCollision;
}

void CEnemy::CollisionField(void)
{
	CMesh* pMesh = NULL;
	pMesh = MESH_GAME;

	// ���_���̎擾
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// ��ӂ̒��S���W�ݒ�
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);

	// �O�ϓ����蔻��
	bool bOutPro = false;

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{// ���b�V���|���S����ӂ̂ݎZ�o
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// ��̒��_�Ɠ_�̊O�ϔ���
			bOutPro = CCollision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}
		if (bOutPro == true)
		{// �_����_��艺�ɂ�����
			Release();
			m_pDeath = CParticle::Create(m_pos, m_size*0.6f, CParticle::TYPE_EXPLOSION);
			m_pDeath->SetTexture("data/TEXTURE/Fog2001.png");
			m_pDeath->SetParticle(10);
			m_pDeath->SetGravity();
			m_pField = CParticle::Create(m_pos, m_size*0.4f, CParticle::TYPE_EXPLOSION);
			m_pField->SetTexture("data/TEXTURE/Crystal001.png");
			m_pField->SetParticle(50);
			m_pField->SetGravity();
			break;
		}
	}
}

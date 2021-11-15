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
#include "particle.h"


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
	// �|���S���̐���
	CScene2D::Init(m_pos, m_size);	

	// �T�C�Y�̐ݒ�
	CScene2D::SetSize(m_size);		

	// �F�̐ݒ�
	CScene2D::SetCol(m_col);		

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// �I������
	CScene2D::Uninit();	
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	// �v���C���[���ʏ�̏�Ԃł����
	if (this->CollisionPlayer() == true && CGame::GetPlayer()->GetState() != CPlayer::STATE_KNOCKUP)
	{
		// ��ޕ�
		switch (this->m_type)
		{
		case ENEMY_BLACK:

			// �����������}�𑗂�
			CGame::GetPlayer()->SetCollEnemy(true);

			// ���S�t���O�𗧂Ă�
			this->Release();

			break;

		case ENEMY_WHITE:

			// ���S�t���O�𗧂Ă�
			this->Release();

			break;

		default:
			break;
		}
	}

	// �e�Ƃ̓����蔻��
	CollisionEnemy();

	// �n�ʂƂ̓����蔻��
	CollisionField();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// �`�揈��
	CScene2D::Draw();	
}

//=============================================================================
// ��ނ��Ƃ̓����蔻���̏���
//=============================================================================
void CEnemy::CollisionEnemy(void)
{
	// �����蔻�肪TRUE��������
	if (this->GetBool() == true)
	{
		// �Q�[���V�[������擾
		CBoss *pBoss = CGame::GetBoss();	

		// �G�̎��
		switch (this->m_type)
		{
		case ENEMY_TYPE0:

			// ���C�t����
			this->DamegeLife(1);	

			// ���C�t���[����������
			if (this->GetLife() == 0)
			{
				// �I������
				CScene2D::Uninit();

				// �p�[�e�B�N���̔���
				Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");
			}
			break;

		case ENEMY_TYPE1:

			// �I������
			CScene2D::Uninit();

			// �p�[�e�B�N���̔���
			Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");

			break;

		case ENEMY_TYPE2:

			// �G���ʏ�̏�Ԃ�������
			if (pBoss->GetState() == BOSS_NONE)
			{
				// �_���[�W��H����Ă����Ԃɂ���
				pBoss->SetState(BOSS_DAMAGE);
			}
			break;
		}

		// �t���O��߂�
		this->SetBool(false);	
	}
}

//=============================================================================
// �v���C���[�Ƃ̓����蔻��
//=============================================================================
bool CEnemy::CollisionPlayer(void)
{
	// �v���C���[�̈ʒu�擾
	D3DXVECTOR3 pos = CGame::GetPlayer()->GetPos();

	// �v���C���[�̃T�C�Y�擾
	D3DXVECTOR3 size = CGame::GetPlayer()->GetSize();

	// �����蔻��̌��ʂ�Ԃ�
	return  Collision::CollisionCycle(m_pos, pos, size.x);
}

//=============================================================================
// �n�ʂƂ̓����蔻��
//=============================================================================
void CEnemy::CollisionField(void)
{
	// ���b�V���̏��擾�i�Q�[�����[�h�́j
	CMesh* pMesh = CGame::GetMesh();

	// ���_���̎擾
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// ��ӂ̒��S���W�ݒ�
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{// ���b�V���|���S����ӂ̂ݎZ�o
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// ��̒��_�Ɠ_�̊O�ϔ���
			if (Collision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA))
			{// �_����_��艺�ɂ�����

				// �I������
				Uninit();

				// �p�[�e�B�N���̔���
				Particle::SetParticle(m_pos, m_size, 10, Particle::TYPE_EXPLOSION, "data/TEXTURE/Crystal001.png");
				break;
			}
		}
	}
}

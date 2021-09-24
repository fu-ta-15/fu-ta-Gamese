//*****************************************************************************
//
// �o���b�g���� [bullet.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "bullet.h"
#include "game.h"
#include "collision.h"
#include "enemy.h"

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
CEnemy *CBullet::m_pBoss = NULL;

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define BULLET_TEXTURE		("data/TEXTURE/FlowerShot.png")
#define P_ENEMY				(pEnemy[nCntEnemy])

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet() : CScene2D(OBJ_BULLET)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// �o���b�g�̐���
//=============================================================================
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move, BULLET type)
{
	/* �o���b�g�̗v�f��ݒ� */
	CBullet* pBullet = new CBullet;		// �C���X�^���X����

	pBullet->m_pos = pos;					// �ʒu��ݒ�
	pBullet->m_size = size;					// �T�C�Y��ݒ�
	pBullet->m_move = move;					// �ړ��ʂ�ݒ�
	pBullet->m_type = type;					// �e�̃^�C�v
	pBullet->CreateTexture(BULLET_TEXTURE);	// �e�N�X�`��
	pBullet->m_Collision = false;			// �����蔻��
	pBullet->m_bUse = true;					// �����m�F
	pBullet->Init();						// ������

	return pBullet;						// �o���b�g����Ԃ�
}

//=============================================================================
// ������
//=============================================================================
HRESULT CBullet::Init(void)
{
	CScene2D::Init(m_pos,m_size);	// �|���S���̏���������

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();		// �|���S���̏I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	// ��ʂɑ��݂��Ă��邩���Ȃ���
	if (m_bUse == true)
	{// ���Ă���ꍇ
		
		CScene2D::Update();				// �|���S���̍X�V����
		
		m_pos.x += m_move.x;			// �ʒu�Ɉړ��ʂ����Z

		CScene2D::SetPos(m_pos);		// �ʒu�̍X�V

		switch (m_type)
		{// �N�̃o���b�g��
		case CBullet::BULLET_PLAYER:	// �v���C���[�̒e
			m_Collision = CollisionBullet(m_pos,m_size);	// �����蔻��m�F
			break;

		default:
			break;
		}

		if (m_Collision == true)
		{// ���������ꍇ 
			m_bUse = false;				// ���݂�����
			CScene2D::Uninit();		// �|���S���̏I������
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();	// �|���S���̕`��
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CBullet::CollisionBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	D3DXVECTOR3 posEnemy;							// �G�̈ʒu
	D3DXVECTOR3 sizeEnemy;							// �G�̃T�C�Y
	int EnemyNum = CScene::GetObjeNum(OBJ_ENEMY);	// ��ʂɂ���G�̐��̎擾
	CEnemy **pEnemy = CEnemy::GetEnemy();			// �G�̏��擾

	if (EnemyNum == 0)
	{/* ��ʂɂ���G���[���̏ꍇ */
		m_Collision = false;	// �����蔻��͖���
	}
	else if (EnemyNum >= 1)
	{/* �G����̈ȏア��ꍇ */
		for (int nCntEnemy = 0; nCntEnemy < MAX_OBJECT; nCntEnemy++)
		{
			posEnemy = CScene::GetPos(OBJ_ENEMY, nCntEnemy);	 // �G�̈ʒu���擾
			sizeEnemy = CScene::GetSize(OBJ_ENEMY, nCntEnemy);	 // �G�̃T�C�Y���擾

			if (CCollision::CollisionCycle(m_pos, posEnemy, sizeEnemy.x) == true && P_ENEMY->GetEnemyType() != ENEMY_TYPE1)
			{/* �G�͈̔͂ɒe�����݂����� */
				CEnemy::CollisionEnemy(nCntEnemy);			// ���������G�̓����蔻�菈����
				m_Collision = true;							// �����蔻��͗L
				break;
			}
		}
	}
	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f)
	{// ��ʊO����
		m_Collision = true;				// ���݂�����
	}

	return m_Collision;	// ���茋�ʂ�Ԃ�
}


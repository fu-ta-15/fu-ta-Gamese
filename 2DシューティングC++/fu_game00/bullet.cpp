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

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define BULLET_TEXTURE		("data/TEXTURE/Spark002.png")

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
CBullet * CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move)
{
	/* �o���b�g�̗v�f��ݒ� */
	CBullet* pBullet = new CBullet;		// �C���X�^���X����

	pBullet->m_pos = pos;					// �ʒu��ݒ�
	pBullet->m_size = size;					// �T�C�Y��ݒ�
	pBullet->m_move = move;					// �ړ��ʂ�ݒ�
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

		m_Collision = CollisionBullet(m_pos,m_size);	// �����蔻��m�F

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
	if (m_bUse == true)
	{// ���Ă���ꍇ
		CScene2D::Draw();	// �|���S���̕`��
	}
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CBullet::CollisionBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	D3DXVECTOR3 posEnemy;							// �G�̈ʒu
	D3DXVECTOR3 sizeEnemy;							// �G�̃T�C�Y

	CScene *pScene = CScene::GetScene(OBJ_ENEMY);

	m_Collision = false;	// �����蔻��͖���

	for (int nCntEnemy = 0; nCntEnemy < MAX_OBJECT; nCntEnemy++)
	{
		if (pScene != NULL)
		{
			CScene *pSceneNext = pScene->GetSceneNext();
			posEnemy = pScene->GetPos();
			sizeEnemy = pScene->GetSize();

			if (Collision::CollisionCycle(m_pos, posEnemy, sizeEnemy.x) == true)
			{/* �G�͈̔͂ɒe�����݂����� */
				pScene->SetBool(true);
				m_Collision = true;							// �����蔻��͗L
				break;
			}
			pScene = pSceneNext;
		}
	}
	if (m_pos.x - m_size.x > SCREEN_WIDTH || m_pos.x + m_size.x < 0.0f)
	{// ��ʊO����
		m_Collision = true;				// ���݂�����
	}

	return m_Collision;	// ���茋�ʂ�Ԃ�
}


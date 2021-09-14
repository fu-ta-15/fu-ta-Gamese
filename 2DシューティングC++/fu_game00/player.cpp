//*****************************************************************************
//
// �v���C���[���� [player.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "player.h"
#include "keyinput.h"
#include "manager.h"
#include "bullet.h"
#include "collision.h"
#include "mesh.h"
#include "game.h"
#include "tutorial.h"
#include "move.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define PLAYER_MOVE					(5.0f)
#define PLAYER_JUNP					(20.0f)
#define PLAYER_BULLET				(CBullet::BULLET_PLAYER)
#define PLAYER_RETURN_FLOOR			(SCREEN_HEIGHT - m_size.y)
#define MOVE_DECELERATION			(D3DXVECTOR3(1.0f, 0.002f, 0.0f))

#define BULLET_SIZE					(D3DXVECTOR3(4.0f, 4.0f, 0.0f))
#define BULLET_MOVE					(10.0f)
#define BULLET_MOVE_RIGHT			(D3DXVECTOR3(BULLET_MOVE, 0.0f, 0.0f))
#define BULLET_MOVE_LEFT			(D3DXVECTOR3(-BULLET_MOVE, 0.0f, 0.0f))

#define GRAVITY						(1.0f)
#define PLAYER_COLL_POS				(D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, m_pos.z))

#define MESH_GAME					(CGame::GetMesh())
#define MESH_TUTORIAL				(CTutorial::GetMesh())

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CScene2D(OBJ_PLAYER)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// �v���C���[�̐���
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// �v���C���[�̃|�C���^�ϐ�����
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULL�`�F�b�N
		pPlayer = new CPlayer;			// �C���X�^���X����
		pPlayer->m_pos = pos;			// �ʒu�̐ݒ�
		pPlayer->m_size = size;			// �T�C�Y�̐ݒ�
		pPlayer->m_fG = 0.0f;			// �d��
		pPlayer->m_col = WhiteColor;	// ���J���[
		pPlayer->m_bUse = true;			// �g�p��
		pPlayer->Init();				// ����������
	}
	// ����Ԃ�
	return pPlayer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// �|���S���̐���
	CScene2D::Init(m_pos, m_size);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �|���S���̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �L�[�{�[�h���擾
	CKey *pKey = CManager::GetKey();

	// �v���C���[�̃A�N�V����
	PlayerAction(m_pos,m_move);	 // �W�����v��ړ���

	// �d�͂̉��Z
	m_move.y += GRAVITY;
	m_pos += m_move;

	// �ړ��E��]�ʂ̌���
	m_move = CMove::MoveControl(m_move, MOVE_DECELERATION);

	PosControl(m_pos,m_move);	// �ʒu�̐���

	CScene2D::SetUse(m_bUse);	// ���݂��Ă���
	CScene2D::SetPos(m_pos);	// �ʒu�̐ݒ�i�X�V�j
	CScene2D::SetCol(m_col);	// �F�̐ݒ�i�X�V�j
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �|���S���̕`��
	CScene2D::Draw();
}

//=============================================================================
// �v���C���[�̈ʒu����
//=============================================================================
void CPlayer::PosControl(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	CMesh* pMesh = NULL;
	
	// ���b�V���|���S���̏��擾
	switch (CManager::GetMode())
	{
		// �`���[�g���A��
	case CManager::MODE_TUTORIAL:	
		pMesh = MESH_TUTORIAL;
		break;
		// �Q�[��
	case CManager::MODE_GAME:		
		pMesh = MESH_GAME;
		break;
	default:
		break;
	}

	// ���_���̎擾
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// �ʒu�ƈړ��ʂ̑��
	m_pos = pos;
	m_move = move;

	// ��ӂ̒��S���W�ݒ�
	D3DXVECTOR3 posA = PLAYER_COLL_POS;

	// �O�ϓ����蔻��
	bool bOutPro = false;

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum()/2; nCnt++)
	{// ���b�V���|���S����ӂ̂ݎZ�o
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// ��̒��_�Ɠ_�̊O�ϔ���
			bOutPro = CCollision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}
		if (bOutPro == true)
		{// �_����_��艺�ɂ�����
			D3DXVECTOR3 m_posOld = CCollision::WaveCollision(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA, CCollision::TYPE_COL_Y);	// �߂������Z�o
			m_move.y = 0.0f;					// �d�̓[��
			m_bJunp = false;					// �W�����v�\
			m_pos.y = m_posOld.y - m_size.y;	// ��ʓ��܂ňړ�������
			break;
		}
	}

	if (m_pos.y + m_size.y > SCREEN_HEIGHT)
	{// �v���C���[����ʉ����z���悤�Ƃ�����
		m_move.y = 0.0f;					// �d�̓[��
		m_bJunp = false;					// �W�����v�\
		m_pos.y = SCREEN_HEIGHT - m_size.y;	// ���̈ʒu�̖߂�
	}
	if (m_pos.x - m_size.x < 0)
	{// ����ʒ[���
		m_pos.x = 0 + m_size.x;				// ���̈ʒu�̖߂�
	}
	if (m_pos.x + m_size.x > SCREEN_WIDTH)
	{// �E��ʒ[���
		m_pos.x = SCREEN_WIDTH - m_size.x;	// ���̈ʒu�̖߂�
	}
}

//=============================================================================
// �v���C���[�̍s��
//=============================================================================
void CPlayer::PlayerAction(const D3DXVECTOR3 pos, const D3DXVECTOR3 move)
{
	// �L�[�{�[�h���擾
	CKey *pKey = CManager::GetKey();

	// �ʒu�̑��
	m_pos = pos;
	m_move = move;

	/* �v���C���[�̈ړ� */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D) == true)	// �v���X�ED�������ꂽ�Ƃ�
	{
		m_move.x += PLAYER_MOVE;	// �ړ��ʂ̍X�V
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A) == true)	// �v���X�EA�������ꂽ�Ƃ�
	{
		m_move.x -= PLAYER_MOVE;	// �ړ��ʂ̍X�V
	}

	/* �v���C���[�̃W�����v */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	// �g���K�[�ESPACE�������ꂽ�Ƃ�
	{
		if (m_bJunp == false)
		{// �W�����v�\�Ȃ��
			m_move.y -= PLAYER_JUNP;// �ړ��ʂ̍X�V
			m_bJunp = true;			// �W�����v��
		}
	}

	/* �v���C���[�̗������� */
	if (m_bJunp == true)
	{// �W�����v�����A�d�͂��v���X�̎�
		if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	// �g���K�[�ESPACE�������ꂽ�Ƃ�
		{
			m_move.y -= 0.6f;	// �d�͂̌���
		}
	}

	/* �e�̈ړ� */
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6) == true)	// �g���K�[�EK�������ꂽ�Ƃ�
	{
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_RIGHT, PLAYER_BULLET);	// �o���b�g�̐���
		printf("�ł��܂����B\n");
	}
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD4) == true)	// �g���K�[�EK�������ꂽ�Ƃ�
	{
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_LEFT, PLAYER_BULLET);	// �o���b�g�̐���
	}
}


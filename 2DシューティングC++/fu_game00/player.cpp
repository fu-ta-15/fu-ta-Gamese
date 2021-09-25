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
#define MOVE_DECELERATION			(D3DXVECTOR3(1.0f, 0.02f, 0.0f))

#define BULLET_SIZE					(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
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
		pPlayer->m_fStayTime = 0.0f;	// �Ƃǂ܂��Ă��鎞��
		pPlayer->m_col = WhiteColor;	// ���J���[
		pPlayer->m_bUse = true;			// �g�p��
		pPlayer->m_pShield = NULL;		// �V�[���h
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

	m_tex = D3DXVECTOR2(2.0f, 0.0f);
	m_number = D3DXVECTOR2(0.0f, 0.0f);
	m_nAnimeCnt = 0;

	// �|���S���̐���
	CScene2D::Init(m_pos, m_size);
	CScene2D::CreateTexture("data/TEXTURE/player0.png");
	CScene2D::SetTex(m_tex, m_number);
	m_pShield = CEffect::Create(m_pos, m_size * 2);
	m_pShield->CreateTexture("data/TEXTURE/Shockwave.png");
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

	// �A�N�V����
	PlayerAction();	

	// �ړ�
	PlayerMove();

	// �ړ��ʂ̉��Z
	m_move.y += GRAVITY;
	m_pos += m_move;

	// �ړ��ʂ̐���
	PlayerMoveControl();

	// �ʒu�̐���
	PosControl();	

	// �A�j���[�V����
	PlayerAnime();

	CScene2D::SetUse(m_bUse);	// ���݂��Ă���
	CScene2D::SetPos(m_pos);	// �ʒu�̐ݒ�i�X�V�j
	CScene2D::SetCol(m_col);	// �F�̐ݒ�i�X�V�j
	m_pShield->SetPos(m_pos);
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
void CPlayer::PosControl(void)
{
	FieldControl();

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
// �v���C���[�̃A�N�V����
//=============================================================================
void CPlayer::PlayerAction(void)
{
	// �L�[�{�[�h���擾
	CKey *pKey = CManager::GetKey();

	/* �e�̈ړ� */
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6) == true)	// �g���K�[�EK�������ꂽ�Ƃ�
	{
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_RIGHT, PLAYER_BULLET);	// �o���b�g�̐���
	}
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD4) == true)	// �g���K�[�EK�������ꂽ�Ƃ�
	{
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_LEFT, PLAYER_BULLET);	// �o���b�g�̐���
	}
}

//=============================================================================
// �v���C���[�̈ړ�
//=============================================================================
void CPlayer::PlayerMove(void)
{
	// �L�[�{�[�h���擾
	CKey *pKey = CManager::GetKey();

	/* �v���C���[�̈ړ� */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D) == true)	// �v���X�ED�������ꂽ�Ƃ�
	{
		m_nAnimeCnt++;
		m_move.x += PLAYER_MOVE;	// �ړ��ʂ̍X�V
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A) == true)	// �v���X�EA�������ꂽ�Ƃ�
	{
		m_nAnimeCnt++;
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
}

//=============================================================================
// �v���C���[�̈ړ��ʂ̃R���g���[��
//=============================================================================
void CPlayer::PlayerMoveControl(void)
{
	// �ړ��E��]�ʂ̌���
	m_move = CMove::MoveControl(m_move, MOVE_DECELERATION);

	if (m_move.x == 0.0f && m_bJunp != true)
	{// �ړ����Ă��Ȃ�������
		m_fStayTime++;	// �J�E���gUP
	}
	if (m_move.x != 0.0f || m_bJunp != false)
	{// �ړ����Ă�����
		m_fStayTime = 0.0f;
	}
	if (m_fStayTime >= 120.0f)
	{// ���Ԃ�������
		m_bStay = true;
	}
}

//=============================================================================
// �t�B�[���h��̃v���C���[����
//=============================================================================
void CPlayer::FieldControl(void)
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

	// ��ӂ̒��S���W�ݒ�
	D3DXVECTOR3 posA = PLAYER_COLL_POS;

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
			D3DXVECTOR3 m_posOld = CCollision::WaveCollision(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA, CCollision::TYPE_COL_Y);	// �߂������Z�o
			m_move.y = 0.0f;					// �d�̓[��
			m_bJunp = false;					// �W�����v�\
			m_pos.y = m_posOld.y - m_size.y;	// ��ʓ��܂ňړ�������
			break;
		}
	}
}

//=============================================================================
// �v���C���[�̃A�j���[�W����
//=============================================================================
void CPlayer::PlayerAnime(void)
{
	if ((m_nAnimeCnt % 10) == 1)
	{
		m_number.x++;
		if (((int)m_number.x + 1 % 2) == 0)
		{
			m_number.x = 0.0f;
		}
		CScene2D::SetTex(m_tex, m_number);
	}
}

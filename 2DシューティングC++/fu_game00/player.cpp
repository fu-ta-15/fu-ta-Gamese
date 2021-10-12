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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CScene2D(OBJ_PLAYER)
{
	this->m_col = WhiteColor;				// �F���
	this->m_fLife = PLAYER_LIFE;			// �̗�
	this->m_state = STATE_NONE;				// ���
	this->m_bUse = true;					// �g�p��
	this->m_nBullet = PLAYER_BULLET_STOCK;	// �e�̃X�g�b�N
	this->m_pShield = NULL;					// �V�[���h
	this->m_nBulletTime = 0;
	this->m_bAlive = true;					// ������
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

	// �v���C���[�\���ݒ�
	CScene2D::Init(m_pos, m_size);
	CScene2D::CreateTexture("data/TEXTURE/player0.png");
	CScene2D::SetTex(m_tex, m_number);

	// ���C�t�̐ݒ�
	for (int nCntLife = 0; nCntLife < PLAYER_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(50.0f, 190.0f - (10 * nCntLife), 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f,5.0f, 0.0f);
		m_pLife[nCntLife] = CScene2D::Create(pos, size);
		m_pLife[nCntLife]->CreateTexture("data/TEXTURE/lifeBlock.png");
	}

	// �E�F�|���̐ݒ�
	for (int nCntWeapon = 0; nCntWeapon < PLAYER_BULLET_STOCK; nCntWeapon++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(80.0f, 100.0f + (10 * nCntWeapon), 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 5.0f, 0.0f);
		m_pWeapon[nCntWeapon] = CScene2D::Create(pos, size);
		m_pWeapon[nCntWeapon]->CreateTexture("data/TEXTURE/BulletBlock.png");
	}

	// �V�[���h�̐ݒ�
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

	if (m_state == STATE_NONE)
	{// �����Ȃ���Ԃ̎�
		PlayerAction();	// �A�N�V����
	}

	// ���C�t�̊Ǘ�
	PlayerLife();

	// �ړ��ʂ̉��Z
	m_move.y += GRAVITY;

	// �ړ��ʂ̉��Z
	m_pos += m_move;

	// �ړ��ʂ̐���
	PlayerMoveControl();

	// �ʒu�̐���
	PosControl();	

	// �A�j���[�V����
	PlayerAnime();

	// ��ԊǗ�
	PlayerState();

	CScene2D::SetUse(m_bUse);	// ���݂��Ă���
	CScene2D::SetPos(m_pos);	// �ʒu�̐ݒ�i�X�V�j
	CScene2D::SetCol(m_col);	// �F�̐ݒ�i�X�V�j
	m_pShield->SetPos(m_pos);	// �V�[���h�̈ʒu�X�V
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
	// �n�ʂ̓����蔻��
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

	// �e�����p�̃^�C��
	m_nBulletTime++;

	// �e�̕���
	if ((m_nBulletTime % 35) == 0)
	{
		for (int nCntWeapon = PLAYER_BULLET_STOCK; nCntWeapon > 0; nCntWeapon--)
		{
			if (m_pWeapon[nCntWeapon - 1]->GetUse() != true)
			{
				m_nBullet += 1;
				m_pWeapon[nCntWeapon - 1]->SetUse(true);
				break;
			}
		}
	}

	// �ړ�
	PlayerMove();

	// �e�̔���
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6) == true && m_nBullet > 0)	
	{// �g���K�[�ENUM6 �������ꂽ�Ƃ�
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_RIGHT);	// �o���b�g�̐���
		PlayerBullet();											// �v���C���[�̒e����
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
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D) == true)	
	{// �v���X�ED�������ꂽ�Ƃ�
		m_nAnimeCnt++;
		m_move.x += PLAYER_MOVE;	// �ړ��ʂ̍X�V
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A) == true)	
	{// �v���X�EA�������ꂽ�Ƃ�
		m_nAnimeCnt++;
		m_move.x -= PLAYER_MOVE;	// �ړ��ʂ̍X�V
	}

	/* �v���C���[�̃W�����v */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	
	{// �g���K�[�ESPACE�������ꂽ�Ƃ�
		if (m_bJunp == false)
		{// �W�����v�\�Ȃ��
			m_move.y -= PLAYER_JUNP;// �ړ��ʂ̍X�V
			m_bJunp = true;			// �W�����v��
		}
	}
	/* �v���C���[�̗������� */
	if (m_bJunp == true)
	{// �W�����v�����A�d�͂��v���X�̎�
		if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	
		{// �v���X�ESPACE�������ꂽ�Ƃ�
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
			if (pVtx[nCnt].pos.y > SCREEN_HEIGHT &&  pVtx[nCnt + 1].pos.y > SCREEN_HEIGHT)
			{
				m_bFall = true;
			}
			else
			{
				m_bFall = false;
			}
			if (m_bFall == false)
			{
				D3DXVECTOR3 m_posOld = CCollision::WaveCollision(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA, CCollision::TYPE_COL_Y);	// �߂������Z�o
				m_move.y = 0.0f;					// �d�̓[��
				m_bJunp = false;					// �W�����v�\
				m_pos.y = m_posOld.y - m_size.y;	// ��ʓ��܂ňړ�������
			}
			break;
		}
	}
}

//=============================================================================
// �v���C���[�̃A�j���[�W����
//=============================================================================
void CPlayer::PlayerAnime(void)
{
	if ((m_nAnimeCnt % 10) == 0)
	{// �J�E���g���P�O���܂�P�̎�
		m_number.x += 1;	// �e�N�X�`�����W���Z
		m_nAnimeCnt = 0;
		if (((int)m_number.x % 2) == 0)
		{// �e�N�X�`�����W���߂��Ă�����
			m_number.x = 0.0f;	// �����l�ɖ߂�
		}
		// �e�N�X�`�����W�̔��f
		CScene2D::SetTex(m_tex, m_number);
	}
}

//=============================================================================
// �v���C���[�̏��
//=============================================================================
void CPlayer::PlayerState(void)
{
	if (m_state == STATE_NONE)
	{// �ʏ��Ԃ̏ꍇ
		m_col = WhiteColor;	// �F��߂�
		m_bDamage = false;	// �_���[�W����OFF
	}

	if (m_pShield->GetUse() == true && m_bCollEnemy == true)
	{// �V�[���h������ꍇ
		m_pShield->SetUse(false);	// �V�[���h�폜
		m_bCollEnemy = false;		// �����蔻������Ƃɖ߂�
	}

	if (m_pShield->GetUse() == false && m_bCollEnemy == true)
	{// �V�[���h���Ȃ�������
		m_state = STATE_KNOCKUP;					// �m�b�N�A�b�v���
		m_bDamage = true;							// �_���[�W����ON
		m_KnockUpPos.x = 0.0f;						// �m�b�N�A�b�v�̈ʒu
		m_KnockUpPos.y = m_pos.y + 40.0f;			// �m�b�N�A�b�v�̈ʒu
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// �_���[�W�̐F�ɕύX
		m_bCollEnemy = false;						// �����蔻������Ƃɖ߂�
	}

	if (m_state == STATE_KNOCKUP)
	{// �m�b�N�A�b�v��Ԃ̎�
		DamagePlayer();	// �m�b�N�A�b�v��Ԃ̏���
	}
}

//=============================================================================
// �v���C���[���g�Ƀ_���[�W���󂯂�����
//=============================================================================
void CPlayer::DamagePlayer(void)
{
	m_nDamageCnt++;	// �J�E���g�A�b�v
	
	// �ړI�̏ꏊ�փm�b�N�A�b�v
	m_pos.x = CMove::TargetPosMove(D3DXVECTOR3(m_KnockUpPos.x, 0.0f, 0.0f), m_pos, 0.035f).x;
	m_pos.y = CMove::TargetPosMove(D3DXVECTOR3(0.0f, m_KnockUpPos.y, 0.0f), m_pos, 0.015f).y;

	if ((m_nDamageCnt % 15) == 0)
	{// �J�E���g�����܂ŗ�����
		m_fLife -= (m_fLife * 0.056f + 1.2f);
		m_state = STATE_NONE;	 // ��Ԃ�߂�
		m_nDamageCnt = 0.0f;	 // �J�E���g������������
	}
}

void CPlayer::PlayerLife(void)
{
	for (int nCntLife = 0; nCntLife < PLAYER_LIFE_STOCK; nCntLife++)
	{
		if (m_fLife < nCntLife * 10)
		{// ���C�t�����̒l�𒴂�����
			if (m_pLife[nCntLife] != NULL)
			{// ���C�t���폜
				m_pLife[nCntLife]->Release();
				m_pLife[nCntLife] = NULL;
				break;
			}
		}
	}
	if (m_fLife < 0)
	{// ���C�t���[���ɂȂ�����
		m_bAlive = false;
		m_bUse = false;
	}
}

void CPlayer::PlayerBullet(void)
{
	for (int nCntWeapon = 0; nCntWeapon < PLAYER_BULLET_STOCK; nCntWeapon++)
	{
		if (m_pWeapon[nCntWeapon]->GetUse() != false)
		{// �g�p�������̃|���S�����c���Ă�����
			m_nBullet -= 1;							// �X�g�b�N�̌��炷
			m_pWeapon[nCntWeapon]->SetUse(false);	// ���̃|���S�����g�p���Ă��Ȃ���Ԃɂ���
			break;
		}
	}
}

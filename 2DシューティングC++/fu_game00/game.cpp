//*****************************************************************************
//
// �Q�[������ [game.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "game.h"
#include "scene2D.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "time.h"
#include "boss.h"
#include "mesh.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define PLAYER_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define PLAYER_POS		(D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 0.0f))

#define SCORE_POS		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))
#define SCORE_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))

#define TIME_POS		(D3DXVECTOR3(SCREEN_WIDTH / 3, 50.0f, 0.0f))
#define TIME_SIZE		(D3DXVECTOR3(20.0f, 30.0f, 0.0f))
#define TIME_SET		(20)

#define BG_SIZE			(D3DXVECTOR3(WIDTH_HALF,HEIGHT_HALF,0.0f))

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
CPlayer*	CGame::m_pPlayer	= NULL;
CScore*		CGame::m_pScore		= NULL;
CTime*		CGame::m_pTime		= NULL;
CBoss*		CGame::m_pBoss		= NULL;
CScene2D*	CGame::m_pBg		= NULL;
CMesh*		CGame::m_pMesh		= NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame() : CScene(OBJ_NONE)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// �Q�[���̐���
//=============================================================================
CGame * CGame::Create(void)
{
	// �������m��
	CGame* pGame = new CGame;

	// ����������
	pGame->Init();

	return pGame;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CGame::Init(void)
{
	m_pBg = CScene2D::Create(CENTER_POS, BG_SIZE);


	m_pMesh = CMesh::Create(40, 0, D3DXVECTOR3(0.0f, 768 - 100, 0.0f), D3DXVECTOR3(900.0f, 100.0f, 0.0f));
	m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE);

	m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);
	m_pTime = CTime::Create(TIME_POS, TIME_SIZE, TIME_SET);
	m_pBoss = CBoss::Create(BOSS_POS, BOSS_SIZE, BOSS_LIFE);

	m_pBoss->CreateTexture("data/TEXTURE/Boss.png");
	m_pBg->CreateTexture("data/TEXTURE/BG.png");
	m_pMesh->CreateTexture("data/TEXTURE/Field.png");

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
	CKey *pKey = CManager::GetKey();

	/* �t�F�[�h���� */
	if (pKey->GetState(CKey::STATE_RELEASE, DIK_RETURN))
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}
	if (m_pTime->GetTime() == 0 || CBoss::GetAlive() == false)
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

	/* �X�R�A�̉��Z */
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_Y))
	{
		m_pScore->AddScore(12);
	}

	/* �^�C�}�[���� */
	int nCntTime = m_pTime->AddCnt(1);

	if ((nCntTime % 60) == 0)
	{
		m_pTime->AddTime(-1);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGame::Draw(void)
{
	
}

//*****************************************************************************
//
// �^�C�g������ [title.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "title.h"
#include "scene2D.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"
#include "mesh.h"
#include "move.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define TITLE_BG_SIZE		(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF, 0.0f))

#define TITLE_BUTTON_POS	(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF+250.0f, 0.0f))
#define TITLE_BUTTON_SIZE	(D3DXVECTOR3(150.0f, 50.0f, 0.0f))

#define TITLE_LOGO_POS		(D3DXVECTOR3(WIDTH_HALF-300, HEIGHT_HALF-500.0f, 0.0f))
#define TITLE_LOGO_SIZE		(D3DXVECTOR3(600.0f, 400.0f, 0.0f))


//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
CScene2D *CTitle::m_paTitleUI[UI_MAX] = {};
CMesh *CTitle::m_pTitleLogo = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle() : CScene(OBJ_NONE)
{
	m_ButtonCol = WhiteColor;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// �^�C�g���̐���
//=============================================================================
CTitle * CTitle::Create(void)
{
	// �������m��
	CTitle* pTitle = new CTitle;

	// ����������
	pTitle->Init();

	return pTitle;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CTitle::Init(void)
{
	m_paTitleUI[UI_BG] = CScene2D::Create(CENTER_POS, TITLE_BG_SIZE);
	m_paTitleUI[UI_BUTTON] = CScene2D::Create(TITLE_BUTTON_POS, TITLE_BUTTON_SIZE);
	m_pTitleLogo = CMesh::Create(90, 0, TITLE_LOGO_POS, TITLE_LOGO_SIZE);

	m_paTitleUI[UI_BG]->CreateTexture("data/TEXTURE/BG.jpg");
	m_paTitleUI[UI_BUTTON]->CreateTexture("data/TEXTURE/Next_Mode.png");
	m_pTitleLogo->CreateTexture("data/TEXTURE/TitleUI.png");
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	CKey *pKey = CManager::GetKey();

	m_nCntTime++;
	char *cC = NULL;

	m_pTitleLogo->WaveMove(50.0f, 10.0f, 100.0f, m_nCntTime);

	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE))
	{
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	}
	ButtonUI();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// Button�̏���
//=============================================================================
void CTitle::ButtonUI(void)
{
	m_paTitleUI[UI_BUTTON]->SetCol(m_ButtonCol);// ���l�̍X�V

	m_ButtonCol.a += AddCol; // ���l�̌��Z�Ɖ��Z

	if (m_ButtonCol.a >= 1.0f)
	{// ���Z������l
		AddCol = -0.02f;
	}
	if (m_ButtonCol.a <= 0.0f)
	{// ���Z������l
		AddCol = 0.02f;
	}
}

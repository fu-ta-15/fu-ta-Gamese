//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"

#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "input_mouse.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "texture.h"
#include "base.h"
#include "keep_object.h"
#include "shadow.h"

#include "title.h" 
#include "game.h"
#include "result.h"
#include "mode_select.h"
#include "preparation_player.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;
CSound *CManager::m_pSound = NULL;
CBase *CManager::m_pBase = NULL;					
CManager::MODE CManager::m_mode = MODE_TITLE;
CFade *CManager::m_pFade = NULL;
bool CManager::m_bPause = false;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CTexture *CManager::m_pTexture = NULL;
CKeepObject *CManager::m_pKeepObject = NULL;

//=============================================================================
// CManager�̃R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// CManager�̃f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	// ���ݎ����ɂ���ă����_���ɐݒ肷��
	srand((unsigned)time(NULL));

	// �����_���[�̏���������
	m_pRenderer = new CRenderer;

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	// �L�[�{�[�h�̏���������
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// �W���C�p�b�h���̏���������
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Init(hInstance, hWnd);
	}

	// �}�E�X���̏���������
	m_pInputMouse = new CInputMouse;

	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Init(hInstance, hWnd);
	}

	// �T�E���h�̏���������
	m_pSound = new CSound;

	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}

	// �e�N�X�`���Ǘ��̐���
	m_pTexture = new CTexture;

	if (m_pTexture != NULL)
	{
		m_pTexture->Load();
	} 

	// ���[�h�̊m��
	CBase *pBase = SetMode();

	// �t�F�[�h�̏���������
	m_pFade = new CFade;

	if (m_pFade != NULL)
	{
		m_pFade->Init(pBase);
	}

	// ���C�g�̏�����
	m_pLight = new CLight;

	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

	// �f�o�b�N�\���̐���
	CDebugProc *pDebugProc = new CDebugProc;

	if (pDebugProc != NULL)
	{
		pDebugProc->Init();
	}

	// �f�[�^�ۑ��p�N���X�̐���
	m_pKeepObject = new CKeepObject;

	if (m_pKeepObject != NULL)
	{
		m_pKeepObject->Init();
	}

	// ���[�h�̌Ăяo��
	SetMode(pBase);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �T�E���h�̏I������
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// �t�F�[�h�̏I������
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// �����_���̏I������
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �I�u�W�F�N�g�̏I������
	CScene::ReleaseAll();

	// �x�[�X�̔j��
	if (m_pBase = NULL)
	{
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = NULL;
	}

	// �e�N�X�`���̃A�����[�h
	if (m_pTexture != NULL)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	// �}�E�X�̏I������
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// �W���C�p�b�h�̏I������
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// �L�[�{�[�h�̏I������
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �W���C�p�b�h�̍X�V����
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	// �}�E�X�̍X�V����
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	// �����_���̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// �x�[�X�̍X�V����
	if (m_pBase != NULL)
	{
		m_pBase->Update();
	}

	// �I�u�W�F�N�g�̍X�V����
	CScene::UpdateAll();

	// �t�F�[�h�̍X�V����
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	if (m_pKeepObject != NULL)
	{
		m_pKeepObject->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �����_���̕`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(CBase *pBase)
{
	// �I�u�W�F�N�g�̍폜
	CScene::ReleaseAll();

	// �x�[�X�̔j��
	if (m_pBase != NULL)
	{
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = NULL;
	}

	// ���[�h�̈ڍs
	m_pBase = pBase;

	//�V�[���̏�����
	if (m_pBase != NULL)
	{
		m_pBase->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	}
}

//=============================================================================
// ���[�h�̊m��
//=============================================================================
CBase *CManager::SetMode(void)
{
	CBase *pBase = NULL;

	// ���[�h�̊m��
	switch (m_mode)
	{
	case MODE_TITLE:
		pBase = new CTitle;
		break;

	case MODE_MODESELECT:
		pBase = new CModeSelect;
		break;

	case MODE_PREPARATION:
		pBase = new CPreparationPlayer;
		break;

	case MODE_GAME:
		pBase = new CGame;
		break;

	case MODE_RESULT:
		pBase = new CResult;
		break;
	}

	return pBase;
}
//*****************************************************************************
//
// �}�l�[�W���[���� [manager.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
//�@�C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "keyinput.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "sound.h"

//-----------------------------------------------------------------------------
//�ÓI�����o�ϐ��錾
//-----------------------------------------------------------------------------
CRenderer *CManager::m_pRenderer = NULL;

CKey *CManager::m_pKey = NULL;

CManager::MODE CManager::m_mode = CManager::MODE_TITLE;

CFade *CManager::m_pFade = NULL;

CTitle *CManager::m_pTitle = NULL;

CTutorial *CManager::m_pTutorial = NULL;

CGame *CManager::m_pGame = NULL;

CResult *CManager::m_pResult = NULL;

CSound *CManager::m_pSound = NULL;

CPause *CManager::m_pPause = NULL;
bool CManager::m_bPause = false;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// ������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	srand((unsigned)time(NULL));	// ���ݎ����̏��ŏ�����

	m_pRenderer = new CRenderer;

	// �����_��������
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	m_pKey = new CKey;

	// �L�[�{�[�h������
	if (FAILED(m_pKey->Init(hInstance, hWnd)))
	{
		return -1;
	}
	m_pSound = new CSound;

	// �T�E���h�̏�����
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return -1;
	}

	m_pFade = new CFade;

	//�t�F�[�h�̏�����
	m_pFade->Init();

	// �t�F�[�h���Ă���^�C�g����
	m_pFade->SetFade(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	//���݃��[�h�̏I��
	UninitMode(m_mode);

	// �t�F�[�h�̊J��
	if (m_pFade != NULL)		
	{// NULL�`�F�b�N
		m_pFade->Uninit();		
		delete m_pFade;			
		m_pFade = NULL;			
	}

	// �����_���[�̊J��
	if (m_pRenderer != NULL)	
	{// NULL�`�F�b�N
		m_pRenderer->Uninit();	
		delete m_pRenderer;		
		m_pRenderer = NULL;		
	}

	// �L�[�̊J��
	if (m_pKey != NULL)			
	{// NULL�`�F�b�N
		m_pKey->Uninit();		
		delete m_pKey;			
		m_pKey = NULL;			
	}

	// �T�E���h�̊J��
	if (m_pSound != NULL)
	{// NULL�`�F�b�N
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// �V�[���̑S�폜
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �����_���[�̍X�V
	if (m_pRenderer != NULL)
	{// NULL�`�F�b�N
		m_pRenderer->Update();	
	}

	// �L�[�̍X�V
	if (m_pKey != NULL)
	{// NULL�`�F�b�N
		m_pKey->Update();		
	}

	// �t�F�[�h�̍X�V
	if (m_pFade != NULL)
	{// NULL�`�F�b�N
		m_pFade->Update();		
	}

	// �|�[�Y�̍X�V
	PauseUpdate();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	// �`�揈��
	if (m_pRenderer != NULL)
	{// NULL�`�F�b�N
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�̏I������
//=============================================================================
void CManager::UninitMode(MODE mode)
{
	// �e���[�h�̊J��
	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// NULL�`�F�b�N
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// NULL�`�F�b�N
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// NULL�`�F�b�N
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// NULL�`�F�b�N
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// ���[�h�̐���
//=============================================================================
void CManager::CreateMode(MODE mode)
{
	// �e���[�h�̐���
	switch (mode)
	{
	case MODE_TITLE:	
		if (m_pTitle == NULL)
		{// NULL�`�F�b�N
			m_pTitle = CTitle::Create();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// NULL�`�F�b�N
			m_pTutorial = CTutorial::Create();
		}
		break;

	case MODE_GAME:		
		if (m_pGame == NULL)
		{// NULL�`�F�b�N
			m_pGame = CGame::Create();
		}
		break;

	case MODE_RESULT:	
		if (m_pGame == NULL)
		{// NULL�`�F�b�N
			m_pResult = CResult::Create();
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//���݃��[�h�̏I��
	UninitMode(m_mode);

	// �V�[�����̃����[�X
	CScene::ReleaseAll();

	//���[�h��ݒ�
	m_mode = mode;
	m_bPause = false;
	m_pPause = NULL;

	// ���[�h�̐���
	CreateMode(m_mode);
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// �����_���[�N���X�̃Q�b�g�֐�
//=============================================================================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// �L�[�{�[�h���͎擾
//=============================================================================
CKey* CManager::GetKey(void)
{
	return m_pKey;
}

//=============================================================================
// �|�[�Y�̍X�V
//=============================================================================
void CManager::PauseUpdate(void)
{
	CKey *pKey = CManager::GetKey();	   // �L�[���͏��
	CFade::FADE Fade = CFade::GetFade();   // �t�F�[�h���

	if (Fade == CFade::FADE_NONE && m_mode == MODE_GAME)
	{// �t�F�[�h���������Ă��Ȃ���
		if (pKey->GetState(CKey::STATE_TRIGGER, DIK_P) == true)
		{// P�������ꂽ�Ƃ�
			m_bPause = m_bPause ? false : true;	// false��true�ɐ؂�ւ���
		}
		if (m_bPause == true && m_pPause == NULL)
		{// �|�[�Y���J�n���ꂽ�Ƃ�
			m_pPause = CPause::Create();	// �|�[�Y�𐶐�
		}
		else if (m_bPause == false && m_pPause != NULL)
		{
			m_pPause->Uninit();
			m_pPause = NULL;
		}
	}
}


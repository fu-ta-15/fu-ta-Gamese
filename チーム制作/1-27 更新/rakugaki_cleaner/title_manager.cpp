//=============================================================================
//
// �^�C�g����ʊǗ����� [title_manager.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title_manager.h"
#include "manager.h"
#include "base.h"
#include "title.h"
#include "press_logo.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "debugproc.h"

//=============================================================================
// CTitleManager�̃R���X�g���N�^
//=============================================================================
CTitleManager::CTitleManager()
{
	// �����o�ϐ��̏�����
	m_pPressLogo = NULL;
	m_bPress = false;
}

//=============================================================================
// CTitleManager�̃f�X�g���N�^
//=============================================================================
CTitleManager::~CTitleManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitleManager::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_pPressLogo = NULL;
	m_bPress = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitleManager::Uninit(void)
{
	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitleManager::Update(void)
{
	// �L�[���ƃp�b�h�����擾
	CInputKeyboard *pKey = GET_KEYBOARD;
	CInputJoypad *pJoypad = GET_JOYPAD;

	// �{�^������������true�ɂ���
	if (m_bPress == false)
	{
		if (pKey->GetTrigger(KEY_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_DECISION))
		{
			m_pPressLogo->PressButton();

			// true�ɂ���
			m_bPress = true;
		}
	}
	else
	{
		// �J�ڂł���Ƃ�����
		if (m_pPressLogo->GetTransition())
		{
			// �J�ڐ���w��
			CTitle *pTitle = (CTitle*)(CManager::GetBase());

			pTitle->SetTitleMode(CTitle::TITLEMODE_TRANSITION);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitleManager::Draw(void)
{

}
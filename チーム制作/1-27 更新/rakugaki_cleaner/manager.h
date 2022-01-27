//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : 
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

#include <stdlib.h>
#include <time.h>

// �}�N����`
#define GET_KEYBOARD		(CManager::GetInputKeyboard())		// �L�[�{�[�h���̎擾
#define GET_JOYPAD			(CManager::GetInputJoypad())		// �L�[�{�[�h���̎擾
#define GET_MOUSE			(CManager::GetInputMouse())		// �L�[�{�[�h���̎擾

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;			// �����_���N���X
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CInputJoypad;			// �W���C�p�b�h�N���X
class CInputMouse;			// �}�E�X�N���X
class CSound;				// �T�E���h�N���X
class CFade;				// �t�F�[�h�N���X
class CCamera;				// �J�����N���X
class CLight;				// ���C�g�N���X
class CTexture;				// �e�N�X�`���N���X
class CBase;				// �x�[�X�N���X
class CKeepObject;			// �f�[�^�ۑ��p�N���X

//-----------------------------------------------------------------------------
// �}�l�[�W���[�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CManager
{
public:
	// ���[�h�̎��
	typedef enum
	{
		MODE_TITLE = 0,		// �^�C�g��
		MODE_MODESELECT,	// ���[�h�I��
		MODE_PREPARATION,	// ����
		MODE_TUTORIAL,		// �`���[�g���A��
		MODE_GAME,			// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_RANKING,		// �����L���O
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetMode(CBase *pBase);
	static MODE GetMode(void) { return m_mode; }
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CFade *GetFade(void) { return m_pFade; }
	static CSound *GetSound(void) { return m_pSound; }
	static CLight *GetLight(void) { return m_pLight; }
	static CTexture *GetTexture(void) { return m_pTexture; }
	static CBase *GetBase(void) { return m_pBase; }
	static CKeepObject *GetKeepObject(void) { return m_pKeepObject; }

	static bool GetPause(void) { return m_bPause; }
	static void SetPause(bool bPause) { m_bPause = bPause; }

private:
	static CRenderer *m_pRenderer;				// �����_�����̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h���̃|�C���^
	static CInputJoypad *m_pInputJoypad;		// �W���C�p�b�h���̃|�C���^
	static CInputMouse *m_pInputMouse;			// �}�E�X���̃|�C���^
	static CSound *m_pSound;					// �T�E���h���̃|�C���^
	static CBase *m_pBase;						// �x�[�X�V�[�����̃|�C���^
	static MODE m_mode;							// ���[�h���
	static CFade *m_pFade;						// �t�F�[�h���̃|�C���^
	static bool m_bPause;						// �|�[�Y���邩�ǂ���
	static CCamera *m_pCamera;					// �J�������̃|�C���^
	static CLight *m_pLight;					// ���C�g���̃|�C���^
	static CTexture *m_pTexture;				// �e�N�X�`�����̃|�C���^
	static CKeepObject *m_pKeepObject;			// �f�[�^�ۑ��p���̃|�C���^

	CBase *SetMode(void);
};
#endif
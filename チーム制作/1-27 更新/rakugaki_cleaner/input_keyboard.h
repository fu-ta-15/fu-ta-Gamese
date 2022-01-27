//=============================================================================
//
// �L�[�{�[�h���� [input_keyboard.h]
// Author : 
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DATA							(256)										// �ő�f�[�^��
#define KEY_LookAtPoint_LEFT				(CInputKeyboard::KEYINFO_LookAtPoint_LEFT)	// �����_����]
#define KEY_LookAtPoint_RIGHT				(CInputKeyboard::KEYINFO_LookAtPoint_RIGHT)	// �����_�E��]
#define KEY_Point_LEFT						(CInputKeyboard::KEYINFO_Point_LEFT)		// ���_����]
#define KEY_Point_RIGHT						(CInputKeyboard::KEYINFO_Point_RIGHT)		// ���_�E��]
#define KEY_MOVE_UP							(CInputKeyboard::KEYINFO_UP)				// ��ړ�
#define KEY_MOVE_DOWN						(CInputKeyboard::KEYINFO_DOWN)				// ���ړ�
#define KEY_MOVE_RIGHT						(CInputKeyboard::KEYINFO_RIGHT)				// �E�ړ�
#define KEY_MOVE_LEFT						(CInputKeyboard::KEYINFO_LEFT)				// ���ړ�
#define KEY_MOVE_LookAtPoint_UP				(CInputKeyboard::KEYINFO_LookAtPoint_UP)	// �����_��ړ�
#define KEY_MOVE_LookAtPoint_DOWN			(CInputKeyboard::KEYINFO_LookAtPoint_DOWN)	// �����_���ړ�
#define KEY_MOVE_Point_UP					(CInputKeyboard::KEYINFO_Point_UP)			// ���_��ړ�
#define KEY_MOVE_Point_DOWN					(CInputKeyboard::KEYINFO_Point_DOWN)		// ���_���ړ�
#define KEY_FLAME							(CInputKeyboard::KEYINFO_FLAME)				// ���_���ړ�
#define KEY_MOVE_Up							(CInputKeyboard::KEYINFO_Up)				// ��ړ�
#define KEY_MOVE_Down						(CInputKeyboard::KEYINFO_Down)				// ���ړ�
#define KEY_MOVE_Right						(CInputKeyboard::KEYINFO_Right)				// �E�ړ�
#define KEY_MOVE_Left						(CInputKeyboard::KEYINFO_Left)				// ���ړ�

#define KEY_RESET							(CInputKeyboard::KEYINFO_RESET)				// ���ړ�

#define KEY_PAUSE							(CInputKeyboard::KEYINFO_PAUSE)				// ���ړ�
#define KEY_OK								(CInputKeyboard::KEYINFO_OK)				// ���ړ�

#define KEY_PLAYEROP_UP						(CInputKeyboard::KEYINFO_PLAYEROP_UP)		// ���ړ�
#define KEY_PLAYEROP_DOWN					(CInputKeyboard::KEYINFO_PLAYEROP_DOWN)		// ���ړ�

#define KEY_DELETE							(CInputKeyboard::KEYINFO_DELETE)		// ���ړ�
#define KEY_SWITCH_ERASER					(CInputKeyboard::KEYINFO_SWITCH_ERASER)		// ���ړ�
#define KEY_SKILL							(CInputKeyboard::KEYINFO_SKILL)		// ���ړ�

//-----------------------------------------------------------------------------
// �L�[�{�[�h�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	// �L�[���͏��
	typedef enum
	{
		KEYINFO_OK = 0,				// ����
		KEYINFO_UP,					// ��I��
		KEYINFO_DOWN,				// ���I��
		KEYINFO_RIGHT,				// �E�ړ�
		KEYINFO_LEFT,				// ���ړ�
		KEYINFO_PAUSE,				// �|�[�Y
		KEYINFO_LookAtPoint_LEFT,	// �����_����]
		KEYINFO_LookAtPoint_RIGHT,	// �����_�E��]
		KEYINFO_Point_LEFT,			// ���_����]
		KEYINFO_Point_RIGHT,		// ���_�E��]
		KEYINFO_LookAtPoint_UP,		// �����_��ړ�
		KEYINFO_LookAtPoint_DOWN,	// �����_���ړ�
		KEYINFO_Point_UP,			// ���_��ړ�
		KEYINFO_Point_DOWN,			// ���_���ړ�
		KEYINFO_FLAME,				// ���C���[�t���[���̕\��
		KEYINFO_Up,
		KEYINFO_Down,
		KEYINFO_Right,
		KEYINFO_Left,
		KEYINFO_BuffReset,
		KEYINFO_RESET,
		KEYINFO_PLAYEROP_DOWN,
		KEYINFO_PLAYEROP_UP,
		KEYINFO_DELETE,
		KEYINFO_SWITCH_ERASER,
		KEYINFO_SKILL,
		KEYINFO_MAX,
	}KEYINFO;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetReleace(int nKey);

private:
	BYTE m_aKeyState[MAX_DATA];
	BYTE m_aKeyStateTrigger[MAX_DATA];
	BYTE m_aKeyStateReleace[MAX_DATA];
	BYTE m_aKeyStateRepeat[MAX_DATA];

	int m_aKeyInfo[KEYINFO_MAX] =
	{
		DIK_RETURN,		// ����
		DIK_W,			// ��I��
		DIK_S,			// ���I�� 
		DIK_D,			// �E�ړ�
		DIK_A,			// ���ړ�
		DIK_P,			// �|�[�Y
		DIK_Q,			// �����_����]
		DIK_E,			// �����_�E��]
		DIK_Z,			// ���_����]
		DIK_C,			// ���_�E��]
		DIK_T,			// �����_��ړ�
		DIK_B,			// �����_���ړ�
		DIK_R,			// ���_��ړ�
		DIK_V,			// ���_���ړ�
		DIK_M,			// ���C���[�t���[���̕\��
		DIK_UP,
		DIK_DOWN,
		DIK_RIGHT,
		DIK_LEFT,
		DIK_F2,
		DIK_LCONTROL,
		DIK_9,			// ����\�v���C���[�̑I��
		DIK_0,			// ����\�v���C���[�̑I��
		DIK_SPACE,		// ��������
		DIK_K,			// �؂�ւ�
		DIK_J,			// �X�L��
	};
};
#endif
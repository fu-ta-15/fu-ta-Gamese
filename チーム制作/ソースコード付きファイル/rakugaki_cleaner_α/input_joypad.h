//=============================================================================
//
// �W���C�p�b�h���� [input_joypad.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DATA		(256)													// �ő�f�[�^��
#define MAX_CONTROLLER	(4)														// �ڑ��\�ȃR���g���[���[�̍ő吔

#define JOYPAD_DECISION				(CInputJoypad::JPINFO_A)					// ����{�^��
#define JOYPAD_CANCEL				(CInputJoypad::JPINFO_B)					// ����{�^��

#define JOYPAD_JUMP					(CInputJoypad::JPINFO_A)					// �W�����v�{�^��
#define JOYPAD_POINT_ROT_RIGHT		(CInputJoypad::JPINFO_RB)					// �W�����v�{�^��
#define JOYPAD_POINT_ROT_LEFT		(CInputJoypad::JPINFO_LB)					// �W�����v�{�^��

#define JOYPAD_GRAFFITI_DELETE		(CInputJoypad::JPINFO_RB)					// ���N�K�L�폜
#define JOYPAD_ERASERTYPE_SWITCH	(CInputJoypad::JPINFO_X)					// �����S���؂�ւ�
#define JOYPAD_SKILL				(CInputJoypad::JPINFO_A)					// �X�L��

#define JOYPAD_UP					(CInputJoypad::STICK_AND_CROSSKEY_UP)		// ��
#define JOYPAD_DOWN					(CInputJoypad::STICK_AND_CROSSKEY_DOWN)		// ��
#define JOYPAD_RIGHT				(CInputJoypad::STICK_AND_CROSSKEY_RIGHT)	// �E
#define JOYPAD_LEFT					(CInputJoypad::STICK_AND_CROSSKEY_LEFT)		// ��

#define JOYPAD_PAUSE				(CInputJoypad::JPINFO_START)				// �|�[�Y

//*****************************************************************************
// �W���C�p�b�h�N���X(�h���N���X)
//*****************************************************************************
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		JPINFO_X = 0,	// X
		JPINFO_Y,		// Y
		JPINFO_A,		// A
		JPINFO_B,		// B
		JPINFO_LB,		// LB
		JPINFO_RB,		// RB
		JPINFO_LT,		// LT
		JPINFO_RT,		// RT
		JPINFO_LS,		// LS
		JPINFO_RS,		// RS
		JPINFO_BACK,	// BACK
		JPINFO_START,	// START
		JPINFO_GUIDE	// GUIDE
	}JOYPADINFO;

	typedef enum
	{
		STICK_AND_CROSSKEY_UP = 0,	// ��
		STICK_AND_CROSSKEY_DOWN,	// ��
		STICK_AND_CROSSKEY_RIGHT,	// �E
		STICK_AND_CROSSKEY_LEFT,	// ��
		STICK_AND_CROSSKEY_MAX,
	}StickAndCrossKey;

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	void LStickPressInfo(int nCnt, DIJOYSTATE2 State);											// L�X�e�B�b�N�̃v���X����
	void RStickPressInfo(int nCnt, DIJOYSTATE2 State);											// R�X�e�B�b�N�̃v���X����
	void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// �X�e�B�b�N�̃g���K�[����

	void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// �{�^���̃v���X����
	void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// �{�^���̃g���K�[����

	bool GetButtonPress(int nCnt, int nButton);													// �{�^���̃v���X���̎擾
	bool GetButtonTrigger(int nCnt, int nButton);												// �{�^���̃g���K�[���̎擾
	bool GetButtonReleace(int nCnt, int nButton);												// �{�^���̃����[�X���̎擾

	DIJOYSTATE2 GetControllerPless(int nCnt);													// �R���g���[���[�̃v���X���̎擾
	bool *GetLStickPress(int nCnt);																// L�X�e�B�b�N�̃v���X���̎擾
	bool *GetRStickPress(int nCnt);																// R�X�e�B�b�N�̃v���X���̎擾
	bool *GetStickTrigger(int nCnt);															// �X�e�B�b�N�̃g���K�[���̎擾

	bool *GetCrossKeyPress(int nCnt);															// �{�^���̃v���X���̎擾
	bool *GetCrossKeyTrigger(int nCnt);															// �{�^���̃g���K�[���̎擾

	int GetnCntController(void);

private:
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// �R�[���o�b�N�֐�

	typedef struct 
	{
		LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// �W���C�p�b�h�̃f�o�C�X���
		DIJOYSTATE2 State;						// �v���X
		DIJOYSTATE2 Trigger;					// �g���K�[
		DIJOYSTATE2 Release;					// �����[�X
	}CONTROLLER;								// �R���g���[���[�̏��							

	typedef struct 
	{
		bool bLStickPress[STICK_AND_CROSSKEY_MAX];			// L�X�e�B�b�N�̃v���X���
		bool bRStickPress[STICK_AND_CROSSKEY_MAX];			// R�X�e�B�b�N�̃v���X���
		bool bStickTrigger[STICK_AND_CROSSKEY_MAX];			// �X�e�B�b�N�̃g���K�[���
		bool bStickOldTrigger[STICK_AND_CROSSKEY_MAX];		// �X�e�B�b�N�̃g���K�[����ۑ�
		bool bCrossKeyPress[STICK_AND_CROSSKEY_MAX];		// �\���L�[�̃v���X���
		bool bCrossKeyTrigger[STICK_AND_CROSSKEY_MAX];		// �\���L�[�̃g���K�[���
		bool bCrossKeyOldTrigger[STICK_AND_CROSSKEY_MAX];	// �\���L�[�̃g���K�[����ۑ�
	}JPINFO;												// �R���g���[���[�̑�����										

	static CONTROLLER m_Controller[MAX_CONTROLLER];			// �R���g���[���[���̃|�C���^
	static JPINFO m_Stick[MAX_CONTROLLER];					// �X�e�B�b�N���
	static int m_nNumController;							// �g�p����R���g���[���[�̃J�E���g
};
#endif
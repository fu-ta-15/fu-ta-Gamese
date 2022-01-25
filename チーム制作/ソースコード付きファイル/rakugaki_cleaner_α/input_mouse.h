//=============================================================================
//
// �W���C�p�b�h���� [input_joypad.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

#include "input.h"

//*****************************************************************************
// �񋓌^
//*****************************************************************************
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_CENTER,
	MOUSE_MAX,
}MOUSE;

//*****************************************************************************
// �W���C�p�b�h�N���X(�h���N���X)
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelese(int nButton);

	D3DXVECTOR2 GetMouseMove(void);
	float GetMouseWheel(void);

private:
	DIMOUSESTATE m_CurrentMouseState;
	DIMOUSESTATE m_PrevMouseState;
	POINT m_pos;
	POINT m_posOld;
	bool m_bLeftClick;
};
#endif
//=============================================================================
//
// ���[�h�I����ʏ��� [mode_select.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

#include "base.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUIBg;		// UI�w�i�N���X
class CModel;		// ���f���N���X

//*****************************************************************************
// ���[�h�I����ʃN���X(�h���N���X)
//*****************************************************************************
class CModeSelect : public CBase
{
public:
	// �^�C�g���̃��[�h
	typedef enum
	{
		MODESELECT_NORMAL = 0,					// �ʏ�
		MODESELECT_TRANSITION_GAME_BATTLE,		// �V���O�����[�h�J��
		MODESELECT_TRANSITION_GAME_COOPERATION,		// �}���`���[�h�J��
		MODESELECT_TRANSITION_GAME_TUTORIAL,	// �`���[�g���A�����[�h�J��
		MODESELECT_MAX,
	}MODESELECT;

	CModeSelect();
	~CModeSelect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	MODESELECT GetModeSelect(void) { return m_modeselect; }
	void SetMode(MODESELECT modeselect) { m_modeselect = modeselect; }

private:
	MODESELECT m_modeselect;		// ���[�h�I�����[�h
};
#endif
//=============================================================================
//
// �X�e�[�W�I����ʏ��� [stage_select.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _STAGESELECT_H_
#define _STAGESELECT_H_

#include "base.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// ���[�h�I����ʃN���X(�h���N���X)
//*****************************************************************************
class CStageSelect : public CBase
{
public:
	// �^�C�g���̃��[�h
	typedef enum
	{
		MODESELECT_NORMAL = 0,					// �ʏ�
		MODESELECT_TRANSITION_GAME_SINGLE,		// �V���O�����[�h�J��
		MODESELECT_TRANSITION_GAME_MULTI,		// �}���`���[�h�J��
		MODESELECT_MAX,
	}STAGESELECT;

	CStageSelect();
	~CStageSelect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	STAGESELECT GetStageSelect(void) { return m_stageselect; }
	void SetMode(STAGESELECT stageselect) { m_stageselect = stageselect; }

private:
	STAGESELECT m_stageselect;		// ���[�h�I�����[�h
};
#endif
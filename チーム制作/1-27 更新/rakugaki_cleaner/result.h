//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "base.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer_Result;			// �X�R�A�N���X
class CCamera;			// �X�R�A�N���X
class CResultManager;
class CScore;			// �X�R�A�N���X
class CNumber;			// �i���o�[�N���X
class CScene2D;			// Cscene2D�N���X
class CResult_UI;		// ���U���gUI�N���X

//*****************************************************************************
// ���U���g��ʃN���X(�h���N���X)
//*****************************************************************************
class CResult : public CBase
{
public:
	// ���[�h
	typedef enum
	{
		RESULTMODE_NORMAL = 0,
		RESULTMODE_RESTART,
		RESULTMODE_TRANSITION_MODESELECT,
		RESULTMODE_TRANSITION_TITLE,
		RESULTMODE_MAX,
	}ResultMode;

	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer_Result **GetPlayer(void) { return m_pPlayer; }
	static CCamera *GetCamera(void) { return m_pCamera; }

	int GetNumPlayer(void) { return m_nNumPlayer; }
	static void SetResultMode(ResultMode resultMode) { m_resultMode = resultMode; }

private:
	static CPlayer_Result **m_pPlayer;						// �v���C���[���̃|�C���^
	static CCamera *m_pCamera;						// �J���������i�[����擪�A�h���X�̃|�C���^
	static ResultMode m_resultMode;
	CResultManager *m_pResultManager;
	int m_nNumPlayer;

	CResult_UI *m_pResult_UI;
};
#endif
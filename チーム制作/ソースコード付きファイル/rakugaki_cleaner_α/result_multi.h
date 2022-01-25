//=============================================================================
//
// �}���`���[�h�̃��U���g��ʏ��� [result_multi.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTMULTI_H_
#define _RESULTMULTI_H_

#include "result.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;			// �X�R�A�N���X
class CCamera;			// �X�R�A�N���X

//*****************************************************************************
// ���U���g��ʃN���X(�h���N���X)
//*****************************************************************************
class CResult : public CBase
{
public:
	CResult();
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer **GetPlayer(void) { return m_pPlayer; }
	static CCamera **GetCamera(void) { return &m_pCamera[0]; }

	int GetNumPlayer(void) { return m_nNumPlayer; }

private:
	static CPlayer **m_pPlayer;						// �v���C���[���̃|�C���^
	static CCamera **m_pCamera;						// �J���������i�[����擪�A�h���X�̃|�C���^

	int m_nNumPlayer;
};
#endif
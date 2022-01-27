//=============================================================================
//
// ���U���g��ʂ�UI���� [result_ui.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTUI_H_
#define _RESULTUI_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScore;			// �X�R�A�N���X
class CNumber;			// �i���o�[�N���X
class CScene2D;			// Cscene2D�N���X

//*****************************************************************************
// ���U���g��ʃN���X(�h���N���X)
//*****************************************************************************
class CResult_UI : public CScene
{
public:
	CResult_UI(int nPriority = PRIORITY_UI);
	~CResult_UI();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetAchivement(void) { return m_bAchivement; }

private:
	bool InitDisplayAchivement(void);
	void InitDisplayAchivementPlayer(D3DXVECTOR3 pos, int nCnt);

	CScore *m_pScoreInteger;	// �X�R�A�N���X���̃|�C���^(��������)
	CNumber *m_pNumberDecimal;	// �i���o�[�N���X���̃|�C���^(��������)
	CScene2D *m_pLogoComma;		// �R���}���S���̃|�C���^
	CScene2D *m_pLogoPercent;	// �p�[�Z���g���S���̃|�C���^

	CScore *m_pScoreIntegerPlayer[4];	// �X�R�A�N���X���̃|�C���^(��������)
	CNumber *m_pNumberDecimalPlayer[4];	// �i���o�[�N���X���̃|�C���^(��������)
	CScene2D *m_pLogoCommaPlayer[4];		// �R���}���S���̃|�C���^
	CScene2D *m_pLogoPercentPlayer[4];	// �p�[�Z���g���S���̃|�C���^

	bool m_bAchivement;
};
#endif
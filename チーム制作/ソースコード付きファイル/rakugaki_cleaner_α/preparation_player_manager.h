//=============================================================================
//
// �v���C���[������ʊǗ����� [preparation_player_manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PREPARATIONPLAYERMANAGER_H_
#define _PREPARATIONPLAYERMANAGER_H_

#include "scene.h"

//*****************************************************************************
// �v���C���[�ҋ@��ʃN���X(�h���N���X)
//*****************************************************************************
class CPressLogo;			// �v���X���S�N���X
class CScene2D;				// Scene2D�N���X

//*****************************************************************************
// �v���C���[�ҋ@��ʊǗ��N���X(�h���N���X)
//*****************************************************************************
class CPreparationPlayerManager : public CScene
{
public:
	CPreparationPlayerManager();
	~CPreparationPlayerManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPressLogo *GetPressLogo(void) { return m_pPressLogo; }
	void SetPressLogo(CPressLogo *pPressLogo) { m_pPressLogo = pPressLogo; }
	CScene2D *GetFrame(int nCnt) { return m_pFrame[nCnt]; }
	void SetFrame(CScene2D *pFrame,int nCnt) { m_pFrame[nCnt] = pFrame; }
	bool GetTransition(void) { return m_bTransition; }

private:
	typedef struct
	{
		bool bJoin;		// �Q�����Ă��邩�ǂ���
		bool bReady;	// �����ł������ǂ���
	}LogoInfo;

	void Transition(void);				// �J��
	void InitCharaSelect(void);			// �L�����I��������
	void CharaSelect(void);				// �L�����I��

	LogoInfo m_logoInfo[4];				// ���S�̕\�����
	CPressLogo *m_pPressLogo;			// �v���X���S���̃|�C���^
	bool m_bPlayerReadyOK;				// �����ł��Ă��邩�ǂ���
	bool m_bOK;
	bool m_bTransition;					// �J�ڂ��邩�ǂ���

	CScene2D *m_pFrame[4];				// �t���[��
	CScene2D *m_pJoinLogo[4];			// �Q���{�^�����S
	CScene2D *m_pCompLogo[4];			// �����������S
	CScene2D *m_pOkLogo[4];				// OK���S
};
#endif
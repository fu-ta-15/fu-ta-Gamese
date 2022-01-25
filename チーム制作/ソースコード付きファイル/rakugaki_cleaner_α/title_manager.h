//=============================================================================
//
// �^�C�g����ʊǗ����� [title_manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPressLogo;		// �v���X���S�N���X

//*****************************************************************************
// �^�C�g����ʊǗ��N���X(�h���N���X)
//*****************************************************************************
class CTitleManager : public CScene
{
public:
	CTitleManager();
	~CTitleManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPressLogo(CPressLogo *pPressLogo) { m_pPressLogo = pPressLogo; }

private:
	CPressLogo *m_pPressLogo;		// �v���X���S���̃|�C���^
	bool m_bPress;					// ���������ǂ���
};
#endif
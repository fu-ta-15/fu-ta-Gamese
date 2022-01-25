//=============================================================================
//
// �������� [crown.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _CROWN_H_
#define _CROWN_H_

#include "scene2D.h"

//*****************************************************************************
// �����N���X(�h���N���X)
//*****************************************************************************
class CCrown : public CScene2D
{
public:
	CCrown(int nPriority);
	~CCrown();

	static CCrown *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int Tex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int Tex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDisplay(bool bDis) { m_bDisplay = bDis; }

private:
	bool m_bDisplay;		// �`�悷�邩�ǂ���
};
#endif
//=============================================================================
//
// �ۉe���� [shadow.h]
// Author : 
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//-----------------------------------------------------------------------------
// �X�L���N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CShadow : public CScene3D
{
public:
	CShadow(int nPriority);
	~CShadow();

	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif
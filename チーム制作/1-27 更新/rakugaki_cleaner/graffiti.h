//=============================================================================
//
// ���N�K�L���� [graffiti.h]
// Author : 
//
//=============================================================================
#ifndef _GRAFFITI_H_
#define _GRAFFITI_H_

#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//-----------------------------------------------------------------------------
// ���N�K�L�N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CGraffiti : public CScene3D
{
public:
	// ���N�K�L�̐F
	typedef enum
	{
		GRAFFITICOLOR_BLACK = 0,		// ��
		GRAFFITICOLOR_COLORFUL,			// �J���t��(���ȊO)
	}GraffitiColor;

	CGraffiti(int nPriority);
	~CGraffiti();

	static CGraffiti *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	GraffitiColor GetGraffitiCol(void) { return m_graffitiCol; }

	bool Collision(D3DXVECTOR3 pos,D3DXVECTOR3 size,int nType);			// ���N�K�L�̓����蔻��

private:
	GraffitiColor m_graffitiCol;			// ���N�K�L�̐F
};
#endif

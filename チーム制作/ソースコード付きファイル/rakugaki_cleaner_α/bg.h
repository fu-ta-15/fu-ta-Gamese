//=============================================================================
//
// �w�i���� [bg.h]
// Author : 
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// �w�i�N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CBg : public CScene2D
{
public:
	CBg(int nPriority);
	~CBg();

	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXCOLOR col, D3DXVECTOR2 moveTex,int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, D3DXVECTOR2 moveTex, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void MoveTex(void);		// �e�N�X�`�����W�ړ�

	D3DXVECTOR2 m_tex;		// �e�N�X�`�����W
	D3DXVECTOR2 m_moveTex;	// �e�N�X�`�����W�����p
	int m_nTex;				// �e�N�X�`���ԍ�
};
#endif
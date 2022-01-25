//=============================================================================
//
// ナンバー処理 [number.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "Scene2D.h"

//*****************************************************************************
// ナンバークラス(基本クラス)
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return m_size; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetCol(D3DXCOLOR col);
	void SetTexNum(int nTex) { m_nTex = nTex; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR2 m_size;						// 大きさ		

	int m_nTex;								// テクスチャ番号
};
#endif
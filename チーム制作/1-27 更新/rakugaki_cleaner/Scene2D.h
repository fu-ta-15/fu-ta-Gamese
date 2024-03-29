//=============================================================================
//
// シーン2D処理 [scene2D.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) // 頂点フォーマット
#define MAX_VERTEX (4)												// 頂点の数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos; // 頂点座標
	float rhw;	     // 1.0で固定
	D3DCOLOR col;    // 頂点カラー
	D3DXVECTOR2 tex;
} VERTEX_2D;

//-----------------------------------------------------------------------------
// オブジェクトクラス(基本クラス)
//-----------------------------------------------------------------------------
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_fWidth,m_fHeight); }
	D3DXCOLOR GetCol(void) { return m_col; }
	D3DXVECTOR2 GetTexSheet(void) { return m_texSheets; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetRotate(float fAngle);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV);
	void SetCol(D3DXCOLOR col);
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }
	void SetTexSheet(D3DXVECTOR2 texSheets) { m_texSheets = texSheets; }

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }

	/* 追加 by Suzuki_Futa */

	void AddPosTex(const D3DXVECTOR3& pos, const D3DXVECTOR2& tex, int nNo1, int nNo2);
	void TexControl(void);
	static CScene2D *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size,int nPriority, int nTex);
	HRESULT CreateTexture(const LPCSTR pSrcFile);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャ情報のポインタ
	D3DXVECTOR3	m_pos;							// 座標
	D3DXCOLOR m_col;							// 頂点カラー

	D3DXVECTOR3	m_rot;							// ポリゴンの回転角
	float m_fAngle;								// ポリゴンの対角線の角度
	float m_fLength;							// ポリゴンの対角線の長さ
	float m_fWidth;								// ポリゴンの幅
	float m_fHeight;							// ポリゴンの高さ

	D3DXVECTOR2 m_texSheets;

	bool m_bDeath;								// 死亡フラグ
};
#endif
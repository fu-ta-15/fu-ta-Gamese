//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : 
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"

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
class CLogo
{
public:
	CLogo();
	~CLogo();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	D3DXVECTOR3 GetPosition(void) { return m_CorLogoInfo.pos; }
	D3DXVECTOR2 GetSize(void) { return m_CorLogoInfo.size; }
	D3DXCOLOR GetCol(void) { return m_CorLogoInfo.col; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetRotate(float fAngle);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets);
	void SetCol(D3DXCOLOR col);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	float m_fAngle;								// ポリゴンの対角線の角度
	float m_fLength;							// ポリゴンの対角線の長さ
	float m_fWidth;								// ポリゴンの幅
	float m_fHeight;							// ポリゴンの高さ

	int m_nTex;									// テクスチャ番号

	// ロゴに必要な情報
	typedef struct
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR2 size;	// サイズ
		D3DXCOLOR col;		// 色
		D3DXVECTOR2 tex;	// テクスチャ座標
	}LogoInfo;

	LogoInfo m_CorLogoInfo;	// 現在のロゴ情報
	LogoInfo m_DefLogoInfo;	// デフォルト(初期)のロゴ情報
};
#endif
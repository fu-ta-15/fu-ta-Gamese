//*****************************************************************************
//
// シーン処理 [scene.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _POLYGON_H_
#define _POLYGON_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CPolygon
{
public:

	typedef enum POLYGON_MOVE
	{
		POLYGON_CENTER = 0,
		POLYGON_LIGHT,
		POLYGON_LEFT,
		POLYGON_MAX
	}POLYGON_MOVE;

	CPolygon();
	~CPolygon();

	static CPolygon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT CreateTexture(const LPCSTR pSrcFile);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// SET関数
	void SetPos(const D3DXVECTOR3 pos);
	void SetSize(const D3DXVECTOR3 size);
	void SetCol(const D3DXCOLOR col);
	void SetTex(const D3DXVECTOR2 tex, const D3DXVECTOR2 fnumber);
	void SizeChangeX(float AddSize, POLYGON_MOVE type);

	// GET関数
	D3DXVECTOR3 GetPos(void);
	D3DCOLOR GetCol(void);

private:
	// ポリゴンに使用するメンバ変数
	LPDIRECT3DTEXTURE9			m_pTex = NULL;			// テクスチャへのポイント
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// 頂点バッファへのポインタ

	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_rot;					// 角度
	D3DXVECTOR3					m_size;					// サイズ
	D3DXVECTOR3					m_move;					// 移動量
	D3DXVECTOR2					m_tex;					// テクスチャ座標
	D3DXCOLOR					m_col;					// カラー
	float						m_fAngle;				// 対角線の角度
	float						m_fAnimeY;				// アニメーション座標Y
	float						m_fAnimeX;				// アニメーション座標X

};


#endif
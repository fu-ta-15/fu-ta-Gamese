//*****************************************************************************
//
// シーン処理 [scene.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CScene2D : public CScene
{
public:
	CScene2D(Priority type);
	CScene2D(PauseType type);
	~CScene2D();

	static CScene2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT CreateTexture(const LPCSTR pSrcFile);

	HRESULT Init(void);
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	HRESULT AnimationInit(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR2 tex);

	void Uninit(void);
	void Uninit(bool bPause);
	void Update(void);
	void Draw(void);

	// SET関数
	void SetPos(const D3DXVECTOR3 pos);
	void SetSize(const D3DXVECTOR3 size);
	void SetCol(const D3DXCOLOR col);
	void SetUse(const bool bUse);
	void SetTexture(const LPDIRECT3DTEXTURE9 pTex);
	void SetTex(const D3DXVECTOR2 tex, const D3DXVECTOR2 fnumber);

	// GET関数
	D3DXVECTOR3 GetPos(void);
	D3DCOLOR GetCol(void);
	bool GetUse(void) { return m_bUse; }

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
	PauseType					m_PauseType;			// ポーズの種類
	bool						m_bUse;					// 使用の有無
	float						m_fAngle;				// 対角線の角度
	float						m_fAnimeY;				// アニメーション座標Y
	float						m_fAnimeX;				// アニメーション座標X
};


#endif
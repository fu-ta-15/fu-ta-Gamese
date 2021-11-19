//*****************************************************************************
//
// フェード処理 [fade.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "manager.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CFade
{
public:
	//-------------------------------------------------------------------------
	// フェイドの状態
	//-------------------------------------------------------------------------
	typedef enum
	{
		FADE_NONE = 0,	// 何もしてない
		FADE_IN,		// フェードイン
		FADE_OUT,		// フェードアウト
		FADE_MAX
	}FADE;

	CFade();
	~CFade();

	static CFade *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// SET関数
	void SetFade(CManager::MODE mode);	
	void SetCol(D3DXCOLOR col);

	// Get関数
	static FADE GetFade(void);				

private:
	// ポリゴンに使用するメンバ変数
	static FADE					m_FadeMode;				// フェードの状態
	CManager::MODE				m_modeNext;				// フェード終了後のモード設定用
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_size;					// サイズ
	D3DXCOLOR					m_col;					// カラー
	VERTEX_2D					*m_pVtx;				// 2Dポリゴンのポインタ
};


#endif
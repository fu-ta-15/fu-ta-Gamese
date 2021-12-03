//*****************************************************************************
//
// エフェクト処理 [effect.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "manager.h"
#include "scene2D.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CEffect : public CScene2D
{
public:

	CEffect();		 // コンストラクタ
	~CEffect();		 // デストラクタ

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	  // エフェクトポリゴンのクリエイト

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size	  // パーティクル用クリエイト
							,D3DXVECTOR3 move, LPDIRECT3DTEXTURE9 ptex);

	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

	// Set関数
	void SetColor(D3DXCOLOR col)	{ m_col = col; }
	void SetMove(D3DXVECTOR3 move)	{ m_move = move; }
	void SetUse(bool bUse)			{ m_bUse = bUse; }
	void SetPos(D3DXVECTOR3 pos)	{ m_pos = pos; }

	// Get関数
	D3DXCOLOR GetColor(void)		{ return m_col; }
	D3DXVECTOR3 GetPos(void)		{ return m_pos; }
	D3DXVECTOR3 GetMove(void)		{ return m_move; }
	bool GetUse(void)				{ return m_bUse; }

private:
	D3DXVECTOR3					m_pos;		   // 位置
	D3DXVECTOR3					m_size;		   // サイズ
	D3DXVECTOR3					m_move;		   // 移動量
	D3DXCOLOR					m_col;		   // 色
	bool						m_bUse;		   // 使用しているかどうか
	bool						m_bMove;	   // 固定・自由
};
#endif // !_EFFECT_H_
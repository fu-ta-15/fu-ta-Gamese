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

	typedef enum EFFECT_TYPE
	{
		TYPE_ALPHA = 0,
		TYPE_RGB,
		TYPE_MAX
	}EFFECT_TYPE;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CEffect();
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXVECTOR3 move, LPDIRECT3DTEXTURE9 ptex);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionField(void);

	// Set関数
	void SetColor(D3DXCOLOR col)		{ m_col = col; }
	void SetMove(D3DXVECTOR3 move)		{ m_move = move; }
	void SetLife(int nLife)				{ m_nLife = nLife; }
	void SetUse(bool bUse)				{ m_bUse = bUse; }
	void SetGravity(bool bGravity)		{ m_bGravity = bGravity; }
	void SetPos(D3DXVECTOR3 pos)		{ m_pos = pos; }

	// Get関数
	D3DXCOLOR GetColor(void)	{ return m_col; }
	D3DXVECTOR3 GetPos(void)	{ return m_pos; }
	D3DXVECTOR3 GetMove(void)	{ return m_move; }
	int GetLife(void)			{ return m_nLife; }
	bool GetUse(void)			{ return m_bUse; }

private:
	D3DRENDERSTATETYPE			m_State;
	DWORD						m_Value;
	EFFECT_TYPE					m_type;
	D3DXVECTOR3					m_pos;
	D3DXVECTOR3					m_size;
	D3DXVECTOR3					m_move;
	D3DXCOLOR					m_col;
	bool						m_bUse;
	bool						m_bMove;
	bool						m_bGravity;
	int							m_nLife;

};
#endif // !_EFFECT_H_
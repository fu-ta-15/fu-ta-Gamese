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

	void Unload();
	HRESULT Load(const LPCSTR pSrcFile);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Set関数
	void SetColor(D3DXCOLOR col);
	void SetMove(D3DXVECTOR3 move);
	void SetLife(int nLife);
	void SetUse(bool bUse);
	void SetPos(D3DXVECTOR3 pos);

	// Get関数
	D3DXCOLOR GetColor(void)	{ return m_col; }
	D3DXVECTOR3 GetPos(void)	{ return m_pos; }
	D3DXVECTOR3 GetMove(void)	{ return m_move; }
	int GetLife(void)			{ return m_nLife; }
	bool GetUse(void)			{ return m_bUse; }

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;
	D3DRENDERSTATETYPE			m_State;
	DWORD						m_Value;
	EFFECT_TYPE					m_type;
	D3DXVECTOR3					m_pos;
	D3DXVECTOR3					m_size;
	D3DXVECTOR3					m_move;
	D3DXCOLOR					m_col;
	bool						m_bUse;
	int							m_nLife;

};
#endif // !_EFFECT_H_
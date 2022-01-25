//*****************************************************************************
//
// レンダラー処理 [renderer.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"


class CPause;
class CFade;

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CRenderer		// レンダラークラス
{
public:
	CRenderer();
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ゲット関数
	LPDIRECT3DDEVICE9 GetDevice(void);

private:

	// FPS描画関数
	void DrawFPS(void);

	// メンバ変数
	LPDIRECT3D9					m_pD3D			= NULL;		// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9			m_pD3DDevice	= NULL;		// Deviceオブジェクト(描画に必要)
	LPD3DXFONT					m_pFont			= NULL;		// フォントへのポインタ
	D3DFILLMODE m_fillMode = D3DFILL_SOLID;				// ワイヤーフレーム	
};


#endif
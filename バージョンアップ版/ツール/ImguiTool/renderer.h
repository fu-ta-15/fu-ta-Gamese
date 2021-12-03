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

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CRenderer		
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
	D3DPRESENT_PARAMETERS		m_d3dpp;					// パラメータ保管用
	LPDIRECT3D9					m_pD3D			= NULL;		// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9			m_pD3DDevice	= NULL;		// Deviceオブジェクト(描画に必要)
	LPD3DXFONT					m_pFont			= NULL;		// フォントへのポインタ
};


#endif
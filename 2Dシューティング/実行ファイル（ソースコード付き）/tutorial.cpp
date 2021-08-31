//##################################################################################################################################################################//
//
// チュートリアル処理 [tutorial.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"
#include "sound.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// マクロ変数
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define COLOR_RED			(255)			// 赤色
#define COLOR_BULUE			(255)			// 青色
#define COLOR_GREEN			(255)			// 緑色
#define COLOR_ALPHA			(255)			// アルファ値

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// グローバル変数
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	// 頂点バッファの情報
LPDIRECT3DTEXTURE9 g_pTextureTutorial = NULL;			// テクスチャの情報
LPDIRECT3DTEXTURE9 g_pTextureTutoriallogo = NULL;			// テクスチャの情報

//=====================================================================================================================================================================//
// 背景の初期化処理
//=====================================================================================================================================================================//
HRESULT InitTutorial(MODE mode)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// 頂点ポインタ
	VERTEX_2D *pVtx;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bgtt.png", &g_pTextureTutorial);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\playerlog.png", &g_pTextureTutoriallogo);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定(X座標・Y座標・Z座標(2Dは0固定)右回りで描画）
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhwの設定		=	1.0f固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定・赤・緑・青・アルファ
	pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

	//テクスチャの頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0, 1);
	pVtx[1].tex = D3DXVECTOR2(0, 0);
	pVtx[2].tex = D3DXVECTOR2(1, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 0);

	//頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4;

	//頂点座標の設定(X座標・Y座標・Z座標(2Dは0固定)右回りで描画）
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

	//rhwの設定		=	1.0f固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定・赤・緑・青・アルファ
	pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

	//テクスチャの頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0, 1);
	pVtx[1].tex = D3DXVECTOR2(0, 0);
	pVtx[2].tex = D3DXVECTOR2(1, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 0);

	//頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	//PlaySound(SOUND_LABEL_BGM000);

	// 処理完了
	return S_OK;
}

//=====================================================================================================================================================================//
// ポリゴンの終了処理
//=====================================================================================================================================================================//
void UninitTutorial(void)
{
	//テクスチャの開放
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//頂点バッファの開放
	if (g_pTextureTutoriallogo != NULL)
	{
		g_pTextureTutoriallogo->Release();
		g_pTextureTutoriallogo = NULL;
	}
	StopSound(SOUND_LABEL_BGM000);	//BGM
}

//=====================================================================================================================================================================//
// ポリゴンの更新処理
//=====================================================================================================================================================================//
void UpdateTutorial(void)
{
	FADE pFade;
	pFade = GetFade();

	if (GetKeyboardTrigger(DIK_RETURN) == true && pFade == FADE_NONE)
	{
		SetFade(FADE_OUT, MODE_GAME);
	}
}

//=====================================================================================================================================================================//
// ポリゴンの描画処理
//=====================================================================================================================================================================//
void DrawTutorial(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutoriallogo);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

}
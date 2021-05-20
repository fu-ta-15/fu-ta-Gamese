//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//


#include "game.h"
#include "fade.h"
#include "pause.h"
#include "input.h"

#define MAX_MENU		(PAUSE_MENU_MAX)
#define COLOR_RED		(255)		//赤色
#define COLOR_BULUE		(255)		//青色
#define COLOR_GREEN		(255)		//緑色
#define COLOR_ALPHA		(255)	//アルファ値


//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		//頂点バッファの情報
LPDIRECT3DTEXTURE9 g_apTexturePause[MAX_MENU] = {};	//テクスチャの情報
PAUSE g_Pause;										//構造体の変数

//ポーズの初期化処理
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevi;
	VERTEX_2D *pVtx;

	//変数の初期化
	g_Pause.nSelectCounter = PAUSE_MENU_CONTINUE;
	g_Pause.nSelectold = PAUSE_MENU_QUIT;
	g_Pause.bPause = false;

	//デバイスの取得
	pDevi = GetDevice();

	//テクスチャの読み取り
	D3DXCreateTextureFromFile(pDevi, "data\\TEXTURE\\pbg.png", &g_apTexturePause[0]);			//テクスチャ「背景」
	D3DXCreateTextureFromFile(pDevi, "data\\TEXTURE\\continue.png", &g_apTexturePause[1]);		//テクスチャ「コンティニュー」
	D3DXCreateTextureFromFile(pDevi, "data\\TEXTURE\\retry.png", &g_apTexturePause[2]);			//テクスチャ「リトライ」
	D3DXCreateTextureFromFile(pDevi, "data\\TEXTURE\\quit.png", &g_apTexturePause[3]);			//テクスチャ「終了」

	//頂点バッファの生成
	if (FAILED(pDevi->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MENU, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCuntPause = 0; nCuntPause < MAX_MENU; nCuntPause++)
	{
		if (nCuntPause == 0)
		{
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

			//頂点カラーの設定・赤・緑・青
			pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);
			pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);
			pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);
			pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);

			//テクスチャの頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0, 1);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1, 1);
			pVtx[3].tex = D3DXVECTOR2(1, 0);

		}
		else if (nCuntPause == 1 || nCuntPause == 2 || nCuntPause == 3)
		{
			//頂点座標の設定(X座標・Y座標・Z座標(2Dは0固定)右回りで描画）
			pVtx[0].pos = D3DXVECTOR3(430.0f, 100.0f + (nCuntPause * 200), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(430.0f, 0.0f + (nCuntPause * 200), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(840.0f, 100.0f + (nCuntPause * 200), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(840.0f, 0.0f + (nCuntPause * 200), 0.0f);

			//rhwの設定		=	1.0f固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定・赤・緑・青
			pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);
			pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);
			pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);
			pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA/3);

			//テクスチャの頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0, 1);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1, 1);
			pVtx[3].tex = D3DXVECTOR2(1, 0);

		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
	return S_OK;


}

//ポーズの終了処理
void UninitPause(void)
{
	for (int nCuntPause = 0; nCuntPause < MAX_MENU; nCuntPause++)
	{

		//テクスチャの開放
		if (g_apTexturePause[nCuntPause] != NULL)
		{
			g_apTexturePause[nCuntPause]->Release();
			g_apTexturePause[nCuntPause] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

}

//ポーズの更新処理
void UpdatePause(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += ((g_Pause.nSelectCounter) * 4);

	//頂点カラーの設定・赤・緑・青
	pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);


	pVtx += ((g_Pause.nSelectold) * 4);

	//頂点カラーの設定・赤・緑・青
	pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA / 3);
	pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA / 3);
	pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA / 3);
	pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA / 3);

	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();


	if (GetKeyboardTrigger(DIK_S) == true)
	{
		g_Pause.nSelectold = g_Pause.nSelectCounter;

		g_Pause.nSelectCounter += 1;

	}
	else if (GetKeyboardTrigger(DIK_W) == true)
	{
		g_Pause.nSelectold = g_Pause.nSelectCounter;

		g_Pause.nSelectCounter -= 1;

		if (g_Pause.nSelectCounter == 0)
		{
			g_Pause.nSelectCounter = 3;
		}

	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SelectBuuton(g_Pause.nSelectCounter);
	}

	if (((g_Pause.nSelectCounter) % 4) == 0)
	{
		g_Pause.nSelectCounter = 1;
	}


}


void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCuntPause = 0; nCuntPause < MAX_MENU; nCuntPause++)
	{

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCuntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCuntPause*4, 2);		// 0//描画を開始する頂点インデックス

	}
}

void SelectBuuton(int Buuton)
{

	if (Buuton == PAUSE_MENU_CONTINUE)
	{
		SetPause(g_Pause.bPause);
	}
	else if (Buuton == PAUSE_MENU_RETRY)
	{
		SetFade(FADE_OUT, MODE_GAME);	//フェードアウト・ゲームモードに移行
		SetPause(g_Pause.bPause);


	}
	else if (Buuton == PAUSE_MENU_QUIT)
	{
		SetFade(FADE_OUT, MODE_TITLE);	//フェードアウト・ゲームモードに移行
		SetPause(g_Pause.bPause);

	}

}


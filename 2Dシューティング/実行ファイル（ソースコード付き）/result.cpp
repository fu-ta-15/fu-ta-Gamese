//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//


#include "input.h"
#include "result.h"
#include "fade.h"
#include "sound.h"


//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファの情報
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;		//テクスチャの情報


												//ポリゴンの初期化処理
HRESULT InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\result.png", &g_pTextureResult);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(225, 225, 225, 225);
	pVtx[1].col = D3DCOLOR_RGBA(225, 225, 225, 225);
	pVtx[2].col = D3DCOLOR_RGBA(225, 225, 225, 225);
	pVtx[3].col = D3DCOLOR_RGBA(225, 225, 225, 225);

	//テクスチャの頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0, 1);
	pVtx[1].tex = D3DXVECTOR2(0, 0);
	pVtx[2].tex = D3DXVECTOR2(1, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 0);




	//頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();

	//PlaySound(SOUND_LABEL_BGM003);

	return S_OK;

}

//ポリゴンの終了処理
void UninitResult(void)
{
	//テクスチャの開放
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
	StopSound(SOUND_LABEL_BGM003);

}

//ポリゴンの更新処理
void UpdateResult(void)
{
	FADE pFade;
	pFade = GetFade();

	if (GetKeyboardTrigger(DIK_RETURN) == true && pFade == FADE_NONE)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}

}

//ポリゴンの描画処理
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		// 0//描画を開始する頂点インデックス
}
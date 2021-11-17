//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//



#include "fade.h"


#define COLOR_RED		(0)		//赤色
#define COLOR_BULUE		(0)		//青色
#define COLOR_GREEN		(0)		//緑色
#define COLOR_ALPHA		(125)	//アルファ値


//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//頂点バッファの情報
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;		//テクスチャの情報
FADE g_fade;									//フェードの状態
MODE g_modeNext;								//次のモード
D3DXCOLOR g_colorFade;							//フェードの色
float g_fAlpha;									//アルファ値


HRESULT InitFade(MODE modeNext)
{

	g_fAlpha = 1.0f;

	LPDIRECT3DDEVICE9 pDevi;

	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevi = GetDevice();


	//頂点バッファの生成
	if (FAILED(pDevi->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロック・頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fAlpha);


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
	pVtx[0].col = g_colorFade; 
	pVtx[1].col = g_colorFade; 
	pVtx[2].col = g_colorFade; 
	pVtx[3].col = g_colorFade; 

	//テクスチャの頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0, 1);
	pVtx[1].tex = D3DXVECTOR2(0, 0);
	pVtx[2].tex = D3DXVECTOR2(1, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 0);


	//頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();
	return S_OK;

}

void UninitFade(void)
{
	//テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}

}

void UpdateFade(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
	
	//フェイドのカラー設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_fAlpha);

	//頂点カラーの設定・赤・緑・青
	pVtx[0].col = g_colorFade; 
	pVtx[1].col = g_colorFade; 
	pVtx[2].col = g_colorFade; 
	pVtx[3].col = g_colorFade; 

	if (g_fade == FADE_IN)
	{
		//α値を減らしていく
		g_fAlpha -= 0.02f;
		if (g_fAlpha <= 0.0f)
		{
			g_fade = FADE_NONE;		//何もしていない状態にする
		}
	}
	else if (g_fade == FADE_OUT)
	{
		//α値を増やしていく
		g_fAlpha += 0.02f;
		if (g_fAlpha >= 1.0f)
		{
			SetMode(g_modeNext);	//次のモードへ
			g_fade = FADE_IN;		//フェードインに切り替え
		}
	}
	//頂点バッファのアンロック
	g_pVtxBuffFade->Unlock();
}

void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		// 0//描画を開始する頂点インデックス
}

//フェイドとモードの切り替え
void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;			//フェード
	g_modeNext = modeNext;	//次のモード
}

//フェードの取得
FADE GetFade(void)
{
	return g_fade;
}

//##################################################################################################################################################################//
//
// スコア処理 [score.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "score.h"
#include "ranking.h"
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//マクロ変数
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define MAX_SCORE	(3)
#define SCORE_STZEX	(580)
#define SCORE_SIZEY	(630)


//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//グローバル宣言
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;;		//テクスチャへのポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
int g_nScore;

//初期化処理
HRESULT InitScore(void)
{
	srand((unsigned int)time(0));
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//変数の初期化
	g_nScore = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number002.png", &g_pTextureScore);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffScore, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴン・テクスチャの初期化設定
	for (int nCntBullet = 0; nCntBullet < MAX_SCORE; nCntBullet++)
	{

		//各頂点座標
		pVtx[0].pos = D3DXVECTOR3(SCORE_STZEX + ((float)nCntBullet * 50), SCREEN_HEIGHT-10, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCORE_STZEX + ((float)nCntBullet * 50), SCREEN_HEIGHT-110, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCORE_SIZEY + ((float)nCntBullet * 50), SCREEN_HEIGHT-10, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCORE_SIZEY + ((float)nCntBullet * 50), SCREEN_HEIGHT-110, 0.0f);

		//rhwの設定		=	1.0f固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定・赤・緑・青
		pVtx[0].col = D3DCOLOR_RGBA(125, 10, 125, 225);
		pVtx[1].col = D3DCOLOR_RGBA(125, 10, 125, 225);
		pVtx[2].col = D3DCOLOR_RGBA(125, 10, 125, 225);
		pVtx[3].col = D3DCOLOR_RGBA(125, 10, 125, 225);

		//頂点4進める
		pVtx += 4;
	}

	//頂点バッファのアンロック
	g_pVtxBuffScore->Unlock();

	AddScore(0);

	return S_OK;
}

//終了処理
void UninitScore(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	//テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}


}

//更新処理
void UpdateScore(void)
{

}

//描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (int nCntEnemy = 0; nCntEnemy < MAX_SCORE; nCntEnemy++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
	}

}

//スコアの加算
void AddScore(int nValue)
{
	int nMath = 1000;		//スコア計算
	int aNumber[MAX_SCORE];	//スコアのナンバー
	g_nScore += nValue;		//スコア
	VERTEX_2D *pVtx;		//ポインタ

	//バッファのロック
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//スコアの変更
	for (int nCunNum = 0; nCunNum < MAX_SCORE; nCunNum++)
	{
		//スコアの計算
		aNumber[nCunNum] = (g_nScore%nMath / (nMath / 10));

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2((float)aNumber[nCunNum] / 10, 1.0f);
		pVtx[1].tex = D3DXVECTOR2((float)aNumber[nCunNum] / 10, 0);
		pVtx[2].tex = D3DXVECTOR2(((float)aNumber[nCunNum] / 10) + 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(((float)aNumber[nCunNum] / 10) + 0.1f, 0);

		pVtx += 4;		//ポインタ座標を四つ進める
		nMath /= 10;	//割る10をして次の桁に
	}

	//バッファのアンロック
	g_pVtxBuffScore->Unlock();

}

int GetScore(void)
{
	return g_nScore;
}
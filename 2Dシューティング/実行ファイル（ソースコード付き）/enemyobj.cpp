//##################################################################################################################################################################//
//
// 敵オブジェクト処理 [enemyobj.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//　インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "enemyobj.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//　マクロ定義
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define MAX_ENEOBJ		(6)			// オブジェクトの種類
#define ENEMY_OBJ		(6/2)		// 列に並ぶオブジェクトの数
#define MINUS_COUNT		(3)			// 列に並ぶオブジェクトの数（計算用）
#define HALF_MATH		(2)			// 半分
#define OBJ_ENEMY_X		(310)		// x座標
#define OBJ_NEXT_X		(530)		// x座標となりの
#define OBJ_ENEMY_Y		(120)		// y座標
#define OBJ_NEXT_Y		(280)		// y座標となりの
#define OBJ_WIDTH		(220)		// オブジェクトの横幅
#define COLOR_RED		(255)		// 赤色
#define COLOR_BULUE		(255)		// 青色
#define COLOR_GREEN		(255)		// 緑色
#define COLOR_ALPHA		(255)		// アルファ値
#define POINT_X			(110)		// 中心座標X
#define POINT_Y			(80)		// 中心座標Y

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureEneObjct = NULL;		//テクスチャへのポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEneObjct = NULL;	//頂点バッファへのポインタ
ENEOBJCT g_EneObj[MAX_ENEOBJ];
float g_fpos[MAX_ENEOBJ];

//オブジェクトの初期化処理
HRESULT InitEnObjct(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ
	VERTEX_2D *pVtx;			//VERTEXのポインタ

	for (int nCounterEO = 0; nCounterEO < MAX_ENEOBJ; nCounterEO++)
	{
		g_EneObj[nCounterEO].fLife = 0;
		g_EneObj[nCounterEO].objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EneObj[nCounterEO].EneOState = ENEMY_OBJ_NORMAL;
		g_fpos[nCounterEO] = 0.0f;
	}

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemyobj.png", &g_pTextureEneObjct);				

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEOBJ, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEneObjct, NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファのロック
	g_pVtxBuffEneObjct->Lock(0, 0, (void**)&pVtx, 0);

	// オブジェクト配置
	for (int nCunObj = 0; nCunObj < MAX_ENEOBJ; nCunObj++)
	{
		if (nCunObj < ENEMY_OBJ)
		{
			pVtx[0].pos = D3DXVECTOR3((OBJ_ENEMY_X + (float)nCunObj * OBJ_WIDTH), OBJ_NEXT_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((OBJ_ENEMY_X + (float)nCunObj * OBJ_WIDTH), OBJ_ENEMY_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((OBJ_NEXT_X + (float)nCunObj * OBJ_WIDTH), OBJ_NEXT_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((OBJ_NEXT_X + (float)nCunObj * OBJ_WIDTH), OBJ_ENEMY_Y, 0.0f);

			g_EneObj[nCunObj].objpos = D3DXVECTOR3(((OBJ_ENEMY_X + POINT_X) + (float)nCunObj * OBJ_WIDTH), POINT_Y + OBJ_ENEMY_Y, 0.0f);
		}
		else if (nCunObj >= ENEMY_OBJ)
		{
			pVtx[0].pos = D3DXVECTOR3((OBJ_ENEMY_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_ENEMY_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((OBJ_ENEMY_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_NEXT_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((OBJ_NEXT_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_ENEMY_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((OBJ_NEXT_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_NEXT_Y, 0.0f);

			g_EneObj[nCunObj].objpos = D3DXVECTOR3(((OBJ_ENEMY_X + POINT_X) + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_ENEMY_Y - POINT_Y , 0.0f);
		}

		//テクスチャの頂点座標
		pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0, 0);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

		//rhwの設定		=	1.0f固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定・赤・緑・青
		pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

		//pVtxを4進める
		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffEneObjct->Unlock();
	//S_OKを返す
	return S_OK;	
}

//オブジェクトの終了処理
void UninitEnObjct(void)
{
	if (g_pVtxBuffEneObjct != NULL)
	{
		g_pVtxBuffEneObjct->Release();
		g_pVtxBuffEneObjct = NULL;
	}
	if (g_pTextureEneObjct != NULL)
	{
		g_pTextureEneObjct->Release();
		g_pTextureEneObjct = NULL;
	}
}

//オブジェクトの更新処理
void UpdateEnObjct(void)
{
	VERTEX_2D *pVtx;			//VERTEXのポインタ
	// 頂点バッファのロック
	g_pVtxBuffEneObjct->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCunObj = 0; nCunObj < MAX_ENEOBJ; nCunObj++)
	{
		if (g_EneObj[nCunObj].EneOState == ENEMY_OBJ_NORMAL)
		{
			if (nCunObj < ENEMY_OBJ)
			{
				pVtx[0].pos = D3DXVECTOR3((OBJ_ENEMY_X + (float)nCunObj * OBJ_WIDTH), OBJ_NEXT_Y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((OBJ_ENEMY_X + (float)nCunObj * OBJ_WIDTH), OBJ_ENEMY_Y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((OBJ_NEXT_X + (float)nCunObj * OBJ_WIDTH), OBJ_NEXT_Y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((OBJ_NEXT_X + (float)nCunObj * OBJ_WIDTH), OBJ_ENEMY_Y, 0.0f);

				g_EneObj[nCunObj].objpos = D3DXVECTOR3(((OBJ_ENEMY_X + POINT_X) + (float)nCunObj * OBJ_WIDTH), POINT_Y + OBJ_ENEMY_Y, 0.0f);
			}
			else if (nCunObj >= ENEMY_OBJ)
			{
				pVtx[0].pos = D3DXVECTOR3((OBJ_ENEMY_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_ENEMY_Y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((OBJ_ENEMY_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_NEXT_Y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((OBJ_NEXT_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_ENEMY_Y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((OBJ_NEXT_X + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_NEXT_Y, 0.0f);

				g_EneObj[nCunObj].objpos = D3DXVECTOR3(((OBJ_ENEMY_X + POINT_X) + ((float)nCunObj - MINUS_COUNT) * OBJ_WIDTH), SCREEN_HEIGHT - OBJ_ENEMY_Y - POINT_Y, 0.0f);
			}
			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

			//rhwの設定		=	1.0f固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定・赤・緑・青
			pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

			//pVtxを4進める
			pVtx += 4;
		}
	}
	//頂点バッファのアンロック
	g_pVtxBuffEneObjct->Unlock();
}

//オブジェクトの描画処理
void DrawEnObjct(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEneObjct, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureEneObjct);

	for (int nCunObj = 0; nCunObj < MAX_ENEOBJ; nCunObj++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCunObj * 4, 2);
	}
}

bool HitEnObjct(float nDamage)
{
	return true;
}


//オブジェクトの情報
ENEOBJCT *GetEnObjct(void)
{
	return &g_EneObj[0];
}

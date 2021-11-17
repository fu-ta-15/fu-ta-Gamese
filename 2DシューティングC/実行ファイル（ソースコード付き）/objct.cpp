//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//


#include "objct.h"
#include "hp.h"

#define MAX_OBJ			(OBJTYPE_MAX)	//オブジェクトの種類
#define MAX_ENEMYOBJ	(OBJTYPE_ENEMY)
#define OBJ__WARTER		(300)
#define OBJ_ENEMY_X		(300)
#define COLOR_RED		(255)			//赤色
#define COLOR_BULUE		(255)			//青色
#define COLOR_GREEN		(255)			//緑色
#define COLOR_ALPHA		(255)			//アルファ値
#define POINT_X			(650)			//中心座標X
#define POINT_Y			(520)			//中心座標Y


//グローバル宣言
LPDIRECT3DTEXTURE9 g_apTextureObjct[MAX_OBJ] = {};	//テクスチャへのポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObjct = NULL;		//頂点バッファへのポインタ
OBJCT g_Obj;
int g_nCounter;

//オブジェクトの初期化処理
HRESULT InitObjct(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ
	VERTEX_2D *pVtx;			//VERTEXのポインタ

	g_Obj.nDownLife = 0;
	g_Obj.objpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nCounter = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\01.png", &g_apTextureObjct[0]);				//テクスチャ「城」
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\life-obj.png", &g_apTextureObjct[1]);		//テクスチャ「HPオブジェクト」
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\outelia.png", &g_apTextureObjct[2]);			//テクスチャ「水源１」
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\outelia.png", &g_apTextureObjct[3]);			//テクスチャ「水源２」
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\outelia.png", &g_apTextureObjct[4]);			//テクスチャ「水源３」
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\outelia.png", &g_apTextureObjct[5]);			//テクスチャ「水源４」

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OBJ, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffObjct, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffObjct->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCunObj = 0; nCunObj < MAX_OBJ; nCunObj++)
	{
		if (nCunObj == OBJTYPE_CASTLE)
		{
			pVtx[0].pos = D3DXVECTOR3(540, 580, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(540, 380, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(740, 580, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(740, 380, 0.0f);

			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

		}
		else if (nCunObj == OBJTYPE_HPBAR)
		{
			pVtx[0].pos = D3DXVECTOR3(550, 470, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(550, 260, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(750, 470, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(750, 260, 0.0f);

			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------//
		//水源エリアの設定
		//-------------------------------------------------------------------------------------------------------------------------------------------------//
		if (nCunObj == OBJTYPE_WATER1)
		{
			pVtx[0].pos = D3DXVECTOR3(0, OBJ__WARTER, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(OBJ__WARTER, OBJ__WARTER, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(OBJ__WARTER, 0, 0.0f);

			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f / 2, 0);

		}
		else if (nCunObj == OBJTYPE_WATER2)
		{
			pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, SCREEN_HEIGHT - OBJ__WARTER, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(OBJ__WARTER, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(OBJ__WARTER, SCREEN_HEIGHT - OBJ__WARTER, 0.0f);

			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f / 2, 0);

		}
		else if (nCunObj == OBJTYPE_WATER3)
		{
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - OBJ__WARTER, SCREEN_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - OBJ__WARTER, SCREEN_HEIGHT - OBJ__WARTER, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - OBJ__WARTER, 0.0f);

			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f / 2, 0);

		}
		else if (nCunObj == OBJTYPE_WATER4)
		{
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - OBJ__WARTER, OBJ__WARTER, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - OBJ__WARTER, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, OBJ__WARTER, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);

			//テクスチャの頂点座標
			pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f / 2, 0);

		}

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
	//「城」の中心座標
	g_Obj.objpos = D3DXVECTOR3(POINT_X, POINT_Y, 0.0f);
	//頂点バッファのアンロック
	g_pVtxBuffObjct->Unlock();
	return S_OK;	//S_OKを返す
}

//オブジェクトの終了処理
void UninitObjct(void)
{
	if (g_pVtxBuffObjct != NULL)
	{
		g_pVtxBuffObjct->Release();
		g_pVtxBuffObjct = NULL;
	}

	for (int nCunObj = 0; nCunObj < MAX_OBJ; nCunObj++)
	{
		if (g_apTextureObjct[nCunObj] != NULL)
		{
			g_apTextureObjct[nCunObj]->Release();
			g_apTextureObjct[nCunObj] = NULL;
		}
	}
}

//オブジェクトの更新処理
void UpdateObjct(void) 
{

	VERTEX_2D *pVtx;			//VERTEXのポインタ
	
	//頂点バッファのロック
	g_pVtxBuffObjct->Lock(0, 0, (void**)&pVtx, 0);

	g_nCounter += 1;

	if ((g_nCounter % 60) == 0)
	{
		pVtx += (4 * OBJTYPE_WATER1);

		for (int nTexCounter = 2; nTexCounter < MAX_OBJ; nTexCounter++)
		{
			if (nTexCounter == OBJTYPE_WATER1 || nTexCounter == OBJTYPE_WATER2 || nTexCounter == OBJTYPE_WATER3 || nTexCounter == OBJTYPE_WATER4)
			{
				//テクスチャの頂点座標
				pVtx[0].tex = D3DXVECTOR2(1.0 / 2, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0 / 2, 0);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 1, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 1, 0);
			}
			pVtx += 4;
		}
	}
	else if ((g_nCounter % 110) == 0)
	{
		pVtx += (4 * OBJTYPE_WATER1);

		for (int nTexCounter = 2; nTexCounter < MAX_OBJ; nTexCounter++)
		{
			if (nTexCounter == OBJTYPE_WATER1 || nTexCounter == OBJTYPE_WATER2 || nTexCounter == OBJTYPE_WATER3 || nTexCounter == OBJTYPE_WATER4)
			{
				//テクスチャの頂点座標
				pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0, 0);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 2, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 2, 0);
			}
			pVtx += 4;
		}
		g_nCounter = 0;
	}
	//頂点バッファのアンロック
	g_pVtxBuffObjct->Unlock();
}

//オブジェクトの描画処理
void DrawObjct(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffObjct, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCunObj = 0; nCunObj < MAX_OBJ; nCunObj++)
	{
		pDevice->SetTexture(0, g_apTextureObjct[nCunObj]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCunObj * 4, 2);
	}
}

bool HitObjct(float nDamage)
{
	g_Obj.nDownLife = nDamage;

	DownHp(g_Obj.nDownLife);
	return true;
}


//オブジェクトの情報
OBJCT *GetObjct(void)
{
	return &g_Obj;
}

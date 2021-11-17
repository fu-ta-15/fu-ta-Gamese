//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//


#include "hp.h"
#include "input.h"
#include "fade.h"

#define HP_TYPE			(HPTYPE_MAX)	//体力ゲージの種類
#define MAX_HP			(170)			//体力
#define COLOR_RED		(255)			//赤色
#define COLOR_BULUE		(255)			//青色
#define COLOR_GREEN		(255)			//緑色
#define COLOR_ALPHA		(255)			//アルファ値


//グローバル宣言
LPDIRECT3DTEXTURE9 g_apTextureHp[HP_TYPE] = {};	//テクスチャへのポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;		//頂点バッファへのポインタ
HP g_Hp;
int g_nCun;


HRESULT InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	 g_Hp.fHpGreen = MAX_HP;
	 g_Hp.fHpRed = MAX_HP;
	 g_nCun = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\life-geage.png", &g_apTextureHp[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\life-geage.png", &g_apTextureHp[1]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * HP_TYPE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffHp, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHp = 0; nCntHp < HP_TYPE; nCntHp++)
	{
		if (nCntHp == HPTYPE_RED)
		{

			pVtx[0].pos = D3DXVECTOR3(566, 380, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(566, 355, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpRed), 380, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpRed), 355, 0.0f);

			//rhwの設定		=	1.0f固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定・赤・緑・青
			pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, 0, 0, COLOR_ALPHA);
			pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, 0, 0, COLOR_ALPHA);
			pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, 0, 0, COLOR_ALPHA);
			pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, 0, 0, COLOR_ALPHA);


			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

		}
		else if (nCntHp == HPTYPE_GREEN)
		{
			pVtx[0].pos = D3DXVECTOR3(566, 380, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(566, 355, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpGreen), 380, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpGreen), 355, 0.0f);

			//rhwの設定		=	1.0f固定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定・赤・緑・青
			pVtx[0].col = D3DCOLOR_RGBA(0, COLOR_GREEN, 0, COLOR_ALPHA);
			pVtx[1].col = D3DCOLOR_RGBA(0, COLOR_GREEN, 0, COLOR_ALPHA);
			pVtx[2].col = D3DCOLOR_RGBA(0, COLOR_GREEN, 0, COLOR_ALPHA);
			pVtx[3].col = D3DCOLOR_RGBA(0, COLOR_GREEN, 0, COLOR_ALPHA);


			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

		}
		pVtx += 4;
	}

	g_pVtxBuffHp->Unlock();

	return S_OK;
}


void UninitHp(void)
{
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
	for (int nCntHp = 0; nCntHp < HP_TYPE; nCntHp++)
	{
		if (g_apTextureHp[nCntHp] != NULL)
		{
			g_apTextureHp[nCntHp]->Release();
			g_apTextureHp[nCntHp] = NULL;
		}
	}
}

//体力の更新
void UpdateHp(void)
{
	VERTEX_2D *pVtx;
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (4 * HPTYPE_GREEN);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(566, 380, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(566, 355, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpGreen), 380, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpGreen), 355, 0.0f);

	if (g_Hp.fHpGreen <= 0)
	{
		g_nCun += 1;

		if (g_nCun >= 100)
		{
			SetFade(FADE_OUT, MODE_RANKING);
		}
	}
	g_pVtxBuffHp->Unlock();
}

void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntHp = 0; nCntHp < HP_TYPE; nCntHp++)
	{
		pDevice->SetTexture(0, g_apTextureHp[nCntHp]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHp * 4, 2);
	}
}

bool DownHp(float nDamage)
{
	g_Hp.fHpGreen -= nDamage;

	VERTEX_2D *pVtx;
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (4 * HPTYPE_GREEN);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(566, 380, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(566, 355, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpGreen), 380, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(566 + ((float)g_Hp.fHpGreen), 355, 0.0f);

	g_pVtxBuffHp->Unlock();


	return true;
}


HP *GetHp(void)
{
	return &g_Hp;
}

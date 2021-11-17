//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "effect.h"
#include "enemy.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//マクロ定義
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define MAX_EFFECT		(256)
#define COLOR_RED		(100)
#define COLOR_BULUE		(130)
#define COLOR_GREEN		(10)

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//弾の構造体
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR color;
	int Idx;
	int nLife;
	float nSize;
	bool bUse;			//使用しているかどうか
}EFFECT;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//グローバル宣言
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
EFFECT g_aEffect[MAX_EFFECT];
float fYard[MAX_EFFECT];
float fMetth[MAX_EFFECT];
int g_Idx;


HRESULT InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\effe.png", &g_pTextureEffect);

	for (int nCounterEffect = 0; nCounterEffect < MAX_EFFECT; nCounterEffect++)
	{
		g_aEffect[nCounterEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCounterEffect].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCounterEffect].nLife = 0;
		g_aEffect[nCounterEffect].nSize = 0.0f;
		g_aEffect[nCounterEffect].Idx = 0;
		g_aEffect[nCounterEffect].bUse = false;
		fYard[nCounterEffect] = 0.0f;
		fMetth[nCounterEffect] = 0.0f;
	}


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCounterEffect = 0; nCounterEffect < MAX_EFFECT; nCounterEffect++)
	{
		//各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x - g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y + g_aEffect[nCounterEffect].nSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x - g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y - g_aEffect[nCounterEffect].nSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x + g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y + g_aEffect[nCounterEffect].nSize, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x + g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y - g_aEffect[nCounterEffect].nSize, 0.0f);
																	
		//rhwの設定		=	1.0f固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定・赤・緑・青
		pVtx[0].col = D3DXCOLOR(1.0f, 0.6f, 0.4f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.6f, 0.4f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.6f, 0.4f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.6f, 0.4f, 1.0f);

		//テクスチャの頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
	return S_OK;

}


void UninitEffect(void)
{
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}

	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

}


void UpdateEffect(void)
{
	VERTEX_2D *pVtx;
	EFFECT *pEffect;
	ENEMY *pEnemy;

	pEnemy = GetEnemy();
	pEffect = &g_aEffect[0];

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCounterEffect = 0; nCounterEffect < MAX_EFFECT; nCounterEffect++)
	{

		if (g_aEffect[nCounterEffect].bUse == true)
		{
			g_aEffect[nCounterEffect].nLife--;
			g_aEffect[nCounterEffect].nSize += 1.0f;


			//各頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x - g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y + g_aEffect[nCounterEffect].nSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x - g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y - g_aEffect[nCounterEffect].nSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x + g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y + g_aEffect[nCounterEffect].nSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x + g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y - g_aEffect[nCounterEffect].nSize, 0.0f);

			if (g_aEffect[nCounterEffect].nLife <= 0)
			{
				g_aEffect[nCounterEffect].bUse = false;
				g_aEffect[nCounterEffect].nSize = 0.0f;
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffEffect->Unlock();
}


void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	for (int nCounterEffect = 0; nCounterEffect < MAX_EFFECT; nCounterEffect++)
	{
		if (g_aEffect[nCounterEffect].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCounterEffect * 4, 2);
		}
	}

	//レンダーステートの設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
}


void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR color)
{
	EFFECT *pEffect;
	VERTEX_2D *pVtx;


	pEffect = &g_aEffect[0];


	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCounterEffect = 0; nCounterEffect < MAX_EFFECT; nCounterEffect++)
	{
		if (g_aEffect[nCounterEffect].bUse == false)
		{
			g_aEffect[nCounterEffect].pos = pos;
			g_aEffect[nCounterEffect].color = color;

			//各頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x - g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y + g_aEffect[nCounterEffect].nSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x - g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y - g_aEffect[nCounterEffect].nSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x + g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y + g_aEffect[nCounterEffect].nSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCounterEffect].pos.x + g_aEffect[nCounterEffect].nSize, g_aEffect[nCounterEffect].pos.y - g_aEffect[nCounterEffect].nSize, 0.0f);

			//頂点カラーの設定・赤・緑・青
			pVtx[0].col = g_aEffect[nCounterEffect].color;
			pVtx[1].col = g_aEffect[nCounterEffect].color;
			pVtx[2].col = g_aEffect[nCounterEffect].color;
			pVtx[3].col = g_aEffect[nCounterEffect].color;


			g_aEffect[nCounterEffect].nLife = 500;

			g_aEffect[nCounterEffect].bUse = true;
			break;
		}
		pVtx += 4;
	}

g_pVtxBuffEffect->Unlock();
}

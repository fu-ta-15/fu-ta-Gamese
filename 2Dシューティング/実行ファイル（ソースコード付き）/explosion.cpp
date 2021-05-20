//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//
#include "explosion.h"
#include "player.h"
#include "input.h"
#include "enemy.h"

//マクロ定義
#define MAX_EXPLOSION		(256)
#define EXPLOSION_SIZE_XY	(50)
#define EXPLOSION_ANINM		(1/8)
#define COLOR_RED		(100)
#define COLOR_BULUE		(130)
#define COLOR_GREEN		(10)

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR color;
	bool bUse;			//使用しているかどうか
}EXPLOSION;

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION];
int g_fAnim = 0;
int g_fPaternAnim[MAX_EXPLOSION];
float g_fSizeExp;

//初期化処理
HRESULT InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion000.png", &g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].bUse = false;
		g_fPaternAnim[nCntExplosion] = 0;
	}
	g_fSizeExp = EXPLOSION_SIZE_XY;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_fSizeExp / 2, g_aExplosion[nCntExplosion].pos.y + g_fSizeExp / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_fSizeExp / 2, g_aExplosion[nCntExplosion].pos.y - g_fSizeExp / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_fSizeExp / 2, g_aExplosion[nCntExplosion].pos.y + g_fSizeExp / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_fSizeExp / 2, g_aExplosion[nCntExplosion].pos.y - g_fSizeExp / 2, 0.0f);

		//rhwの設定		=	1.0f固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定・赤・緑・青
		pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, 225);
		pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, 225);
		pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, 225);
		pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, 225);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / 8, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 8, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();


	return S_OK;
}

//終了処理
void UninitExplosion(void)
{
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//更新処理
void UpdateExplosion(void)
{
	EXPLOSION *pExplosion;
	PLAYER *pPlayer;
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;
	pExplosion = &g_aExplosion[0];
	pEnemy = GetEnemy();
	pPlayer = GetPlayer();

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	g_fAnim++;


	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			if ((g_fAnim % 4) == 0)
			{
				if ((g_fPaternAnim[nCntExplosion] + 1) % 7 == 0)
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}
				g_fPaternAnim[nCntExplosion] = (g_fPaternAnim[nCntExplosion] + 1) % 8;
			}
				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_fSizeExp, g_aExplosion[nCntExplosion].pos.y + g_fSizeExp, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_fSizeExp, g_aExplosion[nCntExplosion].pos.y - g_fSizeExp, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_fSizeExp, g_aExplosion[nCntExplosion].pos.y + g_fSizeExp, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_fSizeExp, g_aExplosion[nCntExplosion].pos.y - g_fSizeExp, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion])), 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + (1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion])), 0);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion] + 1), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion] + 1), 0);
		}
		//画面外チェック
		if (g_aExplosion[nCntExplosion].pos.y - EXPLOSION_SIZE_XY / 2 < 0)
		{//弾がｙ方向の画面外に出た場合
			g_aExplosion[nCntExplosion].bUse = false;
		}
		pVtx += 4;	//4頂点ずつずれる
	}
	g_pVtxBuffExplosion->Unlock();
}

//描画処理
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//弾の設定処理
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR color)
{
	VERTEX_2D *pVtx;
	PLAYER *pPlayer;
	ENEMY *pEnemy;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pEnemy = GetEnemy();
	pPlayer = GetPlayer();
	g_aExplosion[0];
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			//位置の設定
			g_aExplosion[nCntExplosion].pos.x = pos.x;
			g_aExplosion[nCntExplosion].pos.y = pos.y;
			//色の設定
			g_aExplosion[nCntExplosion].color = color;

			if (pPlayer->bSkills != true)
			{
				g_fSizeExp = 25;

				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_fSizeExp, g_aExplosion[nCntExplosion].pos.y + g_fSizeExp, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_fSizeExp, g_aExplosion[nCntExplosion].pos.y - g_fSizeExp, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_fSizeExp, g_aExplosion[nCntExplosion].pos.y + g_fSizeExp, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_fSizeExp, g_aExplosion[nCntExplosion].pos.y - g_fSizeExp, 0.0f);

				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion])), 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + (1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion])), 0);
				pVtx[2].tex = D3DXVECTOR2(1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion] + 1), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 8 * ((float)g_fPaternAnim[nCntExplosion] + 1), 0);

			}

			pVtx[0].col = g_aExplosion[nCntExplosion].color;
			pVtx[1].col = g_aExplosion[nCntExplosion].color;
			pVtx[2].col = g_aExplosion[nCntExplosion].color;
			pVtx[3].col = g_aExplosion[nCntExplosion].color;
			
			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}
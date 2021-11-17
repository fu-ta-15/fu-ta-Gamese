#include "score.h"
#include "time.h"
#include "fade.h"

//マクロ変数
#define MAX_TIME	(3)


//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;;		//テクスチャへのポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		//頂点バッファへのポインタ
int g_nTime;
int g_nTimeCounter;

//=====================================================================================================================================================================//
//初期化処理
//=====================================================================================================================================================================//
HRESULT InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//変数の初期化
	g_nTime = 120;
	g_nTimeCounter = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number001.png", &g_pTextureTime);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTime, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴン・テクスチャの初期化設定
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{

		//各頂点座標
		pVtx[0].pos = D3DXVECTOR3(550 + ((float)nCntTime * 50), 0 + 110, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(550 + ((float)nCntTime * 50), 0 + 10, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600 + ((float)nCntTime * 50), 0 + 110, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(600 + ((float)nCntTime * 50), 0 + 10, 0.0f);

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

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0);

		//頂点4進める
		pVtx += 4;
	}

	//頂点バッファのアンロック
	g_pVtxBuffTime->Unlock();

	AddTime(0);

	return S_OK;
}

//=====================================================================================================================================================================//
//終了処理
//=====================================================================================================================================================================//
void UninitTime(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
	//テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
}

//=====================================================================================================================================================================//
//更新処理
//=====================================================================================================================================================================//
void UpdateTime(void)
{

	g_nTimeCounter += 1;

	if ((g_nTimeCounter % 60) == 0)
	{
		AddTime(1);
		if (g_nTime <= 0)
		{
			SetFade(FADE_OUT, MODE_RANKING);
		}
	}
}

//=====================================================================================================================================================================//
//描画処理
//=====================================================================================================================================================================//
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}

}

//=====================================================================================================================================================================//
//タイマーの加算
//=====================================================================================================================================================================//
void AddTime(int nValue)
{
	int nMath = 1000;		//タイマー計算
	int aNumber[MAX_TIME];	//タイマーのナンバー
	VERTEX_2D *pVtx;		//ポインタ

	//バッファのロック
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	g_nTime -= nValue;		//タイマー

	//タイマーの変更
	for (int nCunTimeNum = 0; nCunTimeNum < MAX_TIME; nCunTimeNum++)
	{
		//タイマーの計算
		aNumber[nCunTimeNum] = (g_nTime%nMath / (nMath / 10));

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2((float)aNumber[nCunTimeNum] / 10, 1.0f);
		pVtx[1].tex = D3DXVECTOR2((float)aNumber[nCunTimeNum] / 10, 0);
		pVtx[2].tex = D3DXVECTOR2(((float)aNumber[nCunTimeNum] / 10) + 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(((float)aNumber[nCunTimeNum] / 10) + 0.1f, 0);

		pVtx += 4;		//ポインタ座標を四つ進める
		nMath /= 10;	//割る10をして次の桁に
	}
	//バッファのアンロック
	g_pVtxBuffTime->Unlock();
}

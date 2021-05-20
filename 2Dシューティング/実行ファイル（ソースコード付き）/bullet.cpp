//##################################################################################################################################################################//
//
// 弾処理 [bullet.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "bullet.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "objct.h"
#include "effect.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// マクロ定義
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define BULLET_SIZE_XY					(20)						// 弾のサイズ
#define COLOR_RED						(255)						// 赤色
#define COLOR_BULUE						(255)						// 青色
#define COLOR_GREEN						(255)						// 緑色
#define COLOR_ALPHA						(255)						// アルファ値
#define RHW								(10/10)						// rhwの数値1.0固定
#define HALF_MATH						(2)							// 半分
#define PVTEX_MAX						(4)							// pVtx
#define PLAYER_GREEN_BULLET				(pEnemy->bUse == true && g_aBullet[nCntBullet].state == BULLET_TYPE_PLAYER_GREEN && pEnemy->Type != ENEMY_TYPE_FIRE && pEnemy->state == ENEMYSTATE_NORMAL)
#define PLAYER_RED_BULLET				(pEnemy->bUse == true && g_aBullet[nCntBullet].state == BULLET_TYPE_PLAYER_RED && pEnemy->Type != ENEMY_TYPE_TANK && pEnemy->state == ENEMYSTATE_NORMAL)

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// グローバル宣言
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
LPDIRECT3DTEXTURE9 g_apTextureBullet[BULLET_TYPE_MAX] = {};	// テクスチャの情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;			// 頂点バッファの情報
BULLET g_aBullet[MAX_BULLET];								// 弾の情報
D3DXVECTOR3 pos;											// 位置
D3DXCOLOR color;											// 色
int g_nType;												// 弾のタイプ
float g_fAngleBullet;										// 対角線の角度

//=====================================================================================================================================================================//
// 初期化処理
//=====================================================================================================================================================================//
HRESULT InitBullet(void)
{
	VERTEX_2D *pVtx;			// 頂点のポインタ
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// グローバル変数の初期化
	g_nType = BULLET_TYPE_PLAYER_GREEN;
	g_fAngleBullet = 0.0f;							
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			
	color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\refe.png", &g_apTextureBullet[BULLET_TYPE_PLAYER_GREEN]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\warter.png", &g_apTextureBullet[BULLET_TYPE_PLAYER_RED]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemybullet01.png", &g_apTextureBullet[BULLET_TYPE_ENEMY]);

	// 弾の初期化設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
		g_aBullet[nCntBullet].state = BULLET_TYPE_PLAYER_GREEN;		
		g_aBullet[nCntBullet].nLife = 100;							
		g_aBullet[nCntBullet].bUse = false;							
	}

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * PVTEX_MAX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}
	// 頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// 各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);

		// rhwの設定		=	1.0f固定
		pVtx[0].rhw = RHW;
		pVtx[1].rhw = RHW;
		pVtx[2].rhw = RHW;
		pVtx[3].rhw = RHW;

		// 頂点カラーの設定・赤・緑・青
		pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0, 0);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

		// プラス4進める
		pVtx += PVTEX_MAX;
	}
	
	// 頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();

	// 初期化完了
	return S_OK;
}

//=====================================================================================================================================================================//
// 終了処理
//=====================================================================================================================================================================//
void UninitBullet(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
	for (int nCuntBullet = 0; nCuntBullet < BULLET_TYPE_MAX; nCuntBullet++)
	{// テクスチャの開放
		if (g_apTextureBullet[nCuntBullet] != NULL)
		{
			g_apTextureBullet[nCuntBullet]->Release();
			g_apTextureBullet[nCuntBullet] = NULL;
		}
	}
	StopSound(SOUND_LABEL_SE_SHOT_E);	//BGM
}

//=====================================================================================================================================================================//
// 更新処理
//=====================================================================================================================================================================//
void UpdateBullet(void)
{
	// 構造体のポインタ
	VERTEX_2D *pVtx;
	BULLET *pBullet;
	ENEMY *pEnemy;
	PLAYER *pPlayer;
	OBJCT *pObj;
	ENEMY_FLAWAR *pFlawar;

	// 構造体の情報取得
	pBullet = &g_aBullet[0];
	pObj = GetObjct();
	pPlayer = GetPlayer();

	// 頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true )
		{
			// 位置の更新
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;

			// ライフの減少
			g_aBullet[nCntBullet].nLife -= 2;

			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);

			// 敵の情報を入れなおす
			pEnemy = GetEnemy();
			pFlawar = GetFlawar();

			// 敵との当たり判定
			for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++, pFlawar++)
			{// 敵が存在している・プレイヤーの弾である場合
			 // プレイヤーの条件
				if (PLAYER_GREEN_BULLET || PLAYER_RED_BULLET)
				{
					// 敵とプレイヤーの弾の当たり判定
					if (pEnemy->pos.y + ENEMY_SIZE / 3 > g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / 3 &&
						pEnemy->pos.x + ENEMY_SIZE / 3 > g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / 3 &&
						pEnemy->pos.y - ENEMY_SIZE / 3 < g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / 3 &&
						pEnemy->pos.x - ENEMY_SIZE / 3 < g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / 3 )
					{
						// 弾の消滅
						g_aBullet[nCntBullet].bUse = false;

						// ダメージ
						int nDamage = 1;

						// 敵が被弾した
						HitEnemy(nCntEnemy, nDamage);
					}
				}
			}
			//プレイヤー・城と敵の弾の当たり判定
			if (g_aBullet[nCntBullet].state == BULLET_TYPE_ENEMY)
			{
				// プレイヤーの当たり判定
				if (pPlayer->Playerpos.y + 40 > g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / 3 &&
					pPlayer->Playerpos.x + 20 > g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / 3 &&
					pPlayer->Playerpos.y - 10 < g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / 3 &&
					pPlayer->Playerpos.x - 20 < g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / 3)
				{
					// 弾の消滅
					g_aBullet[nCntBullet].bUse = false;

					// ダメージ量
					int nDamage = 1;

					// プレイヤーが被弾した場合
					HitPlayer(nDamage);
				}
				// 城の当たり判定
				if (pObj->objpos.y + 60 > g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / 3 &&
					pObj->objpos.x + 60 > g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / 3 &&
					pObj->objpos.y - 60 < g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / 3 &&
					pObj->objpos.x - 60 < g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / 3)
				{
					// 弾の消滅
					g_aBullet[nCntBullet].bUse = false;

					// ダメージ量
					float nDamage = 3.2f;

					// 城が被弾した場合
					HitObjct(nDamage);

					// ダメージ音
					PlaySound(SOUND_LABEL_SE_HIT);
				}
			} 
			// 寿命チェック
			if (g_aBullet[nCntBullet].nLife == 0)
			{
				// 0になったらフォルスに変える
				g_aBullet[nCntBullet].bUse = false;
			}
			// 画面外チェック・弾がｙ方向の画面外に出た場合
			if (g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH < 0 || g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH > SCREEN_HEIGHT ||
				g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH < 0 || g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH > SCREEN_WIDTH)
			{
				// フォルスを代入
				g_aBullet[nCntBullet].bUse = false;
			}
		}
		// 4頂点ずつずれる
		pVtx += PVTEX_MAX;
	}
	// 頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================================================================================================================//
// 描画処理
//=====================================================================================================================================================================//
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	// プレイヤーの取得
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[nCntBullet].state]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * PVTEX_MAX, HALF_MATH);
		}
	}
}

//=====================================================================================================================================================================//
// 弾の設定処理
//=====================================================================================================================================================================//
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLET_TYPE Btype)
{
	//ポインタ
	BULLET *pBullet;
	PLAYER *pPlayer;
	OBJCT *pObj;
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;
	//情報取得
	pEnemy = GetEnemy();
	pPlayer = GetPlayer();
	pObj = GetObjct();
	pBullet = &g_aBullet[0];
	//頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);
	//弾の設定
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//使用されていない弾
			//プレイヤーのタイプ情報
			g_nType = pPlayer->player;

			// プレイヤー1の弾
			if (g_nType == BULLET_TYPE_PLAYER_GREEN)
			{
					// 弾の種類の情報
					g_aBullet[nCntBullet].state = BULLET_TYPE_PLAYER_GREEN;

					// 位置の設定
					g_aBullet[nCntBullet].pos.x = pos.x;
					g_aBullet[nCntBullet].pos.y = pos.y;

					// 移動の設定
					g_aBullet[nCntBullet].move.y = move.y;
					g_aBullet[nCntBullet].move.x = move.x;

					// 寿命の設定
					g_aBullet[nCntBullet].nLife = nLife;

					// 頂点座標
					pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);
			}
			else if (g_nType == BULLET_TYPE_PLAYER_RED)
			{// プレイヤー2の弾
				// 弾の種類の情報
				g_aBullet[nCntBullet].state = BULLET_TYPE_PLAYER_RED;

				// 位置の設定
				g_aBullet[nCntBullet].pos.x = pos.x;
				g_aBullet[nCntBullet].pos.y = pos.y;

				// 移動の設定
				g_aBullet[nCntBullet].move.y = move.y;
				g_aBullet[nCntBullet].move.x = move.x;

				// 寿命の設定
				g_aBullet[nCntBullet].nLife = nLife;

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / HALF_MATH, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / HALF_MATH, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / HALF_MATH, 0.0f);

			}
			if (Btype == BULLET_TYPE_ENEMY)
			{// 敵の弾
				// 弾の種類の情報
				g_aBullet[nCntBullet].state = BULLET_TYPE_ENEMY;
				PlaySound(SOUND_LABEL_SE_SHOT_E);

				// 位置の設定
				g_aBullet[nCntBullet].pos.x = pos.x;
				g_aBullet[nCntBullet].pos.y = pos.y;

				// 敵の弾と城の角度
				g_fAngleBullet = atan2f(g_aBullet[nCntBullet].pos.x - (float)pObj->objpos.x, g_aBullet[nCntBullet].pos.y - (float)pObj->objpos.y);

				// 移動の設定
				g_aBullet[nCntBullet].move.y = cosf(D3DX_PI-g_fAngleBullet)*HALF_MATH;
				g_aBullet[nCntBullet].move.x = sinf(D3DX_PI+g_fAngleBullet)*HALF_MATH;

				// 寿命の設定
				g_aBullet[nCntBullet].nLife = nLife;

				// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / 2, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZE_XY / 2, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / 2, g_aBullet[nCntBullet].pos.y + BULLET_SIZE_XY / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZE_XY / 2, g_aBullet[nCntBullet].pos.y - BULLET_SIZE_XY / 2, 0.0f);
			}
			// 弾を発射
			g_aBullet[nCntBullet].bUse = true;
			break;
		}
		pVtx += PVTEX_MAX;	// 頂点情報を4進める
	}
	// 頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}

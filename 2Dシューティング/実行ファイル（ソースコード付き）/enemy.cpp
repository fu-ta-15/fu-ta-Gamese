//##################################################################################################################################################################//
//
// 敵の処理 [enemy.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "bullet.h"
#include "enemy.h"
#include "input.h"
#include "score.h"
#include "objct.h"
#include "sound.h"
#include "explosion.h"
#include "enemyobj.h"
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//マクロ定義
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define MAX_ENYMY_TYPE				(ENEMY_TYPE_MAX)																							// 敵の種類
#define MAX_ENEMY					(256)																										// 敵の数
#define COLOR_RED					(255)																										// 赤色
#define COLOR_BULUE					(255)																										// 青色
#define COLOR_GREEN					(255)																										// 緑色
#define COLOR_ALPHA					(255)																										// アルファ値
#define MAX_COLOR					(255)																										// 色の最大値
#define COLOR_DAMAGEALPHA			(255/5)																										// ダメージ状態のアルファ値
#define VECTORO_ZERO				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))																				// VECTOROの初期化
#define MOVE_ZERO					(D3DXVECTOR3(0.0f, 0.0f, 0.0f))																				// スピードゼロ
#define COLOR_ZERO					(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))																			// 色の初期化
#define ALPHA_INTERVAL				(5)																											// アルファ値のインターバル
#define PVTEX_MAX					(4)																											// pVtx
#define TEXPOS_ONE					(1.0f)																										// テクスチャ座標１
#define TEXPOS_HALF					(0.5f)																										// テクスチャ座標２
#define TEXPOS_ZERO					(0.0f)																										// テクスチャ座標３
#define ADD_SCORE					(1)																											// スコア
#define FLAWAR_NUMBER				(4)																											// 花の敵の数
#define FLAWAR_LIFE					(4)																											// 花の体力
#define FLAWAR_P					(1100)																										// 花の敵の位置
#define FLAWAR_O					(150)																										// 花の敵の位置
#define FLAWAR_S					(810)																										// 花の敵の位置
#define FLAWAR_ZERO					(atan2f(g_EnemySetPos[nIdx].x - FLAWAR_P, g_EnemySetPos[nIdx].y - FLAWAR_O))								// 花ナンバー0
#define FLAWAR_ONE					(atan2f(g_EnemySetPos[nIdx].x - FLAWAR_O, g_EnemySetPos[nIdx].y - FLAWAR_O))								// 花ナンバー1
#define FLAWAR_TWO					(atan2f(g_EnemySetPos[nIdx].x - FLAWAR_O, g_EnemySetPos[nIdx].y - FLAWAR_S))								// 花ナンバー2
#define FLAWAR_THREE				(atan2f(g_EnemySetPos[nIdx].x - FLAWAR_P, g_EnemySetPos[nIdx].y - FLAWAR_S))								// 花ナンバー3
#define GUS_POS_RIGHT				(D3DXVECTOR3(SCREEN_WIDTH + 10, (SCREEN_HEIGHT - 600) + (float)g_Gus[nIdx].AddPosRand, 0.0f))				// 黒い敵右側
#define GUS_POS_LEFT				(D3DXVECTOR3(0 - 10, (SCREEN_HEIGHT - 600) + (float)g_Gus[nIdx].AddPosRand, 0.0f))							// 黒い敵左側
#define ENEMY_GUS_LIFE				(1)																											// 黒い敵の体力
#define GUS_DAMAGE					(1.0f)																										// 黒い敵の攻撃力
#define FIRE_APPER					(7)																											// 火の出現
#define TANK_APPER					(7)																											// 戦車の出現
#define GUS_FLAWAR					(D3DXCOLOR(0.8f, 0.0f, 0.8f, 1.0f))																			// 花専用
#define GUS_FIRE					(D3DXCOLOR(0.7f, 0.2f, 0.3f, 1.0f))																			// 火専用
#define GUS_TANK					(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f))																			// 戦車専用
#define GUS_NORMAL					(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))																			// ノーマル
#define CASTLE_ANGLE				(atan2f(g_EnemySetPos[nIdx].x - (float)pObj->objpos.x, g_EnemySetPos[nIdx].y - (float)pObj->objpos.y))		// 城との角度
#define CASTLE_POS					(60)																										// 城の中心からの距離
#define FLAWAR_BULLET				(300)																										// 花の弾発射時間
#define FRAME_MATH					(60)																										// 


//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//グローバル宣言
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMY_TYPE_MAX] = {};	//テクスチャへのポイント
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				//頂点バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									//敵の種類
D3DXVECTOR3 g_EnemySetPos[MAX_ENEMY];						//敵の位置
ENEMY_GUS g_Gus[MAX_ENEMY];									//黒の敵
ENEMY_FIRE g_Fire;											//火の敵
ENEMY_TANK g_Tank;											//洗車の敵
ENEMY_FLAWAR g_Flawar[FLAWAR_NUMBER];						//花の敵
int g_anApperCounter;										//ランダムカウンター
int g_nCounterEnemy;										//敵の番号
float g_fAngleEnemy;										//敵の対角線の角度
int g_fEnemyAlpha[MAX_ENEMY];								//敵の透明度
int g_anFlame[MAX_ENEMY];									//花の敵

//=====================================================================================================================================================================//
// 初期化処理
//=====================================================================================================================================================================//
HRESULT InitEnemy(void)
{
	//ポインタ
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	//デバイスの取得
	pDevice = GetDevice();
	pEnemy = &g_aEnemy[0];

	// ランダム
	srand((unsigned int)time(NULL));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy0.png", &g_apTextureEnemy[ENEMY_TYPE_GUS]);			//黒の敵
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\0202.png", &g_apTextureEnemy[ENEMY_TYPE_FIRE]);			//火の敵
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\enemy0-1-1.png", &g_apTextureEnemy[ENEMY_TYPE_TANK]);	//洗車の敵
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\flowers.png", &g_apTextureEnemy[ENEMY_TYPE_FLAWAR]);		//花の敵

	//敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_anFlame[nCntEnemy] = 0;
		// 敵の構造体
		g_aEnemy[nCntEnemy].pos = VECTORO_ZERO;
		g_aEnemy[nCntEnemy].move = MOVE_ZERO;
		g_aEnemy[nCntEnemy].Type = {};
		g_aEnemy[nCntEnemy].EneSkill = false;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nCountBullet = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		// 配列の変数
		g_EnemySetPos[nCntEnemy] = VECTORO_ZERO;
		g_fEnemyAlpha[nCntEnemy] = COLOR_ALPHA;
		// 黒い敵
		g_Gus[nCntEnemy].AddPosRand = 0;								// 基準からプラスされる位置
		g_Gus[nCntEnemy].ObjctNunber = 0;								// オブジェクトのナンバー
		g_Gus[nCntEnemy].RandApper = 0;									// 出現カウンター
		g_Gus[nCntEnemy].StandardPos = 0;								// 基準位置
		g_Gus[nCntEnemy].GusType = NULL;								// 黒い敵のタイプ
		g_Gus[nCntEnemy].GusCol = COLOR_ZERO;							// 黒い敵の色
		// 花の敵
		g_Flawar[nCntEnemy].LifeDeath = false;
		g_Flawar[nCntEnemy].RandApper = 0;
		g_Flawar[nCntEnemy].BulletTime = MAX_COLOR;
		g_Flawar[nCntEnemy].FlawarNumber = ENEMY_TYPE_FLAWAR;
	}
	// 配列なしの変数の初期化
	g_fAngleEnemy = 0.0f;
	g_nCounterEnemy = 0;
	g_anApperCounter = 0;
	// 火の敵
	g_Fire.AddPosRand = 0;												// 基準からプラスされる位置
	g_Fire.RandApper = 0;												// 出現カウンター
	g_Fire.StandardPos = 0;												// 基準位置
	// 戦車の敵
	g_Tank.AddPosRand = 0;												// 基準からプラスされる位置
	g_Tank.RandApper = 0;												// 出現カウンター
	g_Tank.StandardPos = 0;												// 基準位置

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * PVTEX_MAX * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	//敵の初期化設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//各頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE , 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE , 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE , 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE , 0.0f);

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

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEXPOS_ZERO, TEXPOS_ONE);
		pVtx[1].tex = D3DXVECTOR2(TEXPOS_ZERO, TEXPOS_ZERO);
		pVtx[2].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ONE);
		pVtx[3].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ZERO);

		//プラス4進める
		pVtx += PVTEX_MAX;
	}
	//頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();
	return S_OK;
}

//=====================================================================================================================================================================//
// 終了処理
//=====================================================================================================================================================================//
void UninitEnemy(void)
{
	if (g_pVtxBuffEnemy != NULL)
	{//頂点バッファの開放
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENYMY_TYPE; nCntEnemy++)
	{
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{//テクスチャの開放
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
}

//=====================================================================================================================================================================//
// 更新処理
//=====================================================================================================================================================================//
void UpdateEnemy(void)
{
	//構造体のポインタ
	ENEMY *pEnemy;
	VERTEX_2D *pVtx;
	OBJCT *pObj;

	//オブジェクトの情報取得
	pObj = GetObjct();

	//頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が存在しているとき
			//敵の先頭アドレス
			pEnemy = &g_aEnemy[0];

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE , 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE , 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE , 0.0f);

			/*敵の更新*/
			if (g_aEnemy[nCntEnemy].Type == ENEMY_TYPE_GUS)
			{//黒い敵
				EnemyGus(nCntEnemy);
			}
			if (g_aEnemy[nCntEnemy].Type == ENEMY_TYPE_FIRE)
			{//火の敵
				EnemyFire(nCntEnemy);
			}
			if (g_aEnemy[nCntEnemy].Type == ENEMY_TYPE_TANK)
			{//戦車の敵
				EnemyTank(nCntEnemy);
			}
			if (g_aEnemy[nCntEnemy].Type == ENEMY_TYPE_FLAWAR)
			{//花の敵
				EnemyFlawar(nCntEnemy);
			}
			//敵の状態管理
			StateEnemy(nCntEnemy);
		}
		//使用していない敵の番号
		g_nCounterEnemy = nCntEnemy;

		//プラス4進める
		pVtx += PVTEX_MAX;
	}
	//敵の配置
	RandSetEnemy(g_nCounterEnemy);
	SetEnemyGus(g_nCounterEnemy);
	//敵出現タイム
	g_anApperCounter += 1;
	//頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================================================================================================================//
// 描画処理
//=====================================================================================================================================================================//
void DrawEnemy(void)
{
	//ポインタ
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;

	//デバイスの取得
	pDevice = GetDevice();
	pEnemy = &g_aEnemy[0];

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->Type]);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * PVTEX_MAX, 2);
		}
	}
}

//=====================================================================================================================================================================//
//敵の設置するときの設定											                                                                                                   //
//=====================================================================================================================================================================//
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move,ENEMY_TYPE Type,int nLife)
{
	// 頂点ポインタ
	VERTEX_2D *pVtx;
	// 頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{// 使用していない敵
		 // 敵の位置と種類の情報
			g_aEnemy[nCntEnemy].pos = pos;				// 位置
			g_aEnemy[nCntEnemy].Type = Type;			// 種類
			g_aEnemy[nCntEnemy].nLife = nLife;			// 体力
			g_aEnemy[nCntEnemy].move = move;			// 移動
			if (g_aEnemy[nCntEnemy].Type == ENEMY_TYPE_FLAWAR)
			{// 花の最初の状態
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_FALSE;
			}

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE , 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE , 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZE , 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZE , g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZE , 0.0f);

			if (g_aEnemy[nCntEnemy].Type != ENEMY_TYPE_GUS)
			{
				// 各頂点のカラー情報
				pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
				pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
				pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
				pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			}

			//敵が画面内に存在している
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
		//頂点を４進める
		pVtx += PVTEX_MAX;
	}
	//頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================================================================================================================//
//敵のランダム配置																																					   //
//=====================================================================================================================================================================//
void RandSetEnemy(int nIdx)
{
	//-----------------------//
	/*		  火の敵		 */
	//-----------------------//
	g_Fire.AddPosRand = rand() % 360 + 300;

	if (g_Fire.RandApper == FIRE_APPER)
	{
		SetEnemy(D3DXVECTOR3(-10, SCREEN_HEIGHT - (float)g_Fire.AddPosRand, 0), D3DXVECTOR3(2.0f, 0.0f, 0.0f), ENEMY_TYPE_FIRE, 10);
		g_Fire.RandApper = 0;
	}

	//-----------------------//
	/*		  機械の敵　	 */
	//-----------------------//
	g_Tank.AddPosRand = rand() % 360 + 300;

	if (g_Tank.RandApper == TANK_APPER)
	{
		SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 10, SCREEN_HEIGHT - (float)g_Tank.AddPosRand, 0), D3DXVECTOR3(2.0f, 0.0f, 0.0f), ENEMY_TYPE_TANK, 10);
		g_Tank.RandApper = 0;
	}

	//-----------------------//
	/*		  花の敵　　　　 */
	//-----------------------//
	// 四体固定 //
	if (g_anApperCounter == 1)
	{
		SetEnemy(D3DXVECTOR3(FLAWAR_P, FLAWAR_O, 0),MOVE_ZERO, ENEMY_TYPE_FLAWAR, FLAWAR_LIFE);
	}
	if (g_anApperCounter == 2)
	{
		SetEnemy(D3DXVECTOR3(FLAWAR_O, FLAWAR_O, 0), MOVE_ZERO, ENEMY_TYPE_FLAWAR, FLAWAR_LIFE);
	}
	if (g_anApperCounter == 3)
	{
		SetEnemy(D3DXVECTOR3(FLAWAR_O, FLAWAR_S, 0), MOVE_ZERO, ENEMY_TYPE_FLAWAR, FLAWAR_LIFE);
	}
	if (g_anApperCounter == 4)
	{
		SetEnemy(D3DXVECTOR3(FLAWAR_P, FLAWAR_S, 0), MOVE_ZERO, ENEMY_TYPE_FLAWAR, FLAWAR_LIFE);
	}
}

//=====================================================================================================================================================================//
// 黒い敵のランダム配置　　                                                                                                                                                   
//=====================================================================================================================================================================//
void SetEnemyGus(int nIdx)
{
	// 各構造体のポインタ
	OBJCT *pObj;
	ENEOBJCT *pEneObj;
	// 情報取得
	pEneObj = GetEnObjct();
	pObj = GetObjct();

	//-----------------------//
	/*		  黒い敵		 */
	//-----------------------//
	// 出現カウンター変数
	g_Gus[nIdx].AddPosRand = rand() % 250 + 50;
	g_Gus[nIdx].ObjctNunber = rand() % 5;

		pEneObj += g_Gus[nIdx].ObjctNunber;	//オブジェクトナンバー
		if ((g_anApperCounter % g_Gus[nIdx].AddPosRand) == 0 && pEneObj->EneOState == ENEMY_OBJ_NORMAL)
		{// オブジェクト
			g_EnemySetPos[nIdx] = pEneObj->objpos;																													// 座標
			g_fAngleEnemy = CASTLE_ANGLE;																															// 城との角度
			SetEnemy(g_EnemySetPos[nIdx], D3DXVECTOR3(sinf(D3DX_PI + g_fAngleEnemy) * 2, cosf(D3DX_PI - g_fAngleEnemy) * 2, 0.0f), ENEMY_TYPE_GUS, ENEMY_GUS_LIFE);	// 敵の配置
		}
	//ランダム更新
	g_Gus[nIdx].AddPosRand = rand() % 250 + 50;
	g_Gus[nIdx].RandApper = rand() % 300 + 100;
	if ((g_anApperCounter % g_Gus[nIdx].RandApper) == 0)
	{// 右からの敵
	 // 座標
		g_EnemySetPos[nIdx] = GUS_POS_RIGHT;

		// 城との角度
		g_fAngleEnemy = atan2f(g_EnemySetPos[nIdx].x - (float)pObj->objpos.x, g_EnemySetPos[nIdx].y - (float)pObj->objpos.y);

		// 敵の配置
		SetEnemy(g_EnemySetPos[nIdx], D3DXVECTOR3(sinf(D3DX_PI + g_fAngleEnemy) * 2, cosf(D3DX_PI - g_fAngleEnemy) * 2, 0.0f), ENEMY_TYPE_GUS, ENEMY_GUS_LIFE);
	}
	// 出現カウンター変数
	g_Gus[nIdx].AddPosRand = rand() % 250 + 50;
	g_Gus[nIdx].RandApper = rand() % 300 + 100;
	if ((g_anApperCounter % g_Gus[nIdx].RandApper) == 0)
	{// 左からの敵
	 // 座標
		g_EnemySetPos[nIdx] = GUS_POS_LEFT;

		// 城との角度
		g_fAngleEnemy = atan2f(g_EnemySetPos[nIdx].x - (float)pObj->objpos.x, g_EnemySetPos[nIdx].y - (float)pObj->objpos.y);

		// 敵の配置
		SetEnemy(g_EnemySetPos[nIdx], D3DXVECTOR3(sinf(D3DX_PI + g_fAngleEnemy) * 2, cosf(D3DX_PI - g_fAngleEnemy) * 2, 0.0f), ENEMY_TYPE_GUS, ENEMY_GUS_LIFE);
	}
}

//=====================================================================================================================================================================//
// 敵の状態管理　　                                                                                                                                                   
//=====================================================================================================================================================================//
void StateEnemy(int nIdx)
{
	// ポインタ
	VERTEX_2D *pVtx;

	// 変数の初期化
	int nScore = 0;		

	// 頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	if (g_aEnemy[nIdx].state == ENEMYSTATE_FALSE && g_aEnemy[nIdx].Type != ENEMY_TYPE_FLAWAR)
	{// 敵が消滅するとき

		// その敵の頂点番号まで進める
		pVtx += (PVTEX_MAX * nIdx);

		// 移動停止
		g_aEnemy[nIdx].move = MOVE_ZERO;

		// テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ONE);
		pVtx[1].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ZERO);
		pVtx[2].tex = D3DXVECTOR2(TEXPOS_ONE, TEXPOS_ONE);
		pVtx[3].tex = D3DXVECTOR2(TEXPOS_ONE, TEXPOS_ZERO);

		// 各頂点のカラー情報
		pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_fEnemyAlpha[nIdx]);
		pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_fEnemyAlpha[nIdx]);
		pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_fEnemyAlpha[nIdx]);
		pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_fEnemyAlpha[nIdx]);

		// アルファ値を減らす
		g_fEnemyAlpha[nIdx] -= ALPHA_INTERVAL;

		if (g_fEnemyAlpha[nIdx] <= 0)
		{// アルファ値がゼロ以下のとき
			// スコア
			nScore += ADD_SCORE;
			// スコア加算
			AddScore(nScore);

			// テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(TEXPOS_ZERO, TEXPOS_ONE);
			pVtx[1].tex = D3DXVECTOR2(TEXPOS_ZERO, TEXPOS_ZERO);
			pVtx[2].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ONE);
			pVtx[3].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ZERO);

			// 各頂点のカラー情報
			pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

			// アルファ値を戻す
			g_fEnemyAlpha[nIdx] = COLOR_ALPHA;

			// 敵を消す
			g_aEnemy[nIdx].bUse = false;

			// 敵の状態をノーマルに
			g_aEnemy[nIdx].state = ENEMYSTATE_NORMAL;
		}
	}
	else if (g_aEnemy[nIdx].state == ENEMYSTATE_DAMAGE)
	{//ダメージ状態
		// プラス4進める
		pVtx += (PVTEX_MAX * nIdx);

		// カウンターをマイナス
		g_aEnemy[nIdx].nCounterState--;

		if (g_aEnemy[nIdx].nCounterState <= 0)
		{// カウンターがゼロの場合

			// ノーマル状態
			g_aEnemy[nIdx].state = ENEMYSTATE_NORMAL;

			// 色の更新
			pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
			pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		}
	}
	// 頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================================================================================================================//
// 黒い敵　　                                                                                                                                                   
//=====================================================================================================================================================================//
void EnemyGus(int nIdx)
{
	// ポインタ
	VERTEX_2D *pVtx;

	// オブジェクトの情報取得
	OBJCT *pObj;
	pObj = GetObjct();

	g_anFlame[nIdx]++;

	// 頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 移動
	g_aEnemy[nIdx].pos.x += g_aEnemy[nIdx].move.x;	// X方向
	g_aEnemy[nIdx].pos.y += g_aEnemy[nIdx].move.y;	// Y方向
	if ((g_anFlame[nIdx] % FRAME_MATH) == 0)
	{// 一秒に一回
		g_Gus[nIdx].GusType = rand() % 4;		// 黒い敵の種類
	}
	if (g_Gus[nIdx].GusType == 3 && g_aEnemy[nIdx].Type == ENEMY_TYPE_GUS)
	{
		// その敵の頂点番号まで進める
		pVtx += (PVTEX_MAX * nIdx);
		//ノーマル色
		g_Gus[nIdx].GusCol = GUS_NORMAL;
	}
	else if (g_Gus[nIdx].GusType == 0 && g_aEnemy[nIdx].Type == ENEMY_TYPE_GUS)
	{
		// その敵の頂点番号まで進める
		pVtx += (PVTEX_MAX * nIdx);
		//花タイプ
		g_Gus[nIdx].GusCol = GUS_FLAWAR;
	}
	else if (g_Gus[nIdx].GusType == 1 && g_aEnemy[nIdx].Type == ENEMY_TYPE_GUS)
	{
		// その敵の頂点番号まで進める
		pVtx += (PVTEX_MAX * nIdx);
		//火タイプ
		g_Gus[nIdx].GusCol = GUS_FIRE;
	}
	else if (g_Gus[nIdx].GusType == 2 && g_aEnemy[nIdx].Type == ENEMY_TYPE_GUS)
	{
		// その敵の頂点番号まで進める
		pVtx += (PVTEX_MAX * nIdx);
		//戦車タイプ
		g_Gus[nIdx].GusCol = GUS_TANK;
	}
	// 各頂点のカラー情報
	pVtx[0].col = g_Gus[nIdx].GusCol;
	pVtx[1].col = g_Gus[nIdx].GusCol;
	pVtx[2].col = g_Gus[nIdx].GusCol;
	pVtx[3].col = g_Gus[nIdx].GusCol;
	// 城との当たり判定
	if (g_aEnemy[nIdx].pos.x >= pObj->objpos.x - CASTLE_POS &&
		g_aEnemy[nIdx].pos.y >= pObj->objpos.y - CASTLE_POS &&
		g_aEnemy[nIdx].pos.x <= pObj->objpos.x + CASTLE_POS &&
		g_aEnemy[nIdx].pos.y <= pObj->objpos.y + CASTLE_POS)
	{
		// ダメージ量
		float nDamage = GUS_DAMAGE;

		// 城が被弾した場合
		HitObjct(nDamage);

		// ダメージ音
		PlaySound(SOUND_LABEL_SE_HIT);

		if (g_Gus[nIdx].GusType == 1)
		{//火の出現カウンター
			g_Fire.RandApper += 1;
		}
		if (g_Gus[nIdx].GusType == 2)
		{//戦車の出現カウンター
			g_Tank.RandApper += 1;
		}
		// 敵を使っていない状態にする
		g_aEnemy[nIdx].bUse = false;
	}
	// 頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================================================================================================================//
// 火の敵　　                                                                                                                                                   //
//=====================================================================================================================================================================//
void EnemyFire(int nIdx)
{
	g_aEnemy[nIdx].pos.x += g_aEnemy[nIdx].move.x;
	int nBullet = rand() % FLAWAR_BULLET + FLAWAR_BULLET;

	if (g_aEnemy[nIdx].pos.x >= 100)
	{
		g_aEnemy[nIdx].move = MOVE_ZERO;
	}
	// 敵の位置情報
	D3DXVECTOR3 pos = g_aEnemy[nIdx].pos;
	// 敵の弾の発射のタイミング
	g_aEnemy[nIdx].nCountBullet += 1;

	if (g_aEnemy[nIdx].state == ENEMYSTATE_NORMAL && (g_aEnemy[nIdx].nCountBullet % nBullet) == 0)
	{// 火の敵
		D3DXVECTOR3 move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);	//弾の移動
		SetBullet(pos, move, 600, BULLET_TYPE_ENEMY);		//弾の発射
	}

}

//=====================================================================================================================================================================//
// 戦車の敵　　                                                                                                                                                   //
//=====================================================================================================================================================================//
void EnemyTank(int nIdx)
{
	// 移動
	g_aEnemy[nIdx].pos.x -= g_aEnemy[nIdx].move.x;
	int nBullet = rand() % FLAWAR_BULLET + FLAWAR_BULLET;


	if (g_aEnemy[nIdx].pos.x <= SCREEN_WIDTH - 60)
	{// ストップ
		g_aEnemy[nIdx].move = MOVE_ZERO;
	}
	// 敵の位置
	D3DXVECTOR3 pos = g_aEnemy[nIdx].pos;
	// 敵の弾の発射のタイミング
	g_aEnemy[nIdx].nCountBullet += 1;

	if (g_aEnemy[nIdx].state == ENEMYSTATE_NORMAL && (g_aEnemy[nIdx].nCountBullet % nBullet) == 0)
	{// 弾の発射
		D3DXVECTOR3 move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);	//弾の移動
		SetBullet(pos, move, 600, BULLET_TYPE_ENEMY);		//弾の発射
	}
}

//=====================================================================================================================================================================//
// 花の敵　　                                                                                                                                                   
//=====================================================================================================================================================================//
void EnemyFlawar(int nIdx)
{
	// ポインタ
	VERTEX_2D *pVtx;
	// 頂点バッファのロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 敵の位置情報
	D3DXVECTOR3 pos = g_aEnemy[nIdx].pos;

	if (g_aEnemy[nIdx].state == ENEMYSTATE_NORMAL)
	{// 戦闘態勢

		 // 敵の弾の発射のタイミング
		g_aEnemy[nIdx].nCountBullet += 1;
		g_Flawar[nIdx].BulletTime = rand() % FLAWAR_BULLET + FLAWAR_BULLET;

		// プラス4進める
		pVtx += (PVTEX_MAX * nIdx);

		// テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ONE);
		pVtx[1].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ZERO);
		pVtx[2].tex = D3DXVECTOR2(TEXPOS_ONE, TEXPOS_ONE);
		pVtx[3].tex = D3DXVECTOR2(TEXPOS_ONE, TEXPOS_ZERO);

		// 色の更新
		pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
		pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

		// 弾の発射
		if ((g_aEnemy[nIdx].nCountBullet % g_Flawar[nIdx].BulletTime) == 0)
		{// 特定のフレームに来たら
			D3DXVECTOR3 move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);			// 弾の移動
			SetBullet(pos, move, 2000, BULLET_TYPE_ENEMY);		// 弾の発射
		}
	}
	else if (g_aEnemy[nIdx].state == ENEMYSTATE_FALSE)
	{// 戦闘不能
		// 出現カウンターを進める
		g_Flawar[nIdx].RandApper++;
		if ((g_Flawar[nIdx].RandApper % 600) == 0)
		{// 戦闘モード
			g_aEnemy[nIdx].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nIdx].nLife = 4;
		}
		// プラス4進める
		pVtx += (PVTEX_MAX * nIdx);

		// テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(TEXPOS_ZERO, TEXPOS_ONE);
		pVtx[1].tex = D3DXVECTOR2(TEXPOS_ZERO, TEXPOS_ZERO);
		pVtx[2].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ONE);
		pVtx[3].tex = D3DXVECTOR2(TEXPOS_HALF, TEXPOS_ZERO);
	}
	// 頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================================================================================================================//
// 敵が弾の当たった時                                                                                                                                                   //
//=====================================================================================================================================================================//
bool HitEnemy(int nIdx, int nDamage)
{
	// ポインタ
	VERTEX_2D *pVtx;

	// 敵の体力減少
	g_aEnemy[nIdx].nLife -= nDamage;

	if (g_aEnemy[nIdx].nLife <= 0)
	{// 体力がゼロのとき
	 // 敵の消滅
		if (g_aEnemy[nIdx].Type == ENEMY_TYPE_FIRE)
		{
			g_Fire.RandApper += 1;

		}
		if (g_aEnemy[nIdx].Type == ENEMY_TYPE_FIRE)
		{
			g_Tank.RandApper += 1;

		}

		g_aEnemy[nIdx].state = ENEMYSTATE_FALSE;
		return true;
	}
	else
	{// ゼロじゃないとき
	 // ダメージ音
		PlaySound(SOUND_LABEL_SE_HIT);

		// ダメージ状態
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;

		// 状態のカウンター
		g_aEnemy[nIdx].nCounterState = ALPHA_INTERVAL;

		// 頂点バッファのロック
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		// 敵の情報分進める
		pVtx += (PVTEX_MAX * nIdx);

		// 色の更新
		pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_DAMAGEALPHA);
		pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_DAMAGEALPHA);
		pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_DAMAGEALPHA);
		pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_DAMAGEALPHA);

		// 頂点バッファのアンロック
		g_pVtxBuffEnemy->Unlock();
		return false;
	}
}

//=====================================================================================================================================================================//
// 敵の情報　　　　　                                                                                                                                                   //
//=====================================================================================================================================================================//
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================================================================================================================//
// 敵の情報　　　　　                                                                                                                                                   //
//=====================================================================================================================================================================//
ENEMY_FLAWAR *GetFlawar(void)
{
	return &g_Flawar[0];
}

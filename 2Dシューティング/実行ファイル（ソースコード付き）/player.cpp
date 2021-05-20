//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//　インクルードファイル
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "player.h"
#include "objct.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "sound.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//　マクロ定義
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#define PLAYER_ONE			(PLAYER_TYPE_GREEN)					// プレイヤー１
#define PLAYER_TWO			(PLAYER_TYPE_RED)					// プレイヤー２
#define MOVE_SPEED			(5)									// スピード
#define LENGTH_PLAYER_X		(40)								// 中心から各頂点の距離を求める数値
#define LENGTH_PLAYER_Y		(40)								// 中心から各頂点の距離を求める数値
#define COLOR_RED			(255)								// 赤色
#define COLOR_BULUE			(255)								// 青色
#define COLOR_GREEN			(255)								// 緑色
#define COLOR_ALPHA			(255)								// アルファ値
#define PLAYER_POS_FS_X		(490)								// pVtxの１と２のXの数値
#define PLAYER_POS_TF_X		(570)								// pVtxの３と４のXの数値
#define PLAYER_POS_SF_Y		(190)								// pVtxの２と４のYの数値
#define PLAYER_POS_FT_Y		(270)								// pVtxの１と３のYの数値
#define PLAYER_TYPE_TEX		(2)									// プレイヤーの種類
#define PVTEX_MAX			(4)									// pVtx
#define CENTER_POS_X		(650)								// 中心座標（X) 
#define CENTER_POS_Y		(500)								// 中心座標（Y)
#define HALF_MATH			(2)									// 半分
#define FRAME_COUNT			(10)								// FRAMEのカウント
#define NORMAL_PLAYER		(240)								// インターバル
#define BULLET_LIFE			(100)								// 弾のライフ
#define EFFECT_COLORO		(D3DXCOLOR(0.8f, 0.2f, 0.1f, 1.0f))	// エフェクトの色
#define PLAYER_LIFE			(5)									// プレイヤーの体力
#define RHW					(10/10)								// rhwの数値1.0固定
#define G_POS_X				(40)								// 中心からのｘの距離
#define G_POS_Y				(50)								// 中心からのｙの距離
#define WATER_SIZE			(300)								// 水源エリアの一辺
#define SCREEN_ALPHA		(100)								// 背景の緑の部分

//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//グローバル変数
//------------------------------------------------------------------------------------------------------------------------------------------------------------------//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;			// 頂点バッファの情報
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYER_TYPE_TEX] = {};	// テクスチャの情報
D3DXVECTOR3 g_posPlayer;									// 位置情報
D3DXVECTOR3 g_rotPlayer;									// 回転情報
D3DXVECTOR3 g_movePlayer;									// 加減速情報
PLAYER g_Player;											// プレイヤー情報
BULLET_TYPE g_Btype;										// 弾の情報
int g_nConterAnim;											// アニメーションのカウンター
int g_nPatternAnim;											// アニメーション
int g_nAlpha;												// α値
float g_fLengthPlayer;										// 対角線の長さ
float g_fAnglePlayer;										// 対角線の角度
float g_fTexY;												// テクスチャy座標

//=====================================================================================================================================================================//
// プレイヤーの初期化処理
//=====================================================================================================================================================================//
HRESULT InitPlayer(MODE mode)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	VERTEX_2D *pVtx;			// 頂点ポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// グローバル変数の初期化
	g_nConterAnim = 0;
	g_nPatternAnim = 0;
	g_fLengthPlayer = 0.0f;
	g_fAnglePlayer = 0.0f;
	g_fTexY = 0.0f;
	g_nAlpha = COLOR_ALPHA;

	// プレイヤー情報の初期化
	g_Player.nCounterState = 0;
	g_Player.nCounterDisp = 0;
	g_Player.nLife = PLAYER_LIFE;
	g_Player.player = PLAYER_ONE;
	g_Player.Playerpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.PlayerState = PLAYERSTATE_NORMAL;
	g_Player.bSkills = false;
	g_Player.g_mode = mode;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2Dchara2.png", &g_pTexturePlayer[PLAYER_ONE]);	// プレイヤー１
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\2Dchara1.png", &g_pTexturePlayer[PLAYER_TWO]);	// プレイヤー２

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * PVTEX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロック
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 対角線の長さ
	g_fLengthPlayer = sqrtf(LENGTH_PLAYER_X * LENGTH_PLAYER_X + LENGTH_PLAYER_Y * LENGTH_PLAYER_Y);

	// 対角線の角度
	g_fAnglePlayer = atan2f(LENGTH_PLAYER_X, LENGTH_PLAYER_Y);

	// 初期の場所に戻す
	g_posPlayer = D3DXVECTOR3(CENTER_POS_X, CENTER_POS_Y, 0.0f);

	// 各頂点の座標
	pVtx[0].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// 中心座標
	g_posPlayer = D3DXVECTOR3(CENTER_POS_X, CENTER_POS_Y, 0.0f);

	// 回転角度
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = RHW;
	pVtx[1].rhw = RHW;
	pVtx[2].rhw = RHW;
	pVtx[3].rhw = RHW;

	// 頂点カラーの設定・赤・緑・青・アルファ
	pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);
	pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, COLOR_ALPHA);

	// テクスチャの頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0, 0);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	// 初期化完了
	return S_OK;
}

//=====================================================================================================================================================================//
// ポリゴンの終了処理
//=====================================================================================================================================================================//
void UninitPlayer(void)
{
	// テクスチャの開放
	for (int nCuntPlayer = 0; nCuntPlayer < PLAYER_TYPE_TEX; nCuntPlayer++)
	{
		if (g_pTexturePlayer[g_Player.player] != NULL)
		{
			g_pTexturePlayer[g_Player.player]->Release();
			g_pTexturePlayer[g_Player.player] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
	StopSound(SOUND_LABEL_SE_SHOT);	//BGM

}

//=====================================================================================================================================================================//
// ポリゴンの更新処理
//=====================================================================================================================================================================//
void UpdatePlayer(void)
{
	// ポインタ
	VERTEX_2D *pVtx;	// 頂点のポインタ 
	PLAYER *pPlayer;	// プレイヤーのポインタ
	ENEMY *pEnemy;		// 敵のポインタ

	// SETBULLET用の変数
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動
	int nLife;			// 寿命

	// 情報取得
	pEnemy = GetEnemy();// 敵
	pPlayer = &g_Player;// プレイヤー

	// バッファのロック
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
//----------------------//
//	プレイヤーの移動	//
//----------------------//
	if (GetKeyboardPresse(DIK_D) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 右
		g_movePlayer.x += sinf(g_fAnglePlayer) * MOVE_SPEED;				// X方向
		g_movePlayer.z = 0.0f;												// 角度
	// 斜め移動
		if (GetKeyboardPresse(DIK_W) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
		{// 右上
			g_movePlayer.y += cosf(D3DX_PI + g_fAnglePlayer) * MOVE_SPEED;	// Y方向
		}
		else if (GetKeyboardPresse(DIK_S) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
		{// 右下
			g_movePlayer.y -= cosf(D3DX_PI + g_fAnglePlayer) * MOVE_SPEED;	// Y方向
		}
	}
	else if (GetKeyboardPresse(DIK_A) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 左
		g_movePlayer.x -= sinf(g_fAnglePlayer) * MOVE_SPEED;				// X方向
		g_movePlayer.z = 0.0f;												// 角度
	// 斜め移動 
		if (GetKeyboardPresse(DIK_W) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
		{// 左上
			g_movePlayer.y += cosf(D3DX_PI + g_fAnglePlayer) * MOVE_SPEED;	// Y方向
		}
		else if (GetKeyboardPresse(DIK_S) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
		{// 左下
			g_movePlayer.y -= cosf(D3DX_PI + g_fAnglePlayer) * MOVE_SPEED;	// Y方向
		}
	}
	else if (GetKeyboardPresse(DIK_W) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 上
			g_movePlayer.y -= cosf(g_fAnglePlayer) * MOVE_SPEED;			// Y方向
			g_movePlayer.z = 0.0f;											// 角度
	}
	else if (GetKeyboardPresse(DIK_S) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 下
			g_movePlayer.y += cosf(g_fAnglePlayer) * MOVE_SPEED;			// Y方向
			g_movePlayer.z = 0.0f;											 // 角度
	}
//----------------------//
//		 弾の発射		//
//----------------------//
	if (GetKeyboardTrigger(DIK_UP) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 上に発射
	 // 弾の情報
		pos = g_posPlayer;								// プレイヤーの位置			
		move.x = 0;										// X方向の移動量
		move.y = -(cosf(g_fAnglePlayer) + MOVE_SPEED);	// Y方向の移動量
		nLife = BULLET_LIFE;							// 寿命

		// 弾の設定
		SetBullet(pos, move, nLife, g_Btype);			
		PlaySound(SOUND_LABEL_SE_SHOT);
	}
	else if (GetKeyboardTrigger(DIK_DOWN) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 下に発射
	 // 弾の情報
		pos = g_posPlayer;								// プレイヤーの位置
		move.x = 0;										// X方向の移動量
		move.y = (cosf(g_fAnglePlayer) + MOVE_SPEED);	// Y方向の移動量
		nLife = BULLET_LIFE;							// 寿命

		// 弾の設定
		SetBullet(pos, move, nLife, g_Btype);	
		PlaySound(SOUND_LABEL_SE_SHOT);
	}
	else if (GetKeyboardTrigger(DIK_RIGHT) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 右に発射
	 // 弾の情報
		pos = g_posPlayer;								// プレイヤーの位置
		move.x = (sinf(g_fAnglePlayer) + MOVE_SPEED);	// X方向の移動量
		move.y = 0;										// Y方向の移動量
		nLife = BULLET_LIFE;							// 寿命

		// 弾の設定
		SetBullet(pos, move, nLife, g_Btype);	
		PlaySound(SOUND_LABEL_SE_SHOT);
	}
	else if (GetKeyboardTrigger(DIK_LEFT) == true && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// 左に発射
	 // 弾の情報
		pos = g_posPlayer;								// プレイヤーの位置
		move.x = -(sinf(g_fAnglePlayer) + MOVE_SPEED);	// X方向の移動量
		move.y = 0;										// Y方向の移動量
		nLife = BULLET_LIFE;							// 寿命

		// 弾の設定
		SetBullet(pos, move, nLife, g_Btype);
		PlaySound(SOUND_LABEL_SE_SHOT);
	}
//----------------------//
//		 特殊能力		//
//----------------------//
	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.player == PLAYER_TYPE_GREEN && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// ON か OFF
		g_Player.bSkills = g_Player.bSkills ? false : true;
	}
	else if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.player == PLAYER_TYPE_RED && g_Player.PlayerState == PLAYERSTATE_NORMAL)
	{// ON か OFF
		g_Player.bSkills = g_Player.bSkills ? false : true;
	}
	// ON のとき
	if (g_Player.bSkills == true)
	{// 特殊能力　Xで使用
		if (GetKeyboardTrigger(DIK_Q) == true)
		{// エフェクトの発生（仮）

		}
		else if (GetKeyboardTrigger(DIK_E) == true)
		{// エフェクトの発生（仮）

		}
	}
//----------------------//
//	  プレイヤー変更	//
//----------------------//
	if (GetKeyboardTrigger(DIK_Q) == true)
	{// Q　キャラ変更
		if (g_Player.player == PLAYER_TYPE_RED)
		{
			g_Player.player = PLAYER_TYPE_GREEN;
			g_Player.bSkills = false;	//特殊能力をOFF
		}
		else if (g_Player.player == PLAYER_TYPE_GREEN)
		{
			g_Player.player = PLAYER_TYPE_RED;
			g_Player.bSkills = false;	//特殊能力をOFF
		}
	}
	// 位置の更新
	g_posPlayer.x += g_movePlayer.x;
	g_posPlayer.y += g_movePlayer.y;
	g_rotPlayer.z += g_movePlayer.z;
//-----------------------//
// チュートリアルモード
//-----------------------//
	if (g_Player.g_mode == MODE_TUTORIAL)
	{
		// 横の範囲
		if (g_posPlayer.x + G_POS_X > SCREEN_WIDTH || g_posPlayer.x - G_POS_X < 0)
		{// 移動不可
			g_posPlayer.x -= g_movePlayer.x;
		}
		// 縦の範囲
		if (g_posPlayer.y + G_POS_Y > SCREEN_HEIGHT - 200 || g_posPlayer.y - G_POS_Y < SCREEN_ALPHA)
		{// 移動不可
			g_posPlayer.y -= g_movePlayer.y;
		}
	}
//-----------------------//
// ゲームモード
//-----------------------//
	if (g_Player.g_mode == MODE_GAME)
	{
		// 横の範囲
		if (g_posPlayer.x + G_POS_X > SCREEN_WIDTH || g_posPlayer.x - G_POS_X < 0)
		{// 移動不可
			g_posPlayer.x -= g_movePlayer.x;
		}
		// 縦の範囲
		if (g_posPlayer.y + G_POS_Y > SCREEN_HEIGHT - SCREEN_ALPHA || g_posPlayer.y - G_POS_Y < SCREEN_ALPHA)
		{// 移動不可
			g_posPlayer.y -= g_movePlayer.y;
		}
		//水源エリア
		// 左側
		if (g_posPlayer.x - G_POS_X < WATER_SIZE && g_posPlayer.y - G_POS_Y < WATER_SIZE || g_posPlayer.x - G_POS_X < WATER_SIZE && g_posPlayer.y + G_POS_Y > SCREEN_HEIGHT - WATER_SIZE)
		{// 移動不可

			g_posPlayer.x -= g_movePlayer.x;
			g_posPlayer.y -= g_movePlayer.y;
		}
		// 右側
		if (g_posPlayer.x + G_POS_X > SCREEN_WIDTH - WATER_SIZE && g_posPlayer.y - G_POS_Y < WATER_SIZE || g_posPlayer.x + G_POS_X > SCREEN_WIDTH - WATER_SIZE && g_posPlayer.y + G_POS_Y > SCREEN_HEIGHT - WATER_SIZE)
		{// 移動不可
			g_posPlayer.x -= g_movePlayer.x;
			g_posPlayer.y -= g_movePlayer.y;
		}
	}
//-----------------------//
// 頂点情報
//-----------------------//
	// 移動・回転量の減衰
	g_movePlayer.x += (0 - g_movePlayer.x)*1.0f;
	g_movePlayer.y += (0 - g_movePlayer.y)*1.0f;
	g_movePlayer.z += (0 - g_movePlayer.z)*1.0f;

	// アニメテクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(1.0f*(float)g_nPatternAnim, g_fTexY + 1.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f*(float)g_nPatternAnim, g_fTexY + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f*((float)g_nPatternAnim + 1), g_fTexY + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f*((float)g_nPatternAnim + 1), g_fTexY + 0.0f);

	// 各頂点の座標
	pVtx[0].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z - (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z - (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_posPlayer.x + sinf(g_rotPlayer.z + (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_posPlayer.y + cosf(g_rotPlayer.z + (g_fAnglePlayer * 3)) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// 頂点カラーの設定・赤・緑・青・アルファ（透明度）
	pVtx[0].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_nAlpha);
	pVtx[1].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_nAlpha);
	pVtx[2].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_nAlpha);
	pVtx[3].col = D3DCOLOR_RGBA(COLOR_RED, COLOR_GREEN, COLOR_BULUE, g_nAlpha);

	// プレイヤーの位置情報を取得
	g_Player.Playerpos = g_posPlayer;
//-----------------------//
// プレイヤーの状態
//-----------------------//
	if (g_Player.PlayerState == PLAYERSTATE_WAIT)
	{// 出現待ち
	 // 状態のカウンターを進める
		g_Player.nCounterState ++;

		if ((g_Player.nCounterState % FRAME_COUNT) == 0)
		{// １０フレーム
			g_nAlpha = COLOR_ALPHA;	// α値を戻す
		}
		if ((g_Player.nCounterState % (FRAME_COUNT * HALF_MATH)) == 0)
		{// ２０フレーム
			g_nAlpha = COLOR_ALPHA / HALF_MATH;	// α値を半分
		}
		if ((g_Player.nCounterState % NORMAL_PLAYER) == 0)
		{// ２４０フレーム
			g_Player.PlayerState = PLAYERSTATE_NORMAL;	// プレイヤーの状態を通常
			g_nAlpha = COLOR_ALPHA;						// α値を戻す
			g_Player.nLife = PLAYER_LIFE;				// 体力を３に
			g_Player.nCounterState = 0;					// 状態のカウンターを初期化
		}
	}
	else if (g_Player.PlayerState == PLAYERSTATE_DAMAGE)
	{// ダメージの硬直
		// カウントを進める
		g_Player.nCounterDisp ++;

		// プレイヤーの透明度を半分にする
		g_nAlpha = COLOR_ALPHA / HALF_MATH;
		if ((g_Player.nCounterDisp % (FRAME_COUNT / HALF_MATH)) == 0)
		{// ５フレーム後
			g_Player.PlayerState = PLAYERSTATE_NORMAL;	// 状態をノーマルに
			g_Player.nCounterDisp = 0;					// カウントを初期化
			g_nAlpha = COLOR_ALPHA;						// α値を戻す
		}
	}
	// プレイヤーバッファのアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=====================================================================================================================================================================//
// ポリゴンの描画処理
//=====================================================================================================================================================================//
void DrawPlayer(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer[g_Player.player]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		
}

//=====================================================================================================================================================================//
// プレイヤーのダメージ
//=====================================================================================================================================================================//
bool HitPlayer(int Damage)
{
	// ダメージ
	g_Player.nLife -= Damage;

	if (g_Player.nLife == 0)
	{// LIFEがゼロになった場合

		// プレイヤー死亡
		g_Player.PlayerState = PLAYERSTATE_DEATH;
		 
		// 透明度を半分にする
		g_nAlpha /= 2;

		// 初期の場所に戻す
		g_posPlayer = D3DXVECTOR3(CENTER_POS_X, CENTER_POS_Y, 0.0f);

		// 出現状態にする
		g_Player.PlayerState = PLAYERSTATE_WAIT;

		return true;
	}
	else
	{
		// 状態をダメージ
		g_Player.PlayerState = PLAYERSTATE_DAMAGE;

		return false;
	}
}


//=====================================================================================================================================================================//
// プレイヤー情報の取得
//=====================================================================================================================================================================//
PLAYER *GetPlayer(void)
{
	return &g_Player;
}
//*****************************************************************************
//
// メイン処理 [main.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _MAIN_H_
#define _MAIN_H_
#define DIRECTINPUT_VERSION	(0x0800)

//-----------------------------------------------------------------------------
// ヘッダファイルのインクルード
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <random>
#include <d3dx9.h>	//描画処理に必要 //プロパティから、ライブラリ・インクルードファイルを編集Window(:C)からｘ８６へ、フォルダを選択する
#include <dinput.h>
#include <xaudio2.h>
#include <XInput.h>

//-----------------------------------------------------------------------------
// ライブラリファイルのリンク
//-----------------------------------------------------------------------------
#pragma comment(lib, "d3d9.lib")	//描画処理に必要
#pragma comment(lib, "d3dx9.lib")	//拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	//コンポーネント使用に必要
#pragma comment(lib, "dinput8.lib")	//入力処理に必要
#pragma comment(lib, "winmm.lib")	
#pragma comment(lib, "xinput.lib")	

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define SCREEN_WIDTH	(1366)
#define SCREEN_HEIGHT	(768)
#define WIDTH_HALF		(SCREEN_WIDTH/2)
#define HEIGHT_HALF		(SCREEN_HEIGHT/2)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//頂点フォーマット・頂点座標・頂点カラー・テクスチャ座標
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//頂点フォーマット・頂点座標・頂点カラー・テクスチャ座標
#define NUM_VERTEX		(4)															// 頂点数
#define CREATE_POLYGON	(sizeof(VERTEX_2D) * 4)

// 初期化用マクロ変数
#define ZeroVector2		(D3DXVECTOR2(0.0f, 0.0f))
#define ZeroVector3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define WhiteColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
#define BlackColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
#define CENTER_POS		(D3DXVECTOR3(WIDTH_HALF,HEIGHT_HALF,0.0f))
#define COLOR_RED		(255)								// 赤色
#define COLOR_BULUE		(255)								// 青色
#define COLOR_GREEN		(255)								// 緑色
#define COLOR_ALPHA		(255)								// アルファ値


//-----------------------------------------------------------------------------
// 構造体定義
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0で固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
int GetFps(void);

#endif
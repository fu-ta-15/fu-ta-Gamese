//*****************************************************************************
//
// Imgui処理 [imguimanager.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _IMGUIMANAGER_H_
#define _IMGUIMANAGER_H_

//-----------------------------------------------------------------------------
// Imguiファイルのインクルード
//-----------------------------------------------------------------------------
#include "main.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "mesh3D.h"
#include "TextureScene.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define MAX_CAHR_SIZE		(128)

//-----------------------------------------------------------------------------
// 名前空間・ImGuiマネージャ
//-----------------------------------------------------------------------------
namespace ImGuiMana
{
	HRESULT Init(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9 pD3DDevice);		  // 初期化
	void Uninit(void);																		  // 終了
	void Update(void);																		  // 更新
	void DeviState(void);																	  // デバイスの状態
	void Draw(void);																		  // 描画
	void DrawEnd(HRESULT result,LPDIRECT3DDEVICE9 pD3DDevice, D3DPRESENT_PARAMETERS d3dpp);	  // 描画終了
	static HWND L_hWnd = NULL;
}

//-----------------------------------------------------------------------------
// 名前空間・メッシュリスト
//-----------------------------------------------------------------------------
namespace MeshList
{
	void Update(void);			 // 更新
	void MeshInfo(void);		 // 情報
	void MeshWave(void);		 // 波の表現
	void MeshCycle(void);		 // 回転運動
	void CreateMeshMenu(void);	 // 生成メニュー

	static CMesh3D *m_pMesh = NULL;									// メッシュの情報を受け取るポインタ
	static bool m_bCreate = false;									// 生成
	static int m_nVertical = 0;										// 縦線の数
	static int m_nSide = 0;											// 横線の数
	static D3DXVECTOR3 m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	static D3DXVECTOR3 m_size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);	// 大きさ
	static bool m_bWaveStart = false;								// カウント開始と停止
	static int m_nTime = 0;											// 時間（フレームカウント)
	static float m_fHeight = 0.0f;									// 波の高さ
	static D3DXVECTOR3 m_WaveCenter = {};							// 震源
	static int m_nCycle = 100;										// 周期
	static bool m_bDemo = false;									// デモウィンドウ
	static bool m_bExplanation = false;								// 説明書
	static D3DFILLMODE m_fillmode = D3DFILL_SOLID;					// ワイヤーフレーム
	static DWORD m_d3dcullmode = D3DCULL_CCW;						// カリングモード
	static bool m_bNorSeting = false;								// 法線の計算の有無
	static bool m_bCycleStart = false;								// 回転の表現切り替え用
	static CTextureScene *m_pTextureList = NULL;					// テクスチャリストのクラス変数
};

//-----------------------------------------------------------------------------
// 名前空間・メッシュの設定
//-----------------------------------------------------------------------------
namespace MeshOptionMenu
{
	void MeshOption(void);	// 設定
}

//-----------------------------------------------------------------------------
// 名前空間・テクスチャ生成
//-----------------------------------------------------------------------------
namespace TextureMake
{
	void TextureLoad(void);						// テクスチャのロード
	void TextureBind(void);						// テクスチャの貼り付け
	void TextureBind(LPDIRECT3DTEXTURE9 pTex);	// テクスチャの貼り付け

	static bool *bTexList = {};
}

//-----------------------------------------------------------------------------
// 名前空間・詳細設定/説明
//-----------------------------------------------------------------------------
namespace Option
{
	void OperationExplanation(void);
}

//-----------------------------------------------------------------------------
// 名前空間・ライト設定
//-----------------------------------------------------------------------------
namespace LightOption
{
	void LightMove(void);
}

#endif // !_IMGUI_H_

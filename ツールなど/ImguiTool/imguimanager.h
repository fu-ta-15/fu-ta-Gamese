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

//-----------------------------------------------------------------------------
// 名前空間・ImGuiマネージャ
//-----------------------------------------------------------------------------
namespace ImGuiMana
{
	HRESULT Init(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9 pD3DDevice);

	void Uninit(void);

	void Update(void);

	void DeviState(void);

	void Draw(void);

	void DrawEnd(HRESULT result,LPDIRECT3DDEVICE9 pD3DDevice, D3DPRESENT_PARAMETERS d3dpp);
}


//-----------------------------------------------------------------------------
// 名前空間・メッシュリスト
//-----------------------------------------------------------------------------
namespace MeshList
{
	void MeshInfo(void);
	void MeshWave(void);
	void MeshCycle(void);
	void CreateMeshMenu(void);

	static CMesh3D *m_pMesh = NULL;					// メッシュの情報を受け取るポインタ
	static bool m_bCreate = false;					// 生成
	static int m_nVertical = 0;						// 縦線の数
	static int m_nSide = 0;							// 横線の数
	static D3DXVECTOR3 m_pos;						// 位置
	static D3DXVECTOR3 m_size;						// 大きさ
	static bool m_bTimeStart = false;				// カウント開始と停止
	static int m_nTime = 0;							// 時間（フレームカウント)
	static float m_fHeight = 0.0f;					// 波の高さ
	static D3DXVECTOR3 m_WaveCenter = {};			// 震源
	static int m_nCycle = 100;						// 周期
	static bool m_bDemo = false;					// デモウィンドウ
	static D3DFILLMODE m_fillmode = D3DFILL_SOLID;	// ワイヤーフレーム
	static DWORD m_d3dcullmode = D3DCULL_CCW;		// カリングモード
	static bool m_bNorSeting = false;
	static bool m_bCycleStart = false;
};

//-----------------------------------------------------------------------------
// 名前空間・メッシュの設定
//-----------------------------------------------------------------------------
namespace MeshOptionMenu
{
	void MeshOption(void);
}

//-----------------------------------------------------------------------------
// 名前空間・テクスチャ生成
//-----------------------------------------------------------------------------
namespace TextureMake
{
	void TextureLoad(void);
	void TextureBind(void);
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

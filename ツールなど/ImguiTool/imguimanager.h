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
	void CreateMeshMenu(void);

	static CMesh3D *m_pMesh = NULL;
	static bool m_bCreate = false;
	static int m_nVertical = 0;
	static int m_nSide = 0;
	static D3DXVECTOR3 m_pos;
	static D3DXVECTOR3 m_size;

	static bool m_bTimeStart = false;
	static int m_nTime = 0;
	static float m_fHeight = 0.0f;
	static D3DXVECTOR3 m_WaveCenter = {};
	static int m_nCycle = 100;
	static bool m_bDemo = false;
}

//-----------------------------------------------------------------------------
// 名前空間・テクスチャ生成
//-----------------------------------------------------------------------------
namespace TextureMake
{
	void TextureLoad(void);
	void TextureBind(void);

}

namespace Option
{

	void OperationExplanation(void);
}

namespace LightOption
{
	void LightMove(void);
}

#endif // !_IMGUI_H_

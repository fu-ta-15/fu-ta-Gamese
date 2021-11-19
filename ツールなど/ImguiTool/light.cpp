//*****************************************************************************
//																																								
// ライト処理 [light.cpp]																																		
// Author : SUZUKI FUUTA																																		
//																																								
//*****************************************************************************
//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "light.h"
#include "renderer.h"
#include "fade.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CLight::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	

	// 設定用方向ベクトル
	D3DXVECTOR3 vecDir[MAX_LIGHT];			

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアにする
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));

		switch (nCntLight)
		{
		case 0:
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向を設定
			vecDir[nCntLight] = D3DXVECTOR3(0.2f, -0.8f, 0.4f);

			// 正規化する
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);

			// 情報保存
			m_vecDir[nCntLight] = vecDir[nCntLight];

			// 正規化した情報を反映
			m_Light[nCntLight].Direction = vecDir[nCntLight];
			break;

		case 1:
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// ライトの方向を設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.2f, 0.8f, -0.4f);

			// 正規化する
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	

			// 情報保存
			m_vecDir[nCntLight] = vecDir[nCntLight];

			// 正規化した情報を反映
			m_Light[nCntLight].Direction = vecDir[nCntLight];
			break;

		case 2:
			// ライトの種類を設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// ライトの方向を設定
			vecDir[nCntLight] = D3DXVECTOR3(0.9f, -0.1f, 0.4f);

			// 正規化する
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);

			// 情報保存
			m_vecDir[nCntLight] = vecDir[nCntLight];

			// 正規化した情報を反映
			m_Light[nCntLight].Direction = vecDir[nCntLight];
				break;

		default:
			break;
		}

		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_Light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CLight::Update(void)
{
}

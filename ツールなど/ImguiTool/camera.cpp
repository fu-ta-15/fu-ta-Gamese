//*****************************************************************************
//																																								
// カメラ処理 [camera.cpp]																																		
// Author : SUZUKI FUUTA																																		
//																																								
//*****************************************************************************
//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "camera.h"
#include "keyinput.h"
#include "Dinput.h"
#include "player.h"
#include "fade.h"
#include "renderer.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define CAMERA_POS_Y		(100.0f)										 // カメラのY座標
#define CAMERA_POS_Z		(-200.0f)										 // カメラのZ座標
#define INIT_CAMERA_POSV	(D3DXVECTOR3(0.0f, CAMERA_POS_Y, CAMERA_POS_Z))	 // カメラの視点位置

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_posV = INIT_CAMERA_POSV;					// 視点(現在)
	m_posR = ZeroVector3;						// 注視点(現在)
	m_rot = ZeroVector3;						// カメラの向き
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	m_fPosSpeed = 1.5f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
void CCamera::Init(void)
{
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// カメラの更新
//=============================================================================
void CCamera::Update(void)
{
	// キー入力クラスのポインタ
	CKey *pKey = CManager::GetKey();

	// キー入力によるカメラの移動（キー入力の状態・キーの種類）
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_Z))
	{
		// Y軸の角度
		m_rot.y -= 0.005f;

		// 3.14fより大きくなったとき値を-3.14fにする
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 注視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * 200;
		m_posR.z = m_posV.z + cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_C))
	{
		// Y軸の角度
		m_rot.y += 0.005f;

		// 3.14fより大きくなったとき値を-3.14fにする
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 注視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * 200;
		m_posR.z = m_posV.z + cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_Q))
	{
		// Y軸の角度
		m_rot.y -= 0.05f;

		// 3.14fより大きくなったとき値を-3.14fにする
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 視点の更新
		m_posV.x = m_posR.x - sinf(m_rot.y) * 200;
		m_posV.z = m_posR.z - cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_E))
	{
		// Y軸の角度
		m_rot.y += 0.05f;

		// -3.14fより小さくなったとき値を3.14fにする
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		// 視点の更新
		m_posV.x = m_posR.x - sinf(m_rot.y) * 200;
		m_posV.z = m_posR.z - cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_W))
	{
		// 視点の前移動
		m_posV.x += m_fPosSpeed * sinf(m_rot.y);
		m_posV.z += m_fPosSpeed * cosf(m_rot.y);

		// 視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * 200;
		m_posR.z = m_posV.z + cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_S))
	{
		// 視点の後ろ移動
		m_posV.x -= m_fPosSpeed * sinf(m_rot.y);
		m_posV.z -= m_fPosSpeed * cosf(m_rot.y);

		// 視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * 200;
		m_posR.z = m_posV.z + cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A))
	{
		// 視点の左移動
		m_posV.x -= m_fPosSpeed * cosf(m_rot.y);
		m_posV.z += m_fPosSpeed * sinf(m_rot.y);

		// 視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * 200;
		m_posR.z = m_posV.z + cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D))
	{
		// 視点の右移動
		m_posV.x += m_fPosSpeed * cosf(m_rot.y);
		m_posV.z -= m_fPosSpeed * sinf(m_rot.y);

		// 視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * 200;
		m_posR.z = m_posV.z + cosf(m_rot.y) * 200;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_T))
	{
		// 注視点の上移動
		m_posR.y += m_fPosSpeed;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_B))
	{
		// 注視点の下移動
		m_posR.y -= m_fPosSpeed;
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_Y))
	{
		// 視点の上移動
		m_posV.y += m_fPosSpeed;

	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_N))
	{
		// 視点の下移動
		m_posV.y -= m_fPosSpeed;
	}
}

//=============================================================================
// カメラの設置
//=============================================================================
void CCamera::SetCamera(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),							// 画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// 比率
		50.0f,											// 手前距離
		6000.0f);										// 奥の距離

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

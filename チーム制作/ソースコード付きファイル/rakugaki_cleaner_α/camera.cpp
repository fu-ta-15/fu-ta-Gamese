//-----------------------------------------------------------------
//
// カメラ (camera.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define LENGTH					(600.0f)		// 奥行
#define MOVE_ROT_LookAtPoint	(0.05f)			// 注視点回転量
#define MOVE_ROT_Point			(0.05f)			// 視点回転量
#define MOVE_LookAtPoint		(0.5f)			// 注視点移動量
#define MOVE_Point				(10.0f)			// 視点移動量

//-----------------------------------------------------------------
// CCameraのコンストラクタ
//-----------------------------------------------------------------
CCamera::CCamera()
{

}

//-----------------------------------------------------------------
// CCameraのデストラクタ
//-----------------------------------------------------------------
CCamera::~CCamera()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport)
{
	// ポインタ変数の生成
	CCamera *pCamera;
	pCamera = new CCamera;

	// NULLチェック
	if (pCamera != NULL)
	{
		// 初期化処理
		pCamera->Init(posV, posR, viewport);
	}

	return pCamera;
}

//-----------------------------------------------------------------
// カメラの初期化処理
//-----------------------------------------------------------------
void CCamera::Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport)
{
	// 変数の初期化
	m_posR = posR;
	m_posV = posV;
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = SCREEN_WIDTH;
	m_viewport.Height = SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_viewport = viewport;
	m_bParallel = false;

#ifdef _DEBUG
	m_posVDef = m_posV;
	m_posRDef = m_posR;
	m_rotDef = m_rot;
#endif

	SetCamera();
}

//-----------------------------------------------------------------
// カメラの終了処理
//-----------------------------------------------------------------
void CCamera::Uninit(void)
{

}

//-----------------------------------------------------------------
// カメラの更新処理
//-----------------------------------------------------------------
void CCamera::Update(void)
{
#ifdef _DEBUG
	// ポーズ情報の取得
	bool bPause = CManager::GetPause();

	if (bPause != true)
	{
		// マウス情報の取得
		CInputMouse *pMouse = GET_MOUSE;

		/*if (pMouse->GetPress(MOUSE_LEFT))
		{
			D3DXVECTOR2 move = pMouse->GetMouseMove();

			m_rot.y += move.x * 0.005f;

			// 視点の更新
			m_posV.y += move.y;

			// 視点の高さ制限
			if (m_posV.y < -m_fLength)
			{
				m_posV.y = -m_fLength;
			}
			else if (m_posV.y > m_fLength)
			{
				m_posV.y = m_fLength;
			}
			float fAngle = asinf(m_posV.y / m_fLength);
			m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength * cosf(fAngle);
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength * cosf(fAngle);
		}
		else if (pMouse->GetPress(MOUSE_RIGHT))
		{
			D3DXVECTOR2 move = pMouse->GetMouseMove();

			m_rot.y += move.x * 0.005f;

			// 注視点の移動
			float fAngle = asinf(m_posV.y / m_fLength);
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength * cosf(fAngle);
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength * cosf(fAngle);
			m_posR.y -= move.y;
		}*/

		// 注視点操作
		LookAtPointRotate();

		// 視点操作
		PointRotate();

		// 注視点移動
		LookAtPointMove();

		// 視点移動
		PointMove();

		Reset();
	}
	// デバック表示
	CDebugProc::Print("\n--- カメラ情報 ---\n");
	CDebugProc::Print("カメラの視点  :%f %f %f\n", m_posV.x, m_posV.y, m_posV.z);
	CDebugProc::Print("カメラの注視点:%f %f %f\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//-----------------------------------------------------------------
// カメラの設置
//-----------------------------------------------------------------
void CCamera::SetCamera(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// 変数の受け渡し
	pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	if (m_bParallel == false)
	{
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)m_viewport.Width / (float)m_viewport.Height,
			10.0f,
			3000.0f);
	}
	else
	{
		// プロジェクションマトリックスを作成(平行投影)
		D3DXMatrixOrthoLH(&m_mtxProjection, m_parallelSize.x, m_parallelSize.y, -3000.0f, 3000.0f);
	}

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

//-----------------------------------------------------------------
// カメラの注視点回転処理
//-----------------------------------------------------------------
void CCamera::LookAtPointRotate(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_LookAtPoint_LEFT) == true)
	{// 視点の左回転
		m_rot.y -= MOVE_ROT_LookAtPoint;						// 回転量

		if (m_rot.y < -D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 注視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_LookAtPoint_RIGHT) == true)
	{// 視点の右回転
		m_rot.y += MOVE_ROT_LookAtPoint;						// 回転量

		if (m_rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			m_rot.y -= D3DX_PI * 2.0f;
		}

		// 注視点の更新
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;	
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// カメラの視点回転処理
//-----------------------------------------------------------------
void CCamera::PointRotate(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_Point_LEFT) == true)
	{// 視点の左回転
		m_rot.y -= MOVE_ROT_Point;						// 回転量

		if (m_rot.y < -D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			m_rot.y += D3DX_PI * 2.0f;
		}

		// 視点の更新
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
	if (pKey->GetPress(KEY_Point_RIGHT) == true)
	{// 視点の右回転
		m_rot.y += MOVE_ROT_Point;						// 回転量

		if (m_rot.y > D3DX_PI)
		{// -3.14fより小さくなったとき値を3.14fにする
			m_rot.y -= D3DX_PI * 2.0f;
		}
		// 視点の更新
		m_posV.x = m_posR.x - sinf(m_rot.y) * LENGTH;	
		m_posV.z = m_posR.z - cosf(m_rot.y) * LENGTH;	
	}
}

//-----------------------------------------------------------------
// カメラの注視点移動処理
//-----------------------------------------------------------------
void CCamera::LookAtPointMove(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_LookAtPoint_UP) == true)
	{
		// 注視点の上移動
		m_posR.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_LookAtPoint_DOWN) == true)
	{
		// 注視点の下移動
		m_posR.y -= MOVE_Point;
	}
}

//-----------------------------------------------------------------
// カメラの視点移動処理
//-----------------------------------------------------------------
void CCamera::PointMove(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_MOVE_Up) == true)
	{
		// 視点の前移動
		m_posV.x += MOVE_Point * sinf(m_rot.y);	
		m_posV.z += MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Down) == true)
	{
		// 視点の後ろ移動
		m_posV.x -= MOVE_Point * sinf(m_rot.y);
		m_posV.z -= MOVE_Point * cosf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Right) == true)
	{
		// 視点の右移動
		m_posV.x += MOVE_Point * cosf(m_rot.y);
		m_posV.z -= MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Left) == true)
	{
		// 視点の左移動
		m_posV.x -= MOVE_Point * cosf(m_rot.y);
		m_posV.z += MOVE_Point * sinf(m_rot.y);
		m_posR.x = m_posV.x + sinf(m_rot.y) * LENGTH;
		m_posR.z = m_posV.z + cosf(m_rot.y) * LENGTH;
	}
	if (pKey->GetPress(KEY_MOVE_Point_UP) == true)
	{
		// 視点の上移動
		m_posV.y += MOVE_Point;
	}
	if (pKey->GetPress(KEY_MOVE_Point_DOWN) == true)
	{
		// 視点の下移動
		m_posV.y -= MOVE_Point;
	}
}

#ifdef _DEBUG
//-----------------------------------------------------------------
// カメラのリセット
//-----------------------------------------------------------------
void CCamera::Reset(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	if (pKey->GetPress(KEY_RESET) == true)
	{
		// 変数の初期化
		m_posV = m_posVDef;
		m_posR = m_posRDef;
		m_rot = m_rotDef;
	};
}
#endif

//-----------------------------------------------------------------
// ワールド座標からスクリーン座標に変換
//-----------------------------------------------------------------
D3DXVECTOR3 CCamera::ChangePos_WorldtoScreen(D3DXVECTOR3 pos)
{
	// 変数宣言
	D3DXMATRIX mtx, mtxObject;
	D3DXMATRIX mtxScreen =
	{ SCREEN_WIDTH / 2.0f,0,0,0,
		0,-SCREEN_HEIGHT / 2.0f,0,0,
		0,0,1,0,
		SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT / 2.0f ,0,1 };

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	// ワールド座標からスクリーン座標に変換
	D3DXMatrixTranslation(&mtxObject, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxObject);
	D3DXMatrixMultiply(&mtx, &mtx, &m_mtxView);
	D3DXMatrixMultiply(&mtx, &mtx, &m_mtxProjection);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxScreen);

	return D3DXVECTOR3(mtx._41 / mtx._44, mtx._42 / mtx._44, 0.0f);
}

//-----------------------------------------------------------------
// 平行投影の設定
//-----------------------------------------------------------------
void CCamera::SetParallel(D3DXVECTOR2 size)
{
	// 平行投影をOnにする
	m_bParallel = true;

	// サイズの確定
	m_parallelSize = size;

	SetCamera();
}
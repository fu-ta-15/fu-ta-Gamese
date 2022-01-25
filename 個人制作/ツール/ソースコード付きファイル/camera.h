//*****************************************************************************
//
// カメラヘッダファイル [camera.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void MoveCamera(const int nDIK);

private:
	D3DXVECTOR3 m_posV;				// 視点(現在)
	D3DXVECTOR3 m_posR;				// 注視点(現在)
	D3DXVECTOR3 m_rot;				// カメラの向き
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	float		m_fPosSpeed;		// 移動速度

};


#endif // !_CAMERA_H_

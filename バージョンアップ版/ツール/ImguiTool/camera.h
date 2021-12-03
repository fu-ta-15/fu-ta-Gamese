//*****************************************************************************
//
// カメラヘッダファイル [camera.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"


class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

private:
	D3DXVECTOR3 m_posV;			// 視点(現在)
	D3DXVECTOR3 m_posR;			// 注視点(現在)
	D3DXVECTOR3 m_rot;			// カメラの向き
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	float		m_fPosSpeed;

};


#endif // !_CAMERA_H_

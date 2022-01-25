//-------------------------------------------------------------------
//
// カメラヘッダー (camera.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//-------------------------------------------------------------------
// カメラの種類(列挙型)
//-------------------------------------------------------------------
typedef enum
{
	CAMERATYPE_PLAYER_LOOKDOWN = 0,		// プレイヤー見下ろしカメラ
	CAMERATYPE_PLAYER,				// プレイヤー追従カメラ
	CAMERATYPE_MAX,
}CAMERATYPE;

//-------------------------------------------------------------------
// カメラクラス(基本クラス)
//-------------------------------------------------------------------
class CCamera
{
public:
	CCamera();
	~CCamera();

	static CCamera *Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport);
	void Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DVIEWPORT9 viewport);
	void Uninit(void);		
	void Update(void);		
	void SetCamera(void);	

	D3DXVECTOR3 GetRotate(void) { return m_rot; }
	D3DXVECTOR3 GetPositionV(void) { return m_posV; }
	D3DXVECTOR3 GetPositionR(void) { return m_posR; }
	D3DXVECTOR3 GetVecU(void) { return m_vecU; }

	D3DXMATRIX GetProjection(void) { return m_mtxProjection; }
	D3DXMATRIX GetView(void) { return m_mtxView; }

	D3DXVECTOR3 ChangePos_WorldtoScreen(D3DXVECTOR3 pos);

	void SetParallel(D3DXVECTOR2 size);

private:
	void LookAtPointRotate(void);	// カメラの注視点回転処理
	void PointRotate(void);			// カメラの視点回転処理
	void LookAtPointMove(void);		// カメラの注視点移動
	void PointMove(void);			// カメラの視点移動
	void Reset(void);

	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXVECTOR3 m_rot;				// 向き
	D3DXMATRIX m_mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			// ビューマトリックス
	D3DVIEWPORT9 m_viewport;		// ビューポート

	float m_fLength;

	D3DXVECTOR2 m_parallelSize;		// 平行投影時のサイズ
	bool m_bParallel;				// 平行投影かどうか

#ifdef _DEBUG
	D3DXVECTOR3 m_posVDef;				// 視点
	D3DXVECTOR3 m_posRDef;				// 注視点
	D3DXVECTOR3 m_rotDef;				// 向き
#endif
};
#endif
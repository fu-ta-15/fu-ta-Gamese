//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "sceneX.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CShadow;			// 影クラス

//-----------------------------------------------------------------------------
// モデルクラス(基本クラス)
//-----------------------------------------------------------------------------
class CModel
{
public:
	CModel();
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,char *pModelFileName);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pModelFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModel *pModel) { m_pParent = pModel; }		// 親モデルの設定
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			// ワールドマトリックス取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture[8];	// テクスチャへのポインタ
	LPD3DXMESH m_pMesh;					// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアル(材質情報)へのポインタ
	DWORD m_nNumMat;					// マテリアルの数
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 中心座標
	D3DXVECTOR3 m_rot;					// 回転角
	CModel *m_pParent;					// 親モデルのポインタ

	int *m_pTex;						// テクスチャ番号
	CShadow *m_pShadow;					// 影クラス
};
#endif
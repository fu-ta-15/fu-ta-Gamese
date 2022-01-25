//=============================================================================
//
// シーンメッシュフィールド処理 [sceneMeshfield.h]
// Author : 
//
//=============================================================================
#ifndef _SCENEMESHFIELD_H_
#define _SCENEMESHFIELD_H_

#include "scene.h"

//-----------------------------------------------------------------------------
// シーンメッシュフィールドクラス(基本クラス)
//-----------------------------------------------------------------------------
class CSceneMeshfield : public CScene
{
public:
	typedef struct
	{
		D3DXMATRIX mtxWorld;		// ワールドマトリックス
		D3DXVECTOR3 pos;			// 壁の中心座標
		D3DXVECTOR3 rot;			// 壁の向き
		int nCntBlockX;				// ブロック数(列)
		int nCntBlockZ;				// ブロック数(行)
		int nCntVecX;				// 列の頂点数
		int nCntVecZ;				// 行の頂点数
		float fWidth;				// 幅
		float fDepth;				// 奥行
		float fSizeWidth;			// 幅のサイズ
		float fSizeDepth;			// 奥行のサイズ
	}MeshField;

	CSceneMeshfield(int nPriority);
	~CSceneMeshfield();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTextureNum(int nTex) { m_nTex = nTex; }

	D3DXVECTOR3 GetPosition(void) { return m_meshField.pos; }
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_meshField.fWidth, m_meshField.fDepth); }
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetPosition(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR2 size);
	void SetVertex(void);
	void SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets);
	void SetCol(D3DXCOLOR col);
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }
	void SetBlock(D3DXVECTOR2 block);
	D3DXVECTOR2 GetBlock(void) { return D3DXVECTOR2((float)m_meshField.nCntBlockX, (float)m_meshField.nCntBlockZ); }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャ情報のポインタ
	MeshField m_meshField;						// メッシュフィールド情報
	D3DXCOLOR m_col;							// 色情報
	int m_nTex;

	bool m_bDeath;								// 死亡フラグ
};
#endif
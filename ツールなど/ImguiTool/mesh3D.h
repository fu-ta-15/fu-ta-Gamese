//#############################################################################
//
// メッシュヘッダファイル [mesh.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _MESH3D_H_
#define _MESH3D_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include "Scene2D.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CMesh3D : public CScene
{
public:
	//-------------------------------------------------------------------------
	// メンバ関数
	//-------------------------------------------------------------------------
	CMesh3D(Priority type);
	~CMesh3D();

	static CMesh3D *Create(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static CMesh3D *Create(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, Priority nPriority);
	HRESULT CreateTexture(const LPCSTR pSrcFile);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セット関数  
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(const D3DXVECTOR3 size) { m_size = size; }
	void SetVertical(const int nVertical) { m_nVertical = nVertical; }
	void SetSide(const int nSide) { m_nSide = nSide; }
	void SetCol(const D3DXCOLOR col) { m_col = col; }
	void SetVtx(LPDIRECT3DVERTEXBUFFER9 Vtx) { m_pVtxBuff = Vtx; }

	// ゲット関数 
	LPDIRECT3DVERTEXBUFFER9 GetVtx() { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetSize() { return m_size; }
	int GetVertical() { return m_nVertical; }
	int GetSide() { return m_nSide; }
	int GetVtxNum() { return m_nVtx; }
	VERTEX_3D *GetVERTEX() { return m_pVtx; }

	void MeshWave(const D3DXVECTOR3& center, int ntime, float fHeight, int nCycle);
	void MeshWave(const D3DXVECTOR3& center, int ntime, int nCycle);
	void MeshWave(int nID, int ntime, float fHeight);
	void MeshCycleMove(void);
	void VtxPos(int nID, float fHeight);

private:
	HRESULT MeshCreate(int nVertical, int nSide, WORD *pIdx);
	void MeshSetTex(int nVertical, int nSide, VERTEX_3D *pVtx);
	void MeshSetPos(int nVertical, int nSide, VERTEX_3D *pVtx);
	void MeshSetRhw(VERTEX_3D *pVtx);
	void MeshSetCol(VERTEX_3D *pVtx);
	int VertexCreate(int nVertical, int nSide);
	int IndexCreate(int nVertical, int nSide);
	void MeshNor(void);
	D3DXVECTOR3 PolygonNormal(const D3DXVECTOR3& pos0, const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2);

	float getDistance(float x, float y, float x2, float y2);

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// 頂点バッファの情報
	LPDIRECT3DTEXTURE9			m_pTexture;				// テクスチャの情報
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// インデックスバッファの情報
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_size;					// サイズ
	D3DXVECTOR3					m_rot;					// 向き
	D3DXVECTOR3					m_moveRot;
	D3DXCOLOR					m_col;					// 色
	D3DXMATRIX					m_mtxWorld;				// ワールドマトリックス
	int							m_nVertical;			// 縦の分割数
	int							m_nSide;				// 横の分割数
	int							m_nVtx;					// 頂点
	int							m_nIdx;					// インデックス
	bool						m_bWave;				// 波の表現するかしないか
	
	D3DXVECTOR3					**m_Vector;
	D3DXVECTOR3					*m_Nor;

	VERTEX_3D *					m_pVtx;					// 頂点情報の保管用
};

#endif
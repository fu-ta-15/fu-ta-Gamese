//#############################################################################
//
// メッシュヘッダファイル [mesh.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _MESH_H_
#define _MESH_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "scene.h"


//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CMesh : public CScene
{
public:
	CMesh(Priority type);	  // コンストラクタ
	~CMesh();				  // デストラクタ

	// ポリゴンメッシュのクリエイト
	static CMesh *Create(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, CScene::ObjectType type);
	
	HRESULT CreateTexture(const LPCSTR pSrcFile);														// テクスチャの生成
	HRESULT Init(void) { return S_OK; }																	// 初期化(純粋仮想関数)
	HRESULT Init(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size);	// 引数付き初期化
	void Uninit(void);																					// 終了
	void Update(void);																					// 更新
	void Draw(void);																					// 描画


	// セット関数  
	void SetVtxPosY(int nID, float posy);
	void SetVtxPosX(int nID, float posx);
	void SetMove(const D3DXVECTOR3 move);
	void SetPos(const D3DXVECTOR3 pos)			{ m_pos = pos; }
	void SetSize(const D3DXVECTOR3 size)		{ m_size = size; }
	void SetVertical(const int nVertical)		{ m_nVertical = nVertical; }
	void SetSide(const int nSide)				{ m_nSide = nSide; }
	void SetCol(const D3DXCOLOR col)			{ m_col = col; }
	void SetVtx(LPDIRECT3DVERTEXBUFFER9 Vtx)	{ m_pVtxBuff = Vtx; }

	// ゲット関数 
	LPDIRECT3DVERTEXBUFFER9 GetVtx()	{ return m_pVtxBuff; }
	D3DXVECTOR3 GetPos()				{ return m_pos; }
	D3DXVECTOR3 GetMove()				{ return m_move; }
	D3DXVECTOR3 GetSize()				{ return m_size; }
	int GetVertical()					{ return m_nVertical; }
	int GetSide()						{ return m_nSide; }
	int GetVtxNum()						{ return m_nVtx; }
	VERTEX_2D *GetVERTEX()				{ return m_pVtx; }

private:
	HRESULT MeshCreate(int nVertical, int nSide, WORD *pIdx);
	void MeshSetTex(int nVertical, int nSide, VERTEX_2D *pVtx);
	void MeshSetPos(int nVertical, int nSide, VERTEX_2D *pVtx);
	void MeshSetNor(VERTEX_2D *pVtx);
	void MeshSetCol(VERTEX_2D *pVtx);
	int VertexCreate(int nVertical, int nSide);
	int IndexCreate(int nVertical, int nSide);

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// 頂点バッファの情報
	LPDIRECT3DTEXTURE9			m_pTexture;				// テクスチャの情報
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// インデックスバッファの情報
	D3DXVECTOR3					m_pos;					// 位置
	D3DXVECTOR3					m_move;					// 位置
	D3DXVECTOR3					m_Centerpos;			// 中心位置
	D3DXVECTOR3					m_size;					// サイズ
	D3DXVECTOR3					m_rot;					// 向き
	D3DXCOLOR					m_col;					// 色
	D3DXMATRIX					m_mtxWorld;				// ワールドマトリックス
	int							m_nVertical;			// 縦の分割数
	int							m_nSide;				// 横の分割数
	int							m_nVtx;					// 頂点
	int							m_nIdx;					// インデックス
	bool						m_bWave;				// 波の表現するかしないか
	VERTEX_2D *					m_pVtx;					// 頂点情報の保管用
	VERTEX_2D *					m_pVtxsub;				// 頂点情報の保管用
};

#endif
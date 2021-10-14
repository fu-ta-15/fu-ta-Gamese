//#############################################################################
//
// メッシュ処理 [mesh.cpp]
// Author : SUZUKI FUUTA
//
//#############################################################################

//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "mesh.h"
#include "Dinput.h"
#include "renderer.h"
#include "manager.h"
#include "move.h"

//-----------------------------------------------------------------------------
//　マクロ定義
//-----------------------------------------------------------------------------
#define POLYGON_VTX							(2)
#define ADD_SIDE_INDEX						(6 + (2 * m_nVertical))
#define ADD_VER_INDEX						(2 * m_nVertical)
#define DRAW_INDX							(m_nIdx - 2)

//-----------------------------------------------------------------------------
//　静的メンバ変数
//-----------------------------------------------------------------------------


//=============================================================================
// コンストラクタ
//=============================================================================
CMesh::CMesh(Priority type) : CScene(type)
{
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_pIdxBuff = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMesh::~CMesh()
{
}

//=============================================================================
// メッシュの生成
//=============================================================================
CMesh * CMesh::Create(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, CScene::ObjectType type)
{
	CMesh *pMesh = NULL;

	pMesh = new CMesh(type);
	pMesh->SetPos(pos);
	pMesh->SetMove(ZeroVector3);
	pMesh->SetSize(size);
	pMesh->SetSide(nSide);
	pMesh->m_col = WhiteColor;
	pMesh->SetVertical(nVertical);
	pMesh->Init();

	return pMesh;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
HRESULT CMesh::CreateTexture(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTexture);

	return S_OK;
}

//=============================================================================
// メッシュポリゴンの初期化処理
//=============================================================================
HRESULT CMesh::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	m_nVtx = VertexCreate(m_nVertical, m_nSide);			// 総合頂点数
	m_nIdx = IndexCreate(m_nVertical, m_nSide);				// 総合インデックス
	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer((sizeof(VERTEX_2D) * m_nVtx),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL))) {
		return E_FAIL;
	}

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	MeshSetPos(m_nVertical, m_nSide, m_pVtx);		// 位置の設定

	MeshSetRhw(m_pVtx);							// 除算数 1.0fで固定

	MeshSetCol(m_pVtx);							// 色の設定

	MeshSetTex(m_nVertical, m_nSide, m_pVtx);		// テクスチャの頂点座標の設定

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;	// インデックス情報へのポインタ

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// メッシュインデックス
	MeshCreate(m_nVertical, m_nSide, pIdx);

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;	// 初期化完了
}

//=============================================================================
// メッシュポリゴンの終了処理
//=============================================================================
void CMesh::Uninit(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//=============================================================================
// メッシュポリゴンの更新処理
//=============================================================================
void CMesh::Update(void)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	MeshSetCol(m_pVtx);							// 色の設定


	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// メッシュポリゴンの描画処理
//=============================================================================
void CMesh::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0, 0,
		m_nVtx, 0, DRAW_INDX);
}

//=============================================================================
// 頂点に座標を代入
//=============================================================================
void CMesh::SetWavePos(int nID, float pos)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx[nID].pos.y = pos;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

void CMesh::SetVtxPosY(int nID, float posy)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx[nID].pos.y = posy;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

void CMesh::SetVtxPosX(int nID, float posx)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx[nID].pos.x = posx;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

void CMesh::SetVtxMoveY(int nID, float move)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx[nID].pos.y += move;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

void CMesh::SetVtxMoveX(int nID, float move)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx[nID].pos.x += move;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}


void CMesh::WavePosY(int nID)
{
	m_nWaveCnt++;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx[nID].pos.y += CMove::CosMove(2.0f, 120.0f, m_nWaveCnt);

	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		int nID_Diff = nID - nCnt;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

D3DXVECTOR3 CMesh::GetCenterPos(void)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	D3DXVECTOR3 pos;

	float nV_ID = (float)(m_nVertical - 1) / 2;

	return m_pVtx[m_nVtx / 2].pos;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// メッシュポリゴンの生成準備　頂点に番号振り分け
//=============================================================================
HRESULT CMesh::MeshCreate(int nVertical, int nSide, WORD * pIdx)
{
	int nT = 0;
	int nCntSide = nSide;
	int nCntVertical = ((2 + nVertical));
	int nWrapBack = 2 + nVertical;
	int nCntPoly = 0;
	int nCnt = 0;

	for (nCnt = 0; nCnt < m_nIdx / 2; nCnt++)
	{
		if (nCntPoly == nWrapBack && nCntSide != 0)
		{
			pIdx[0] = nT - 1;
			pIdx[1] = nT + nWrapBack;

			nCntPoly = 0;
			nCntSide -= 1;
			nCntVertical = ((2 + nVertical));
			pIdx += 2;
		}
		else if (nCntSide == 0 && nCntVertical == 0)
		{
			break;
		}
		else
		{
			pIdx[0] = nT + nWrapBack;
			pIdx[1] = nT;

			nT += 1;
			nCntVertical -= 1;
			nCntPoly += 1;
			pIdx += 2;
		}
	}
	return S_OK;	// 初期化完了
}

//=============================================================================
// メッシュポリゴンのテクスチャ座標設定
//=============================================================================
void CMesh::MeshSetTex(int nVertical, int nSide, VERTEX_2D * pVtx)
{
	float nNumVertical = (1.0f + (float)nVertical);
	float nNumSide = (1.0f + (float)nSide);
	float nCntVertical = 0.0f;
	float nCntSide = 0.0f;

	// テクスチャ座標振り分け
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{	// 座標の代入
		pVtx[0].tex = D3DXVECTOR2((float)(nCntVertical / (nNumVertical)), (float)(nCntSide / nNumSide));
		if (nCntVertical == nNumVertical)
		{// Y段目のX段が端に行ったら
			nCntSide += 1;			// 一段下げる
			nCntVertical = 0.0f;	// Xを元に戻す
		}
		else
		{// Xがまだ端に振り分けてない時
			nCntVertical += 1;		// １進める
		}
	}
}

//=============================================================================
// メッシュポリゴンの位置情報の設定
//=============================================================================
void CMesh::MeshSetPos(int nVertical, int nSide, VERTEX_2D * pVtx)
{
	int nNumVertical = (1 + nVertical);
	int nNumSide = (1 + nSide);
	int nCntVertical = 0;
	int nCntSide = 0;

	// 各頂点の座標
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		pVtx[nCnt].pos = D3DXVECTOR3((m_pos.x + ((m_size.x / nNumVertical) * nCntVertical)), (m_pos.y + ((m_size.y / nNumSide) * nCntSide)), 0.0f);
		
		if (nCntVertical == nNumVertical)
		{
			nCntSide += 1;
			nCntVertical = 0;
		}
		else
		{
			nCntVertical += 1;
		}
	}
	m_PolygonLength = D3DXVECTOR3((((m_size.x / nNumVertical) * 1)), (((m_size.y / nNumSide) * 1)), 0.0f);
}

//=============================================================================
// メッシュポリゴンの法線の向き
//=============================================================================
void CMesh::MeshSetRhw(VERTEX_2D * pVtx)
{
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{
		pVtx[0].rhw = 1.0f;
	}

}

//=============================================================================
// メッシュポリゴンの色設定
//=============================================================================
void CMesh::MeshSetCol(VERTEX_2D * pVtx)
{
	// 頂点カラー・赤・緑・青・アルファ
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{
		pVtx[0].col = m_col;
	}
}

//=============================================================================
// メッシュポリゴンの頂点の計算
//=============================================================================
int CMesh::VertexCreate(int nVertical, int nSide)
{
	int nVtxVertical = POLYGON_VTX;
	int nVtxSide = POLYGON_VTX;
	int nVtx = 0;

	nVtx = (nVertical + nVtxVertical) * (nSide + nVtxSide);

	return nVtx;
}

//=============================================================================
// メッシュポリゴンの描画番号の計算
//=============================================================================
int CMesh::IndexCreate(int nVertical, int nSide)
{
	int nIdx = 0;

	nIdx = 4 + (nSide * ADD_SIDE_INDEX) + ADD_VER_INDEX;

	return nIdx;
}


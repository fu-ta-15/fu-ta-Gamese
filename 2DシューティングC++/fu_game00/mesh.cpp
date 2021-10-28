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
#define ADD_SIDE_INDEX						(6 + (2 * m_nVertical))		// 横線の計算
#define ADD_VER_INDEX						(2 * m_nVertical)			// 縦線の計算
#define DRAW_INDX							(m_nIdx - 3)				// 描画するときのポリゴン数

//=============================================================================
// コンストラクタ
//=============================================================================
CMesh::CMesh(Priority type) : CScene(type)
{
	m_pVtxBuff = NULL;	 // 頂点バッファのポインタ
	m_pTexture = NULL;	 // テクスチャのポインタ
	m_pIdxBuff = NULL;	 // インデックスバッファのポインタ
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
	// クラスのポインタ変数
	CMesh *pMesh = new CMesh(type);

	if (pMesh != NULL)
	{// NULLチェック
		pMesh->Init(nVertical, nSide, pos, size);	// 初期化
	}

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
HRESULT CMesh::Init(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// 位置
	m_pos = pos;

	// サイズ
	m_size = size;

	// 中の縦線の数
	m_nVertical = nVertical;

	// 中の横線の数
	m_nSide = nSide;

	// 色の初期化
	m_col = WhiteColor;

	// 移動量
	m_move = ZeroVector3;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// 総合頂点数
	m_nVtx = VertexCreate(m_nVertical, m_nSide);

	// 総合インデックス
	m_nIdx = IndexCreate(m_nVertical, m_nSide);

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

	// 位置の設定
	MeshSetPos(m_nVertical, m_nSide, m_pVtx);

	// 除算数 1.0fで固定
	MeshSetRhw(m_pVtx);

	// 色の設定
	MeshSetCol(m_pVtx);

	// テクスチャの頂点座標の設定
	MeshSetTex(m_nVertical, m_nSide, m_pVtx);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックス情報へのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// メッシュインデックス
	MeshCreate(m_nVertical, m_nSide, pIdx);

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// メッシュポリゴンの終了処理
//=============================================================================
void CMesh::Uninit(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{// NULLチェック
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{// NULLチェック
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{// NULLチェック
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 削除
	Release();
}

//=============================================================================
// メッシュポリゴンの更新処理
//=============================================================================
void CMesh::Update(void)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

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
		0, 
		0,
		m_nVtx, 
		0, 
		DRAW_INDX);
}

//=============================================================================
// 頂点に座標を代入（Y座標）
//=============================================================================
void CMesh::SetVtxPosY(int nID, float posy)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 座標代入
	m_pVtx[nID].pos.y = posy;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点に座標を代入（X座標）
//=============================================================================
void CMesh::SetVtxPosX(int nID, float posx)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 座標代入
	m_pVtx[nID].pos.x = posx;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点座標に移動量を加算
//=============================================================================
void CMesh::SetMove(const D3DXVECTOR3 move)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 各頂点の座標
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		m_pVtx[nCnt].pos += move ;
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// メッシュポリゴンの生成準備　頂点に番号振り分け
//=============================================================================
HRESULT CMesh::MeshCreate(int nVertical, int nSide, WORD * pIdx)
{
	// インデックスに振り分けられるID
	int nIdxID = 0;

	// サイドの数
	int nCntSide = nSide;

	// 縦線の一列の総数
	int nCntVertical = ((2 + nVertical));	

	// 折り返しの番号
	int nWrapBack = 2 + nVertical;		

	// カウント用変数
	int nCnt = 0;							

	for (nCnt = 0; nCnt < m_nIdx; nCnt++)
	{
		// カウントが二で割れるとき
		if ((nCnt % 2) == 0)
		{
			if (nCntVertical != 0)
			{
				// 頂点ポリゴンの番号記憶
				pIdx[nCnt] = nIdxID + nWrapBack;
			}
			else
			{
				// 縮退ポリゴンの番号
				pIdx[nCnt] = nIdxID - 1;
			}
		}
		else if ((nCnt % 2) == 1)
		{
			if (nCntVertical != 0)
			{
				// 頂点ポリゴンの番号記憶
				pIdx[nCnt] = nIdxID;

				// 縦線一つ進める
				nIdxID += 1;
				nCntVertical -= 1;
			}
			else
			{
				// 縮退ポリゴンの番号
				pIdx[nCnt] = nIdxID + nWrapBack;

				// 横線一つ進める
				nCntSide -= 1;
				nCntVertical = ((2 + nVertical));
			}
		}
	}

	return S_OK;
}

//=============================================================================
// メッシュポリゴンのテクスチャ座標設定
//=============================================================================
void CMesh::MeshSetTex(int nVertical, int nSide, VERTEX_2D * pVtx)
{
	float nNumVertical = (float)(1 + nVertical);	 // 一列分の縦線の総数
	float nNumSide = (float)(1 + nSide);			 // 横線の数
	float nCntVertical = 0.0f;						 // 縦線のカウント
	float nCntSide = 0.0f;							 // 横線のカウント

	// テクスチャ座標振り分け
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{	
		// 座標の代入
		pVtx[0].tex = D3DXVECTOR2((float)(nCntVertical / (nNumVertical)), (float)(nCntSide / nNumSide));

		// Y段目のX段が端に行ったら
		if (nCntVertical == nNumVertical)
		{
			// 一段下げる
			nCntSide += 1;			

			// Xを元に戻す
			nCntVertical = 0.0f;	
		}
		else
		{// Xがまだ端に振り分けてない時

			// １進める
			nCntVertical += 1;		
		}
	}
}

//=============================================================================
// メッシュポリゴンの位置情報の設定
//=============================================================================
void CMesh::MeshSetPos(int nVertical, int nSide, VERTEX_2D * pVtx)
{
	int nNumVertical = (1 + nVertical);	 // 一列分の縦線の総数
	int nNumSide = (1 + nSide);			 // 横線の数
	int nCntVertical = 0;				 // 縦線のカウント
	int nCntSide = 0;					 // 横線のカウント

	// 各頂点の座標
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		// 座標を設定
		pVtx[nCnt].pos = D3DXVECTOR3((m_pos.x + ((m_size.x / nNumVertical) * nCntVertical)), (m_pos.y + ((m_size.y / nNumSide) * nCntSide)), 0.0f);
		
		// 端までカウントしたら
		if (nCntVertical == nNumVertical)
		{
			// 下にずらす
			nCntSide += 1;

			// カウント初期化
			nCntVertical = 0;
		}
		else
		{// 縦線のカウントアップ
			nCntVertical += 1;
		}
	}
}

//=============================================================================
// メッシュポリゴンの
//=============================================================================
void CMesh::MeshSetRhw(VERTEX_2D * pVtx)
{
	// 各頂点に除算数を１．０で固定
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
	// 各頂点に色の情報を代入
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
	return (nVertical + 2) * (nSide + 2);
}

//=============================================================================
// メッシュポリゴンの描画番号の計算
//=============================================================================
int CMesh::IndexCreate(int nVertical, int nSide)
{
	return (((nVertical + 2) * (nSide + 1)) * 2) + ((nSide + 1) * 2);
}


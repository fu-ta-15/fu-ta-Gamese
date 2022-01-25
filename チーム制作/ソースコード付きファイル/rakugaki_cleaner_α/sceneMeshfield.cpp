//==============================================================================================================
//
// メッシュフィールド (meshfield.cpp)
// Author:Itsuki Takaiwa
//
//==============================================================================================================
#include "sceneMeshfield.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

#include "texture.h"

//--------------------------------------------------------------------------------------------------------------
// マクロ定義
//--------------------------------------------------------------------------------------------------------------
#define WIDTH_NUM			(m_meshField.nCntBlockX)										// 列
#define DEPTH_NUM			(m_meshField.nCntBlockZ)										// 行
#define WIDTH_VEC			(WIDTH_NUM + 1)								// 列の頂点数
#define DEPTH_VEC			(DEPTH_NUM + 1)								// 行の頂点数

#define MAX_MF_VERTEX		(DEPTH_NUM * 2 * (WIDTH_NUM + 2) - 2)		// 総頂点数
#define MAX_INDEX			(WIDTH_NUM * DEPTH_NUM * 2 + (4 * (DEPTH_NUM - 1)))	// 総インデックス数
#define MAX_MESHFIELD		((WIDTH_NUM + 1) * (DEPTH_NUM + 1))					// 

//=============================================================================
// CSceneのコンストラクタ
//=============================================================================
CSceneMeshfield::CSceneMeshfield(int nPriority) : CScene(nPriority)
{
	// メンバ変数の初期化
	m_meshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshField.nCntBlockX = 1;
	m_meshField.nCntBlockZ = 1;
	m_meshField.fWidth = 0.0f;
	m_meshField.fDepth = 0.0f;
	m_meshField.fSizeWidth = m_meshField.nCntBlockX * m_meshField.fWidth;
	m_meshField.fSizeDepth = m_meshField.nCntBlockZ * m_meshField.fDepth;
}

//=============================================================================
// CSceneのデストラクタ
//=============================================================================
CSceneMeshfield::~CSceneMeshfield()
{

}

//--------------------------------------------------------------------------------------------------------------
// メッシュフィールドの初期化処理
//--------------------------------------------------------------------------------------------------------------
HRESULT CSceneMeshfield::Init(D3DXVECTOR3 pos,D3DXVECTOR2 size)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = CManager::GetRenderer()->GetDevice();

	// 情報の初期化
	m_meshField.pos = pos;
	m_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshField.fWidth = size.x;
	m_meshField.fDepth = size.y;
	m_meshField.fSizeWidth = m_meshField.nCntBlockX * m_meshField.fWidth;
	m_meshField.fSizeDepth = m_meshField.nCntBlockZ * m_meshField.fDepth;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_MESHFIELD,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_VEC; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC; nCntX++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-m_meshField.fSizeWidth / 2.0f + (float)nCntX * m_meshField.fWidth,
				0.0f,
				m_meshField.fSizeDepth / 2.0f - (float)nCntZ * m_meshField.fDepth);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[0].tex1 = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

			// 次の情報
			pVtx++;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * MAX_MF_VERTEX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,						// フォーマット
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// インデックス情報へのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < DEPTH_NUM; nCntZ++)
	{
		for (int nCntX = 0; nCntX < WIDTH_VEC + 1; nCntX++)
		{
			if (nCntX != 0 && nCntX == WIDTH_VEC && nCntZ != DEPTH_NUM - 1)
			{// 右端から折り返す時
				pIdx[0] = pIdx[-1];
				pIdx[1] = pIdx[-2] + 1;
			}
			else if (nCntZ == DEPTH_NUM - 1 && nCntX == WIDTH_VEC)
			{// 終了時に無視する
				break;
			}
			else
			{// 通常配置
				pIdx[0] = WIDTH_VEC + (WIDTH_VEC * nCntZ) + nCntX;
				pIdx[1] = pIdx[0] - WIDTH_VEC;
			}
			// 次の情報に移動
			pIdx += 2;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------
// メッシュフィールドの終了処理
//--------------------------------------------------------------------------------------------------------------
void CSceneMeshfield::Uninit(void)
{
	if (m_bDeath == true)
	{
		// インデックスの開放
		if (m_pIdxBuff != NULL)
		{
			m_pIdxBuff->Release();
			m_pIdxBuff = NULL;
		}

		// 頂点バッファの開放
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}

		// テクスチャの開放
		if (m_pTexture != NULL)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
	}
	else
	{
		m_bDeath = true;
	}

	Release();
}

//--------------------------------------------------------------------------------------------------------------
// メッシュフィールドの更新処理
//--------------------------------------------------------------------------------------------------------------
void CSceneMeshfield::Update(void)
{

}

//--------------------------------------------------------------------------------------------------------------
// メッシュフィールドの描画処理
//--------------------------------------------------------------------------------------------------------------
void CSceneMeshfield::Draw(void)
{
	// テクスチャ管理情報の取得
	CTexture *pTexture = CManager::GetTexture();

	// デバイスの取得
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_meshField.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_meshField.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, pTexture->GetAddress(m_nTex));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_MESHFIELD, 0, MAX_INDEX);
}

//--------------------------------------------------------------------------------------------------------------
// ブロック数の設定
//-----------------------------------------------------------------------------------------------w w---------------
void CSceneMeshfield::SetBlock(D3DXVECTOR2 block)
{
	m_meshField.nCntBlockX = (int)block.x;
	m_meshField.nCntBlockZ = (int)block.y;
}
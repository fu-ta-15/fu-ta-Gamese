//*****************************************************************************
//
// 数字（スコアやタイム）処理 [number.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "number.h"
#include "renderer.h"

//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CNumber::m_pTexture;

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
// ナンバーポリゴンの生成
//=============================================================================
CNumber * CNumber::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const float fSplit)
{
	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{
		pNumber = new CNumber;

		pNumber->m_pos = pos;
		pNumber->m_size = size;
		pNumber->m_fSplit = fSplit;
		pNumber->Init();
	}

	return pNumber;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CNumber::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);

	//rhwの設定		=	1.0f固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定・赤・緑・青
	pVtx[0].col = WhiteColor;
	pVtx[1].col = WhiteColor;
	pVtx[2].col = WhiteColor;
	pVtx[3].col = WhiteColor;

	SetNumber(0);

	//バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{//頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Unload();
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// 描画設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 番号の設定
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2((((float)nNumber + 0.0f) / m_fSplit), 1.0f);
	pVtx[1].tex = D3DXVECTOR2((((float)nNumber + 0.0f) / m_fSplit), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((((float)nNumber + 1.0f) / m_fSplit), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((((float)nNumber + 1.0f) / m_fSplit), 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ナンバーのアンロード
//=============================================================================
void CNumber::Unload()
{
	if (m_pTexture != NULL)
	{//頂点バッファの開放
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ナンバーのロード
//=============================================================================
HRESULT CNumber::Load(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTexture);

	return S_OK;
}

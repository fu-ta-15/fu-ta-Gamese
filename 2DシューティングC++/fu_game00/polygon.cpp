//*****************************************************************************
//
// シーン処理 [scene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "polygon.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon::CPolygon()
{
	m_pos = ZeroVector3;
	m_rot = ZeroVector3;
	m_size = ZeroVector3;
	m_move = ZeroVector3;
	m_col = WhiteColor;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon::~CPolygon()
{
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon * CPolygon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CPolygon *pPolygon = new CPolygon;

	pPolygon->SetPos(pos);
	pPolygon->SetSize(size);
	pPolygon->Init();

	return nullptr;
}

//=============================================================================
// コンストラクタ
//=============================================================================
HRESULT CPolygon::CreateTexture(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTex);

	return S_OK;
}

//=============================================================================
// コンストラクタ
//=============================================================================
HRESULT CPolygon::Init(void)
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

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{//頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::Update(void)
{
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTex);

	// 描画設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::SetCol(const D3DXCOLOR col)
{
	m_col = col;
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定・赤・緑・青
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// コンストラクタ
//=============================================================================
void CPolygon::SetTex(const D3DXVECTOR2 tex, const D3DXVECTOR2 fnumber)
{
	m_fAnimeX = fnumber.x;
	m_fAnimeY = fnumber.y;
	m_tex = tex;

	VERTEX_2D *pVtx;

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(((m_fAnimeX + 0.0f) / m_tex.x), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(((m_fAnimeX + 0.0f) / m_tex.x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(((m_fAnimeX + 1.0f) / m_tex.x), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((m_fAnimeX + 1.0f) / m_tex.x), 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// コンストラクタ
//=============================================================================
D3DXVECTOR3 CPolygon::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// コンストラクタ
//=============================================================================
D3DCOLOR CPolygon::GetCol(void)
{
	return m_col;
}

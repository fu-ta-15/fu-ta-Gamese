//*****************************************************************************
//
// シーン2D処理 [scene2D.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "keyinput.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D()
{
}

//=============================================================================
// シーンオブジェクトのコンストラクタ
//=============================================================================
CScene2D::CScene2D(ObjType type) : CScene(type)
{
	m_pos = ZeroVector3;
	m_rot = ZeroVector3;
	m_size = ZeroVector3;
	m_move = ZeroVector3;
	m_col = WhiteColor;
	m_bUse = true;
	m_fAngle = 0.0f;
}

//=============================================================================
// ポーズオブジェクトのコンストラクタ
//=============================================================================
CScene2D::CScene2D(PauseType type) : CScene(type)
{
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{
}

//=============================================================================
// ポリゴンの生成
//=============================================================================
CScene2D * CScene2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CScene2D *pScene2D = new CScene2D(OBJ_NONE);

	pScene2D->Init(pos, size);

	return pScene2D;
}

HRESULT CScene2D::CreateTexture(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTex);

	return S_OK;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// ポリゴンのメンバ変数の値代入
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(45.0f, 45.0f, 0.0f);
	m_fAngle = atan2f(m_size.x, m_size.y);

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
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fAngle, m_pos.y + m_fAngle, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fAngle, m_pos.y - m_fAngle, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_fAngle, m_pos.y + m_fAngle, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fAngle, m_pos.y - m_fAngle, 0.0f);

	//rhwの設定		=	1.0f固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定・赤・緑・青
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
// 初期化（位置とサイズ指定）
//=============================================================================
HRESULT CScene2D::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;

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
// アニメーション付き初期化（位置とサイズ指定）
//=============================================================================
HRESULT CScene2D::AnimationInit(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR2 tex)
{
	m_pos = pos;
	m_size = size;
	m_fAnimeX = tex.x;
	m_fAnimeY = tex.y;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(CREATE_POLYGON, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
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
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(((m_fAnimeX + 0.0f) / m_tex.x), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(((m_fAnimeX + 0.0f) / m_tex.x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(((m_fAnimeX + 1.0f) / m_tex.x), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((m_fAnimeX + 1.0f) / m_tex.x), 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{//頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	Release();
}

//=============================================================================
// pauseの終了処理
//=============================================================================
void CScene2D::Uninit(bool bPause)
{
	if (bPause == true)
	{
		if (m_pVtxBuff != NULL)
		{//頂点バッファの開放
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bUse == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTex);

		// 描画設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// 位置の設定
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
	CScene::SetPos(m_pos);
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
// サイズの設定
//=============================================================================
void CScene2D::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
	CScene::SetSize(m_size);
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
// 色の設定
//=============================================================================
void CScene2D::SetCol(const D3DXCOLOR col)
{
	m_col = col;
	CScene::SetCol(m_col);
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
// 使用中か否か
//=============================================================================
void CScene2D::SetUse(const bool bUse)
{
	m_bUse = bUse;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CScene2D::SetTexture(const LPDIRECT3DTEXTURE9 pTex)
{
	m_pTex = pTex;
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void CScene2D::SetTex(const D3DXVECTOR2 tex, const D3DXVECTOR2 fnumber)
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
// 位置の取得
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 色の取得
//=============================================================================
D3DCOLOR CScene2D::GetCol(void)
{
	return m_col;
}


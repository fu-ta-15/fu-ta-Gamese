//=============================================================================
//
// シーンビルボード処理 [sceneBillboard.cpp]
// Author : 
//
//=============================================================================
#include "sceneBillboard.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_X		(1)							// アニメーションパターンNo.の横の最大数
#define TEXTURE_Y		(1)							// アニメーションパターンNo.の縦の最大数

//=============================================================================
// CSceneBillboardのコンストラクタ
//=============================================================================
CSceneBillboard::CSceneBillboard(int nPriority) : CScene(nPriority)
{
	// メンバ変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
// CSceneBillboardのデストラクタ
//=============================================================================
CSceneBillboard::~CSceneBillboard()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CSceneBillboard *CSceneBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot)
{
	// ポインタ変数の生成
	CSceneBillboard *pBillboard;
	pBillboard = new CSceneBillboard(CScene::PRIORITY_FLOOR);

	// NULLチェック
	if (pBillboard != NULL)
	{
		// 初期化処理
		pBillboard->Init(pos, size);

		// オブジェクトタイプの設定
		pBillboard->SetObjType(OBJTYPE_FLOOR);
	}

	return pBillboard;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数の初期化
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = size.x / 2.0f;
	m_fHeight = size.y / 2.0f;

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX,	// 確保するバッファサイズ
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

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - m_fWidth, pos.y + m_fHeight, pos.z);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_fWidth, pos.y + m_fHeight, pos.z);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_fWidth, pos.y - m_fHeight, pos.z);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_fWidth, pos.y - m_fHeight, pos.z);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標
	pVtx[0].tex1 = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex1 = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex1 = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex1 = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneBillboard::Uninit(void)
{
	if (m_bDeath == true)
	{
		// 頂点バッファの開放
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneBillboard::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneBillboard::Draw(void)
{
	// デバイスの取得
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxView;				// ビューマトリックス取得用

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 1
	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;
#else
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;
#endif

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->SetTexture(0, m_pTexture);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// テクスチャ情報の割り当て
//=============================================================================
void CSceneBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// テクスチャ情報を割り当てる
	m_pTexture = pTexture;
}

//=============================================================================
// 座標設定処理
//=============================================================================
void CSceneBillboard::SetPosition(D3DXVECTOR3 pos)
{
	// 座標設定
	m_pos = pos;

	// 頂点座標の設定
	SetVertex();
}

//=============================================================================
// サイズ設定処理
//=============================================================================
void CSceneBillboard::SetSize(D3DXVECTOR2 size)
{
	// 座標設定
	m_fWidth = size.x;
	m_fHeight = size.y;

	// 頂点座標の設定
	SetVertex();
}

//=============================================================================
// 頂点座標の設定処理
//=============================================================================
void CSceneBillboard::SetVertex(void)
{
	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の更新
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点カラーの設定処理
//=============================================================================
void CSceneBillboard::SetCol(D3DXCOLOR col)
{
	// 変数の更新
	m_col = col;

	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の更新
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定処理
//=============================================================================
void CSceneBillboard::SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets)
{
	// 構造体のポインタ変数
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の更新
	pVtx[0].tex1 = D3DXVECTOR2(texUV.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[1].tex1 = D3DXVECTOR2(texUV.x, texUV.y);
	pVtx[2].tex1 = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[3].tex1 = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
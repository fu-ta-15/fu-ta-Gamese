//=============================================================================
//
// 消しくず 処理[eraserdust.cpp]
// Author : SUZUKI FUUTA
//
//=============================================================================
#include "shadowS.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CShadowS::CShadowS(int nPriority) : CSceneX(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CShadowS::~CShadowS()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CShadowS * CShadowS::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, char *pModelFileName)
{
	// 変数宣言
	CShadowS *pShadows = NULL;
	pShadows = new CShadowS(PRIORITY_SHADOW);

	if (pShadows != NULL)
	{
		// モデルファイルネームの設定
		pShadows->BindXFileName(pModelFileName);

		// 回転の設定
		pShadows->SetRotate(rot);

		// 初期化
		pShadows->Init(pos, size);

		// オブジェクトの種類を設定
		pShadows->SetObjType(OBJTYPE_SHADOW);
	}

	return pShadows;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadowS::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CSceneXの初期化処理
	CSceneX::Init(pos, size);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + SCREEN_HEIGHT / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - SCREEN_HEIGHT / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + SCREEN_HEIGHT / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - SCREEN_HEIGHT / 2.0f, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadowS::Uninit(void)
{
	// CSceneXの終了処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadowS::Update(void)
{
	// CSceneXの更新処理
	CSceneX::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadowS::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ステンシルテストを有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// Zバッファへの書き込みを無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// カラーバッファへの書き込みを無効
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCRSAT);

	// 表面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// CSceneXの描画処理
	CSceneX::Draw();

	// ステンシルバッファの参照値を設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);

	// 裏面をカリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// CSceneXの描画処理
	CSceneX::Draw();

	// カラーバッファへの書き込みを有効
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// ステンシルバッファの参照値を設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 設定を戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shadow.h"

#include "debugproc.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// CModelのコンストラクタ
//=============================================================================
CModel::CModel()
{
	// 変数の初期化
}

//=============================================================================
// CModelのデストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pModelFileName)
{
	// ポインタ変数の生成
	CModel *pModel;
	pModel = new CModel;

	// NULLチェック
	if (pModel != NULL)
	{
		// 初期化処理
		pModel->Init(pos, rot,pModelFileName);
	}

	return pModel;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pModelFileName)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し、初期化
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ管理情報の取得
	CTexture *pTexture = CManager::GetTexture();

	// 情報の初期化
	m_pMesh = NULL;
	m_pBuffMat = NULL;
	m_nNumMat = NULL;
	m_pos = pos;
	m_rot = rot;

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(pModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh)))
	{
		return E_FAIL;
	}

	// 変数宣言
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// メモリの確保
	m_pTex = new int[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			if (pTexture != NULL)
			{
				m_pTex[nCntMat] = pTexture->SetTexture(pMat[nCntMat].pTextureFilename);
			}
		}
		else
		{
			m_pTex[nCntMat] = -1;
		}
	}

	// 影情報の生成
	//m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f }, {0.0f,0.0f});

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// メッシュ情報の開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// マテリアル情報の開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;			// 親モデルのマトリックス

	// 変数の受け渡し
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ管理情報の取得
	CTexture *pTexture = CManager::GetTexture();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{
		// 親がいるとき親モデルのマトリックスを取得する
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		// 親がいないとき最新のマトリックス情報を取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	// 親のマトリックスをかけあわせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_pTex[nCntMat] != -1)
		{
			if (pTexture != NULL)
			{
				pDevice->SetTexture(0, pTexture->GetAddress(m_pTex[nCntMat]));
			}
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
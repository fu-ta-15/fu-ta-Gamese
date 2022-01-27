//=============================================================================
//
// 消しゴム処理 [eraser.cpp]
// Author : 
//
//=============================================================================
#include "eraser.h"
#include "scene.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input_keyboard.h"
#include "player.h"
#include "stage.h"
#include "skill.h"
#include "keep_object.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// CWoodのコンストラクタ
//=============================================================================
CEraser::CEraser(int nPriority) : CScene(nPriority)
{

}

//=============================================================================
// CEraserのデストラクタ
//=============================================================================
CEraser::~CEraser()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEraser *CEraser::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CEraser *pEraser;
	pEraser = new CEraser(CScene::PRIORITY_ERASER);

	// NULLチェック
	if (pEraser != NULL)
	{
		// 初期化処理
		pEraser->Init(pos, size);

		// オブジェクトタイプの設定
		pEraser->SetObjType(OBJTYPE_ERASER);
	}

	return pEraser;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEraser::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = D3DXVECTOR3(size.x, size.y, size.x);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_eraserType = ERASERTYPE_NORMAL;
	m_nNumOffset = 0;
	m_bColStage = false;

	// メモリの確保
	m_pOffsetPos = new D3DXVECTOR3 *[CPlayer::MOTIONTYPE_MAX];
	m_pOffsetRot = new D3DXVECTOR3 *[CPlayer::MOTIONTYPE_MAX];

	for (int nCnt = 0; nCnt < CPlayer::MOTIONTYPE_MAX; nCnt++)
	{
		m_pOffsetPos[nCnt] = new D3DXVECTOR3[ERASERTYPE_MAX];
		m_pOffsetRot[nCnt] = new D3DXVECTOR3[ERASERTYPE_MAX];
	}

	for (int nCntPattern = 0; nCntPattern < CPlayer::MOTIONTYPE_MAX; nCntPattern++)
	{
		for (int nCntType = 0; nCntType < ERASERTYPE_MAX; nCntType++)
		{
			m_pOffsetPos[nCntPattern][nCntType] = { 0.0f,0.0f,0.0f };
			m_pOffsetRot[nCntPattern][nCntType] = { 0.0f,0.0f,0.0f };
		}
	}

	// モデルの読み込み
	for (int nCntModel = 0; nCntModel < 1; nCntModel++)
	{
		m_pModel = CModel::Create(D3DXVECTOR3(m_pOffsetPos[m_nNumOffset][m_eraserType].x, m_pOffsetPos[m_nNumOffset][m_eraserType].y, m_pOffsetPos[m_nNumOffset][m_eraserType].z),
			D3DXVECTOR3(m_pOffsetRot[m_nNumOffset][m_eraserType].x, m_pOffsetRot[m_nNumOffset][m_eraserType].y, m_pOffsetRot[m_nNumOffset][m_eraserType].z),
			"data/MODEL/eraser.x");
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEraser::Uninit(void)
{
	// メモリの開放
	delete[] m_pOffsetPos;
	m_pOffsetPos = NULL;

	delete[] m_pOffsetRot;
	m_pOffsetRot = NULL;

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEraser::Update(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void CEraser::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 変数の受け渡し
	pDevice = CManager::GetRenderer()->GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		// パーツの描画
		m_pModel->Draw();
	}
}

//=============================================================================
// 消しゴムタイプの切り替え処理
//=============================================================================
int CEraser::SwicthType(void)
{
	switch (m_eraserType)
	{
	case ERASERTYPE_NORMAL:
		m_eraserType = ERASERTYPE_SAND;
		break;

	case ERASERTYPE_SAND:
		m_eraserType = ERASERTYPE_NORMAL;
		break;
	}

	// オフセット設定
	SetOffset();

	return (int)m_eraserType;
}

//=============================================================================
// オフセットの変更
//=============================================================================
void CEraser::SetOffset(int nNum)
{
	// 何番目のオフセットを使うか保存
	m_nNumOffset = nNum;

	// オフセット設定
	SetOffset();
}

//=============================================================================
// オフセットの設定
//=============================================================================
void CEraser::SetOffset(void)
{
	// オフセットの位置
	m_pModel->SetPosition(D3DXVECTOR3(m_pOffsetPos[m_nNumOffset][m_eraserType].x,
		m_pOffsetPos[m_nNumOffset][m_eraserType].y,
		m_pOffsetPos[m_nNumOffset][m_eraserType].z));

	// オフセットの向き
	m_pModel->SetRot(D3DXVECTOR3(m_pOffsetRot[m_nNumOffset][m_eraserType].x,
		m_pOffsetRot[m_nNumOffset][m_eraserType].y,
		m_pOffsetRot[m_nNumOffset][m_eraserType].z));
}

//=============================================================================
// オフセットの設定
//=============================================================================
void CEraser::InitOffset(int nType,int nCntPattern, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// オフセットの設定
	m_pOffsetPos[nCntPattern][nType] = pos;
	m_pOffsetRot[nCntPattern][nType] = rot;
}

//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CEraser::GetPosition(void)
{
	// マトリックス情報の取得
	D3DXMATRIX mtx = m_pModel->GetMtxWorld();

	// 位置情報を返す
	return (D3DXVECTOR3(mtx._41,mtx._42,mtx._43));
}

//=============================================================================
// ステージとの当たり判定
//=============================================================================
bool CEraser::CollisionStage(int nRank)
{
	// 床情報の取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_MODEL);

	// NULLチェック
	if (pScene != NULL)
	{
		while (pScene)
		{
			// 次の情報を保存
			CScene *pSceneNext = pScene->GetNext();

			// オブジェクトの種類が床だったら床にキャスト
			if (pScene->GetObjType() == CScene::OBJTYPE_STAGE)
			{
				CStage *pStage = (CStage*)pScene;

				// 位置の取得
				D3DXVECTOR3 pos = GetPosition();
				pos.y -= 24.0f;

				// 当たり判定
				if (pStage->Collision(&pos, GetSize()))
				{
					// 移動量の初期化
					m_move.y = 0.0f;

					if (m_bColStage == false)
					{
						// スキル範囲
						CSkill::Create(D3DXVECTOR3(pos.x,pos.y + 1.0f,pos.z), D3DXVECTOR2(180.0f, 180.0f));

						m_bColStage = true;
					}

					return true;
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
	return false;
}
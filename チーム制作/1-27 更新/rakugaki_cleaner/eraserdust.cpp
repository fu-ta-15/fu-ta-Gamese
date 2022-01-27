//#############################################################################
//
// 消しくず 処理[eraserdust.cpp]
// Author : SUZUKI FUUTA
//
//#############################################################################

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "eraserdust.h"
#include "player.h"
#include "game.h"
#include "stage.h"
#include "skill.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEraserDust::CEraserDust(int nPriority) : CSceneX(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CEraserDust::~CEraserDust()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CEraserDust * CEraserDust::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR2 & size, D3DXVECTOR3 rot, char *pModelFileName)
{
	// 変数宣言
	CEraserDust *pEraserDust = NULL; 
	pEraserDust = new CEraserDust(PRIORITY_MODEL);

	if (pEraserDust != NULL)
	{
		// モデルファイルネームの設定
		pEraserDust->BindXFileName(pModelFileName);

		// 回転の設定
		pEraserDust->SetRotate(rot);

		// 初期化
		pEraserDust->Init(pos, size);

		// オブジェクトの種類を設定
		pEraserDust->SetObjType(OBJTYPE_ERASERDUST);
	}

	return pEraserDust;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEraserDust::Init(const D3DXVECTOR3 & pos, const D3DXVECTOR2 & size)
{
	// CSceneXの初期化処理
	CSceneX::Init(pos, size);

	// 位置の保存
	m_pos = pos;

	// 回転の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf(GetVtxMax().x * GetVtxMax().x + GetVtxMax().z *GetVtxMax().z);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEraserDust::Uninit(void)
{
	// CSceneXの終了処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEraserDust::Update(void)
{
	// CSceneXの更新処理
	CSceneX::Update();

	// 重力
	m_move.y -= 0.3f;

	// 慣性
	m_move.x += (0.0f - m_move.x) * 0.08f;
	m_move.z += (0.0f - m_move.z) * 0.08f;

	// 位置を取得
	D3DXVECTOR3 pos = GetPosition();

	// 移動量を位置に加算
	pos += m_move;

	// 位置の設定
	SetPosition(pos);

	// ステージとの当たり判定
	if (CollisionStage() == false)
	{
		return;
	}

	// スキルとの当たり判定
	CollisionSkill();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEraserDust::Draw(void)
{
	// CSceneXの描画処理
	CSceneX::Draw();
}

//=============================================================================
// 消しカスの当たり判定
//=============================================================================
bool CEraserDust::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 位置取得
	D3DXVECTOR3 posEraserDust = GetPosition();

	// 消しカスの範囲に当たっていたら
	if (posEraserDust.x - CSceneX::GetVtxMin().x < pos.x &&
		posEraserDust.x + CSceneX::GetVtxMax().x > pos.x &&
		posEraserDust.z - CSceneX::GetVtxMin().z - 5.0f < pos.z &&
		posEraserDust.z + CSceneX::GetVtxMax().z + 5.0f > pos.z)
	{
		// 距離を求める
		D3DXVECTOR3 vec = posEraserDust - pos;
		float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

		// ある距離よ短いとき当たり判定を取る
		// 移動量を求める
		float fRate = 1.0f - (fLength / m_fLength);
		/*posEraserDust.x += vec.x * fRate;
		posEraserDust.z += vec.z * fRate;
		m_rot.y += cosf(fLength / m_fLength);*/

		// 位置をずらす
		posEraserDust.x += 1.0f * sinf(rot.y);
		posEraserDust.z += 1.0f * cosf(rot.y);

		// プレイヤーの向きで自身の回転に影響する
		m_rot.y = cosf(rot.y*1.5f);

		// 元クラスに情報を与える
		CSceneX::SetRotate(m_rot);
		CSceneX::SetPosition(posEraserDust);

		return true;
	}
	return false;
}

//=============================================================================
// スキルとの当たり判定
//=============================================================================
void CEraserDust::CollisionSkill(void)
{
	// 床情報の取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_UI);

	// NULLチェック
	if (pScene != NULL)
	{
		while (pScene)
		{
			// 次の情報を保存
			CScene *pSceneNext = pScene->GetNext();

			// オブジェクトの種類が床だったら床にキャスト
			if (pScene->GetObjType() == CScene::OBJTYPE_SKILL)
			{
				CSkill *pSkill = (CSkill*)pScene;

				// 位置の取得
				D3DXVECTOR3 pos = GetPosition();

				// 当たり判定
				pSkill->Collision(pos, GetVtxMax(), &m_move,D3DXVECTOR3(8.0f,10.0f, 8.0f));
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// ステージとの当たり判定
//=============================================================================
bool CEraserDust::CollisionStage(void)
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

				// 当たり判定
				if (pStage->CollisionUp(&pos, GetVtxMax()))
				{
					// 移動量の初期化
					m_move.y = 0.0f;

					// 位置の設定
					SetPosition(pos);

					return true;
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
	return false;
}

//=============================================================================
//
// ゲージ管理処理 [gauge_manager.cpp]
// Author : 
//
//=============================================================================
#include "gauge_manager.h"

#include "manager.h"
#include "base.h"
#include "game.h"
#include "player_game.h"
#include "gauge.h"
#include "scoregauge.h"
#include "skillgauge.h"
#include "graffiti_gauge.h"
#include "graffiti_manager.h"
#include "keep_object.h"
#include "ranking_namespace.h"

#include "player_marker.h"
#include "crown.h"
#include "texture.h"
#include "renderer.h"

//=============================================================================
// CGaugeManagerのコンストラクタ
//=============================================================================
CGaugeManager::CGaugeManager()
{
	// メンバ変数の初期化
	m_nNumPlayer = 0;
}

//=============================================================================
// CGaugeManagerのデストラクタ
//=============================================================================
CGaugeManager::~CGaugeManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGaugeManager::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_nCntRank_One = 0;
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// プレイヤー数の保存
		m_nNumPlayer = pGame->GetNumPlayer();
	}

	// 王冠の初期化
	CTexture *pTex = CManager::GetTexture();

	m_pCrown = new CCrown*[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_pCrown[nCnt] = CCrown::Create({ 0.0f,0.0f,0.0f }, { 50.0f,50.0f }, pTex->SetTexture("data/TEXTURE/6912.png"));
	}

	// プレイヤーマーカーの初期化
	m_pPlayerMarker = new CPlayerMarker*[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_pPlayerMarker[nCnt] = CPlayerMarker::Create({ 0.0f,0.0f,0.0f }, { 20.0f,20.0f }, {4,1},pTex->SetTexture("data/TEXTURE/player_Logo00.png"));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGaugeManager::Uninit(void)
{
	// ゲーム情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		// 保存
		CGame *pGame = (CGame*)pBase;
		CKeepObject *pKeepObject = CManager::GetKeepObject();

		// ラクガキ管理情報を取得
		CGraffitiManager *pGraffiti = pGame->GetGraffitiManager();

		// 達成率の保存
		int nNumTotalGraf = pGraffiti->GetTotalGraffiti();
		int nNumCurGraf = pGraffiti->GetCurGraffiti();
		pKeepObject->SetAchivement((float)(nNumTotalGraf - nNumCurGraf) / (float)nNumTotalGraf);
		pKeepObject->SetTotalGraffiti(nNumTotalGraf);
	}

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGaugeManager::Update(void)
{
	// 初期化
	m_nCntRank_One = 0;

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_pCrown[nCnt]->SetDisplay(false);
	}

	// スコアゲージ処理
	ScoreGauge();

	// スキルゲージ処理
	SkillGauge();

	// ラクガキゲージ処理
	GraffitiGauge();

	// ゲーム情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// プレイヤー情報を取得
		CPlayer_Game **pPlayer = pGame->GetPlayer();

		// ゲージを増やす
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// スコアゲージ情報を取得
				CScoreGauge *pScoreGauge = pPlayer[nCnt]->GetScoreGauge();

				// 位置とサイズ情報の取得
				D3DXVECTOR3 posScore = pScoreGauge->GetPosDef();
				D3DXVECTOR2 sizeScore = pScoreGauge->GetSizeDef();

				// 位置をサイズ分引く
				posScore.x -= sizeScore.x / 2.0f;

				// 位置の変更
				m_pPlayerMarker[nCnt]->SetPosition(D3DXVECTOR3(posScore.x + 35.0f, posScore.y - 20.0f, 0.0f));
				m_pPlayerMarker[nCnt]->SetTex({ (float)nCnt,0 });
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGaugeManager::Draw(void)
{	

}

//=============================================================================
// スコアゲージ処理
//=============================================================================
void CGaugeManager::ScoreGauge(void)
{
	// 変数宣言
	int nNumDeleteGraffiti[4] = {};
	int nNumMax = 0;					// 一番大きい値のプレイヤー番号格納用

	// ゲーム情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// プレイヤー情報を取得
		CPlayer_Game **pPlayer = pGame->GetPlayer();

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// 消したラクガキ数を取得
				nNumDeleteGraffiti[nCnt] = pPlayer[nCnt]->GetDeleteGraffiti();
			}
		}

		// 順位確定
		int *nNumberRank = new int[m_nNumPlayer];

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			nNumberRank[nCnt] = 0;
		}
		RankingSpace::SetRanking(&nNumberRank[0], &nNumDeleteGraffiti[0], m_nNumPlayer);

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			pPlayer[nCnt]->SetRank(nNumberRank[nCnt]);
		}

		// 値を比較
		for (int nCnt = 0; nCnt < m_nNumPlayer - 1; nCnt++)
		{
			for (int nCntSub = nCnt + 1; nCntSub < m_nNumPlayer; nCntSub++)
			{
				if (nNumDeleteGraffiti[nCnt] < nNumDeleteGraffiti[nCntSub])
				{
					int nSub = nNumDeleteGraffiti[nCnt];
					nNumDeleteGraffiti[nCnt] = nNumDeleteGraffiti[nCntSub];
					nNumDeleteGraffiti[nCntSub] = nSub;
				}
			}
		}

		// ゲージを増やす
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// スコアゲージ情報を取得
				CScoreGauge *pScoreGauge = pPlayer[nCnt]->GetScoreGauge();

				// 位置とサイズ情報の取得
				D3DXVECTOR3 posScore = pScoreGauge->GetPosDef();
				D3DXVECTOR2 sizeScore = pScoreGauge->GetSizeDef();

				// 位置をサイズ分引く
				posScore.x -= sizeScore.x / 2.0f;

				// 消したラクガキ数が0じゃないなら処理
				if (pPlayer[nCnt]->GetDeleteGraffiti() != 0)
				{
					if (nNumDeleteGraffiti[0] == pPlayer[nCnt]->GetDeleteGraffiti())
					{
						// 一番消してる場合
						pScoreGauge->AddPosTex(D3DXVECTOR3(posScore.x + sizeScore.x, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), 2, 3);
						pScoreGauge->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

						// 位置の変更
						m_pCrown[m_nCntRank_One]->SetPosition(D3DXVECTOR3(posScore.x + 150.0f, posScore.y - 30.0f,0.0f));
						m_pCrown[m_nCntRank_One]->SetDisplay(true);

						// カウント
						m_nCntRank_One++;
					}
					else
					{
						// 割合を求める
						float fRatio = (float)pPlayer[nCnt]->GetDeleteGraffiti() / (float)nNumDeleteGraffiti[0];

						// 二位以下の場合
						pScoreGauge->AddPosTex(D3DXVECTOR3(posScore.x + sizeScore.x * fRatio, 0.0f, 0.0f), D3DXVECTOR2(fRatio, 0.0f), 2, 3);
						pScoreGauge->SetCol(D3DXCOLOR(1.0f, 0.55f, 0.15f, 1.0f));
					}
				}
			}
		}
	}
}

//=============================================================================
// スキルゲージ処理
//=============================================================================
void CGaugeManager::SkillGauge(void)
{
	// 変数宣言
	int nNumDeleteGraffiti[4] = {};

	// ゲーム情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// プレイヤー情報を取得
		CPlayer_Game **pPlayer = pGame->GetPlayer();

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// 消したラクガキ数を取得
				nNumDeleteGraffiti[nCnt] = pPlayer[nCnt]->GetDeleteGraffiti();
			}
		}

		// ゲージを増やす
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// スキルゲージ
				CSkillGauge *pSkillGauge = pPlayer[nCnt]->GetSkillGauge();

				// 位置とサイズ情報の取得
				D3DXVECTOR3 posSkill = pSkillGauge->GetPosDef();
				D3DXVECTOR2 sizeSkill = pSkillGauge->GetSizeDef();

				// 位置をサイズ分引く
				posSkill.x -= sizeSkill.x / 2.0f;

				// 消したラクガキ数が0なら処理
				if (pPlayer[nCnt]->GetCntSkillGauge() == 0)
				{
					// ゲージを伸ばす
					pSkillGauge->AddPosTex(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 2, 3);

					// 色の変更
					pSkillGauge->SetCol(D3DXCOLOR(1.0f, 0.55f, 0.15f, 1.0f));
				}
				else if (pPlayer[nCnt]->GetMaxSkillGauge() == false)
				{
					// 割合を求める
					float fRatio = (float)pPlayer[nCnt]->GetCntSkillGauge() / (float)DELETE_GARAFFITI_NUMBER_MAX;

					// ゲージを伸ばす
					pSkillGauge->AddPosTex(D3DXVECTOR3(posSkill.x + sizeSkill.x * fRatio, 0.0f, 0.0f), D3DXVECTOR2(fRatio, 0.0f), 2, 3);

					if (pPlayer[nCnt]->GetCntSkillGauge() == DELETE_GARAFFITI_NUMBER_MAX)
					{
						// 色の変更
						pSkillGauge->SetCol(D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f));

						// Maxになったことを伝える
						pPlayer[nCnt]->SetMaxSkillGauge(true);
					}
				}
			}
		}
	}
}

//=============================================================================
// ラクガキゲージ処理
//=============================================================================
void CGaugeManager::GraffitiGauge(void)
{
	// ゲーム情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// ラクガキ管理情報を取得
		CGraffitiManager *pGraffiti = pGame->GetGraffitiManager();

		if (pGraffiti != NULL)
		{
			// 総数と現在の数を取得
			int nNumTotalGraf = pGraffiti->GetTotalGraffiti();
			int nNumCurGraf = pGraffiti->GetCurGraffiti();

			// 総数と現在の数を参照し割合を求める
			float fRatio = 0.0f;

			if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_COOPERATION)
			{
				fRatio = (float)(nNumTotalGraf - nNumCurGraf) / (float)nNumTotalGraf;
			}
			else if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_BATTLE)
			{
				fRatio = (float)(nNumCurGraf) / (float)nNumTotalGraf;
			}

			// 位置とサイズ情報の取得
			D3DXVECTOR3 posGraf = m_pGraffitiGauge->GetPosDef();
			D3DXVECTOR2 sizeGraf = m_pGraffitiGauge->GetSizeDef();

			// 位置にサイズ分減算
			posGraf.x -= sizeGraf.x / 2.0f;

			// 右端のX座標
			float fPosX = posGraf.x + sizeGraf.x * fRatio;

			// ゲージを減らす
			m_pGraffitiGauge->AddPosTex(D3DXVECTOR3(fPosX,0.0f,0.0f), D3DXVECTOR2(0,0), 2, 3);

			// 割合と位置を保存
			m_pGraffitiGauge->SetRatio(fRatio);
			m_pGraffitiGauge->SetPosXRightSide(fPosX);
		}
	}
}
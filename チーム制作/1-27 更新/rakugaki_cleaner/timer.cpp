//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "timer.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "base.h"
#include "game.h"
#include "countdown.h"
#include "sound.h"

//=============================================================================
// CTimerのコンストラクタ
//=============================================================================
CTimer::CTimer(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	for (int nCntNumber = 0; nCntNumber < MAX_DIGIT; nCntNumber++)
	{
		m_apNumber[nCntNumber] = NULL;
	}
	m_nCntInterval = 0;
	m_nTimer = 0;
	m_fSpace = 0.0f;
}

//=============================================================================
// CTimerのデストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nDigit, float fSpace,int nTimer,int nTex)
{
	// ポインタ変数の生成
	CTimer *pTimer;
	pTimer = new CTimer(CScene::PRIORITY_UI);

	// NULLチェック
	if (pTimer != NULL)
	{
		// 桁数の設定
		pTimer->SetDigit(nDigit);

		// テクスチャ番号の設定
		pTimer->SetTexNum(nTex);

		// タイマー配置間隔を保存
		pTimer->SetSpace(fSpace);

		// タイマーの設定
		pTimer->SetTimer(nTimer);

		// 初期化処理
		pTimer->Init(pos, size);

		// オブジェクトタイプの設定
		pTimer->SetObjType(OBJTYPE_TIMER);
	}

	return pTimer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ナンバーの初期化処理
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(pos.x + m_fSpace * nCntNumber, pos.y, 0.0f), size);

		// テクスチャ番号の設定
		m_apNumber[nCntNumber]->SetTexNum(m_nTex);
	}

	// 描画更新
	UpdateDraw();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// ナンバーの終了処理
		m_apNumber[nCntNumber]->Uninit();
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTimer::Update(void)
{
	// カウントダウン情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		CCountdown *pCountdown = pGame->GetCountdwn();

		if (pCountdown != NULL && pCountdown->GetFinish())
		{
			for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
			{
				// ナンバーの更新処理
				m_apNumber[nCntNumber]->Update();
			}

			// カウントの更新
			m_nCntInterval++;

			if (m_nTimer == 0 || pGame->GetGameMode() == CGame::GAMEMODE_FINISH)
			{
				// 点滅処理
				FlashTimer();

				// ゲームモードを終了にする
				pGame->SetGameMode(CGame::GAMEMODE_FINISH);
			}
			else
			{
				// 60になったらタイマーを減らす
				if (m_nCntInterval % 60 == 0)
				{
					m_nTimer--;
					m_nCntInterval = 0;

					if (m_nTimer == 0)
					{
						// サウンド終了
						CSound::Stop();

						// 効果音
						CSound::Play(CSound::SOUND_LABEL_SE_TIMEUP);
					}
				}

				// 描画更新
				UpdateDraw();
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// ナンバーの描画処理
		m_apNumber[nCntNumber]->Draw();
	}
}

//=============================================================================
// 描画更新処理
//=============================================================================
void CTimer::UpdateDraw(void)
{
	// タイマーを格納
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		// 桁を一つずつ格納
		int nNumber = (m_nTimer / (int)powf(10.0f, (float)(m_nDigit - (nCntNumber + 1))));

		// ナンバーセット
		m_apNumber[nCntNumber]->SetNumber(nNumber);
	}
}

//=============================================================================
// タイマーの点滅処理
//=============================================================================
void CTimer::FlashTimer(void)
{
	// カウントに応じてα値を変える
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		if (m_nCntInterval % 20 == 10)
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else if (m_nCntInterval % 20 == 0)
		{
			m_apNumber[nCntNumber]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
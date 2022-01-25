//=============================================================================
//
// プレイヤー待機画面管理処理 [preparation_player_manager.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "preparation_player_manager.h"

#include "manager.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "base.h"
#include "preparation_player.h"
#include "set_preparation_player.h"
#include "press_logo.h"
#include "Scene2D.h"
#include "keep_object.h"
#include "texture.h"
#include "debugproc.h"
#include "sound.h"

//=============================================================================
// CPreparationPlayerManagerのコンストラクタ
//=============================================================================
CPreparationPlayerManager::CPreparationPlayerManager()
{
	// メンバ変数の初期化
	m_pPressLogo = NULL;
	m_bPlayerReadyOK = false;
}

//=============================================================================
// CPreparationPlayerManagerのデストラクタ
//=============================================================================
CPreparationPlayerManager::~CPreparationPlayerManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPreparationPlayerManager::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_pPressLogo = NULL;
	m_bPlayerReadyOK = false;
	m_bOK = false;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_logoInfo[nCnt].bJoin = false;
		m_logoInfo[nCnt].bReady = false;

		m_pJoinLogo[nCnt] = NULL;
		m_pCompLogo[nCnt] = NULL;
		m_pOkLogo[nCnt] = NULL;
		m_pFrame[nCnt] = NULL;
	}

	// プレイヤー準備画面情報の読み込み
	CSetPreparationPlayer *pSet = new CSetPreparationPlayer;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// キャラ選択初期化処理
	InitCharaSelect();

	return S_OK;  
}

//=============================================================================
// 終了処理
//=============================================================================
void CPreparationPlayerManager::Uninit(void)
{
	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPreparationPlayerManager::Update(void)
{
	// キャラ選択
	CharaSelect();

	// 遷移処理
	Transition();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPreparationPlayerManager::Draw(void)
{
	// 各UIの描画処理
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_logoInfo[nCnt].bJoin == false)
		{
			if (m_pJoinLogo[nCnt] != NULL)
			{
				m_pJoinLogo[nCnt]->Draw();
			}
		}
		else if (m_logoInfo[nCnt].bJoin == true && m_logoInfo[nCnt].bReady == false)
		{
			if (m_pCompLogo[nCnt] != NULL)
			{
				m_pCompLogo[nCnt]->Draw();
			}
		}
		else if (m_logoInfo[nCnt].bJoin == true && m_logoInfo[nCnt].bReady == true)
		{
			if (m_pOkLogo[nCnt] != NULL)
			{
				m_pOkLogo[nCnt]->Draw();
			}
		}
	}
}

//=============================================================================
// 遷移処理
//=============================================================================
void CPreparationPlayerManager::Transition(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (m_bPlayerReadyOK == false)
	{
		// 全員が準備完了していないとき処理
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (m_logoInfo[nCnt].bJoin == true)
			{
				// 参加しているかどうか
				if (m_logoInfo[nCnt].bReady != true)
				{
					// 準備完了していないならロゴを表示させない
					m_bPlayerReadyOK = false;

					m_pPressLogo->SetStartCnt(m_bPlayerReadyOK);

					break;
				}
				else
				{
					// 準備完了していたらロゴを出現させる
					m_bPlayerReadyOK = true;

					m_pPressLogo->SetStartCnt(m_bPlayerReadyOK);
				}
			}
		}
	}
	else if (m_bPlayerReadyOK == true && m_bOK == false)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (m_logoInfo[nCnt].bJoin == true)
			{
				// 参加しているとき処理
				if (m_logoInfo[nCnt].bReady != true)
				{
					// 準備完了していないならロゴを表示させない
					m_bPlayerReadyOK = false;

					m_pPressLogo->SetStartCnt(m_bPlayerReadyOK);

					return;
				}
			}
		}
		// ボタンを押したらtrueにする
		if (pKey->GetTrigger(KEY_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_PAUSE))
		{
			m_pPressLogo->PressButton();

			m_bOK = true;
		}
	}

	// 遷移できるとき処理
	if (m_pPressLogo->GetTransition())
	{
		m_bTransition = true;

		// 保存用オブジェクトに人数を保存
		int nNumPlayer = 0;

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (m_logoInfo[nCnt].bReady == true)
			{
				nNumPlayer++;
			}
		}
		CKeepObject *pKeepObject = CManager::GetKeepObject();
		pKeepObject->SetNumPlayer(nNumPlayer);
	}

	if (pJoypad->GetButtonTrigger(0, JOYPAD_CANCEL) && m_logoInfo[0].bJoin == false)
	{
		m_pPressLogo->SetStartCnt(false);

		// 効果音
		CSound::Play(CSound::SOUND_LABEL_SE_CANCEL);

		CBase *pBase = CManager::GetBase();

		if (pBase != NULL)
		{
			CPreparationPlayer *pPP = (CPreparationPlayer*)pBase;

			pPP->SetMode(CPreparationPlayer::PREPARATIONPLAYER_TRANSITION_MODESELECT);
			m_bTransition = true;
		}
	}
}

//=============================================================================
// キャラ選択初期化処理
//=============================================================================
void CPreparationPlayerManager::InitCharaSelect(void)
{
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (m_pFrame[nCnt] != NULL)
		{
			// テクスチャ管理情報の取得
			CTexture *pTex = CManager::GetTexture();

			// フレームの位置情報取得
			D3DXVECTOR3 posFrame = m_pFrame[nCnt]->GetPosition();

			// 各UIの初期化
			m_pJoinLogo[nCnt] = CScene2D::Create(D3DXVECTOR3(posFrame.x, posFrame.y, 0.0f),	// 参加
				D3DXVECTOR3(160.0f, 60.0f, 0.0f),
				PRIORITY_NONE,
				pTex->SetTexture("data/TEXTURE/Entry00.png"));
			m_pCompLogo[nCnt] = CScene2D::Create(D3DXVECTOR3(posFrame.x, posFrame.y + 130.0f, 0.0f),	// 準備完了
				D3DXVECTOR3(130.0f, 30.0f, 0.0f),
				PRIORITY_NONE,
				pTex->SetTexture("data/TEXTURE/ENTER06.png"));
			m_pOkLogo[nCnt] = CScene2D::Create(D3DXVECTOR3(posFrame.x, posFrame.y + 130.0f, 0.0f),	// OK
				D3DXVECTOR3(80.0f, 30.0f, 0.0f),
				PRIORITY_NONE,
				pTex->SetTexture("data/TEXTURE/return00.png"));
		}
	}
}

//=============================================================================
// キャラ選択初期化処理
//=============================================================================
void CPreparationPlayerManager::CharaSelect(void)
{
	// 操作プレイヤーを取得
	int nCntPlayerOP = CPreparationPlayer::GetnCntPlayer();

	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();

	if (pKey->GetTrigger(KEY_OK))
	{
		if (m_logoInfo[nCntPlayerOP].bJoin != true)
		{
			m_logoInfo[nCntPlayerOP].bJoin = true;
		}
		else if (m_logoInfo[nCntPlayerOP].bReady != true)
		{
			m_logoInfo[nCntPlayerOP].bReady = true;
		}
	}
	if (pKey->GetTrigger(KEY_SKILL))
	{
		if (m_logoInfo[nCntPlayerOP].bReady == true)
		{
			m_logoInfo[nCntPlayerOP].bReady = false;
		}
		else if (m_logoInfo[nCntPlayerOP].bJoin == true)
		{
			m_logoInfo[nCntPlayerOP].bJoin = false;
		}	
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pJoypad->GetButtonTrigger(nCnt, JOYPAD_DECISION))
		{
			if (m_logoInfo[nCnt].bJoin != true)
			{
				m_logoInfo[nCnt].bJoin = true;
			}
			else if (m_logoInfo[nCnt].bReady != true)
			{
				m_logoInfo[nCnt].bReady = true;
			}
		}
		else if (pJoypad->GetButtonTrigger(nCnt, JOYPAD_CANCEL))
		{
			if (m_logoInfo[nCnt].bReady == true)
			{
				m_logoInfo[nCnt].bReady = false;
			}
			else if (m_logoInfo[nCnt].bJoin == true)
			{
				m_logoInfo[nCnt].bJoin = false;
			}
		}
	}
}
//=============================================================================
//
// ゲームシーンのプレイヤー処理 [player_game.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "player_game.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "camera.h"
#include "game.h"
#include "score.h"
#include "eraser.h"
#include "graffiti.h"
#include "stage.h"
#include "pause.h"
#include "countdown.h"
#include "eraserdust.h"
#include "player_marker.h"
#include "skill.h"
#include "shadow.h"

#include "keep_object.h"
#include "load_player_data.h"
#include "texture.h"
#include "result.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_PLAYER						(1.8f)				// プレイヤーの移動量
#define JUMP_PLAYER						(15.0f)				// プレイヤーのジャンプ量
#define GRAVITY_PLAYER					(1.0f)				// プレイヤーの重力
#define PI_QUARTER						(D3DX_PI / 4.0f)	// 移動用
#define INERTIA_NORMAL					(0.4f)				// 通常慣性
#define INERTIA_SKY						(0.1f)				// 空中慣性

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// CPlayer_Gameのコンストラクタ
//=============================================================================
CPlayer_Game::CPlayer_Game(int nPriority) : CPlayer(nPriority)
{
	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCleaner = false;
	m_nCntDeleteGraffiti = 0;
}

//=============================================================================
// CPlayer_Gameのデストラクタ
//=============================================================================
CPlayer_Game::~CPlayer_Game()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer_Game *CPlayer_Game::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber)
{
	// ポインタ変数の生成
	CPlayer_Game *pPlayer_Game;
	pPlayer_Game = new CPlayer_Game(CScene::PRIORITY_PLAYER);

	// NULLチェック
	if (pPlayer_Game != NULL)
	{
		// 何番目か保存
		pPlayer_Game->SetNumber(nNumber);

		// 初期化処理
		pPlayer_Game->Init(pos, size);

		// オブジェクトタイプの設定
		pPlayer_Game->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer_Game;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer_Game::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// プレイヤーの初期化処理
	CPlayer::Init(pos, size);

	// 変数の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCleaner = false;
	m_nCntDeleteGraffiti = 0;
	m_pPlayerMarker = NULL;
	m_pEraser = NULL;
	m_bSkill = false;
	m_bColEraser = false;
	m_bColSkill = false;
	m_nCntSkillGauge = 0;
	m_bMaxSkillGauge = false;
	m_nRank = 0;

	m_pShadow = CShadow::Create(pos,D3DXVECTOR2(25.0f, 25.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer_Game::Uninit(void)
{
	// 消したラクガキ数の保存
	CManager::GetKeepObject()->SetDeleteGraffiti(m_nCntDeleteGraffiti, GetNumber());

	// 削除
	CPlayer::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer_Game::Update(void)
{
	// プレイヤーの更新処理
	CPlayer::Update();

	// カウントダウン情報を取得
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;
		CCountdown *pCountdown = pGame->GetCountdwn();

		if (pCountdown != NULL && pCountdown->GetFinish() && pGame->GetGameMode() == CGame::GAMEMODE_NORMAL)
		{
			if (m_bSkill == false)
			{
				// 移動処理
				int nNumber = GetNumber();

				if (m_bColSkill == false)
				{
					/*if (nNumber == CGame::GetnCntPlayer())
					{
						MoveKeyboard();
					}
					else*/
					{
						Move();
					}

					if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_BATTLE)
					{
						// スキルとの当たり判定
						CollisionSkill();
					}
				}
			}
			else
			{
				// 消しゴムとステージの当たり判定
				m_bColEraser = m_pEraser->CollisionStage(m_nRank - 1);

				if (GetFinishMotion() == true)
				{
					m_bSkill = false;
					m_bColEraser = false;
					m_pEraser->SetColStage(false);
				}
			}

			// ポーズ
			Pause();
		}
	}

	// 位置を取得
	D3DXVECTOR3 pos = GetPos();

	// 慣性
	if (pos.y <= 0.0f)
	{
		// 地面
		m_move.x += (0.0f - m_move.x) * INERTIA_NORMAL;
		m_move.z += (0.0f - m_move.z) * INERTIA_NORMAL;
	}
	else
	{
		// 空中
		m_move.x += (0.0f - m_move.x) * INERTIA_SKY;
		m_move.z += (0.0f - m_move.z) * INERTIA_SKY;
	}

	// 重力
	m_move.y -= GRAVITY_PLAYER;

	// 移動量を位置に加算
	pos += m_move;

	// 位置の設定
	SetPos(pos);

	// ステージとの当たり判定
	CollisionStage();

	// 消しカスとの当たり判定
	CollisionEraserDust();

	// プレイヤー同士の当たり判定
	CollisionPlayer();

	if (pBase != NULL && m_pPlayerMarker != NULL)
	{
		CGame *pGame = (CGame*)pBase;
		CCamera *pCamera = pGame->GetCamera();

		// マーカーの位置を変更
		D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
		m_pPlayerMarker->SetPosition(D3DXVECTOR3(pos.x, pos.y - 70.0f, pos.z));
	}

	// 影の位置更新
	pos = GetPos();
	m_pShadow->SetPosition(D3DXVECTOR3(pos.x, 1.0f, pos.z));
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer_Game::Draw(void)
{
	// プレイヤーの描画処理
	CPlayer::Draw();
}

//=============================================================================
// ポーズ設定処理
//=============================================================================
void CPlayer_Game::Pause(void)
{
	// ポーズ情報の取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_PAUSE);

	// NULLチェック
	if (pScene != NULL)
	{
		while (pScene)
		{
			// 次の情報を保存
			CScene *pSceneNext = pScene->GetNext();

			// オブジェクトの種類が床だったら床にキャスト
			if (pScene->GetObjType() == CScene::OBJTYPE_PAUSE)
			{
				CPause *pPause = (CPause*)pScene;

				// キーボード情報の取得
				CInputKeyboard *pKey = GET_KEYBOARD;

				// CInputKeyboardインスタンスの取得
				CInputJoypad *pJoypad = GET_JOYPAD;

				// 何番目か取得
				int nNumber = GetNumber();

				if ((pKey->GetTrigger(KEY_PAUSE) && nNumber == CGame::GetnCntPlayer()) || pJoypad->GetButtonTrigger(nNumber, JOYPAD_PAUSE))
				{
					pPause->SetPause(nNumber);
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer_Game::Move(void)
{
	// カメラの角度情報の取得
	CGame *pGame = NULL;
	CCamera *pCamera = NULL;
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		pGame = (CGame*)pBase;
		pCamera = pGame->GetCamera();
	}
	D3DXVECTOR3 rotCamera = pCamera->GetRotate();

	// 何番目か取得
	int nNumber = GetNumber();

	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	// CInputKeyboardインスタンスの取得
	CInputJoypad *pJoypad = GET_JOYPAD;

	// ゲームパッド情報の取得
	DIJOYSTATE2 Controller = pJoypad->GetControllerPless(nNumber);

	if (Controller.lX != 0 || Controller.lY != 0)
	{
		// 変数宣言
		D3DXVECTOR3 rotDest = {};

		// スティックの入力角度
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY);

		rotDest.y = rotCamera.y - fAngle;

		m_move.x -= MOVE_PLAYER * sinf(rotDest.y);			// X軸移動量の決定
		m_move.z -= MOVE_PLAYER * cosf(rotDest.y);			// Z軸移動量の決定

		// モーションを移動モーションにする
		SetMotion(CPlayer::MOTIONTYPE_MOVE);

		// 角度の設定
		SetRotDest(rotDest);
	}
	else
	{
		// ニュートラルモーションにする
		SetMotion(CPlayer::MOTIONTYPE_NEUTORAL);
	}

	// ラクガキ消す処理
	if (pJoypad->GetButtonPress(nNumber, JOYPAD_GRAFFITI_DELETE))
	{
		// ラクガキとの当たり判定
		CollisionGraffiti();

		// 移動量を半分にする
		m_move *= 0.5f;

		// 消してる状態にする
		m_bCleaner = true;

		// モーションをアクションモーションにする
		SetMotion(CPlayer::MOTIONTYPE_DELETE);

		// スキルゲージ用カウント
		m_nCntSkillGauge++;

		if (m_nCntSkillGauge >= DELETE_GARAFFITI_NUMBER_MAX)
		{
			m_nCntSkillGauge = DELETE_GARAFFITI_NUMBER_MAX;
		}
	}
	else
	{
		m_bCleaner = false;
	}

	// 消しゴム切り替え処理
	if (pJoypad->GetButtonTrigger(nNumber, JOYPAD_ERASERTYPE_SWITCH))
	{
		m_pEraser->SwicthType();
	}

	// スキル発動
	if (m_bMaxSkillGauge == true)
	{
		if (pJoypad->GetButtonTrigger(nNumber, JOYPAD_SKILL))
		{
			// スキルを使用したことを伝える
			m_bSkill = true;

			// モーションをスキルにする
			SetMotion(CPlayer::MOTIONTYPE_SKILL);

			// 値を初期化
			m_nCntSkillGauge = 0;
			m_bMaxSkillGauge = false;
		}
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer_Game::MoveKeyboard(void)
{
	// カメラの角度情報の取得
	CGame *pGame = NULL;
	CCamera *pCamera = NULL;
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		pGame = (CGame*)pBase;
		pCamera = pGame->GetCamera();
	}

	// 変数宣言
	D3DXVECTOR3 rotDest = GetRotDest();
	D3DXVECTOR3 rotCamera = pCamera->GetRotate();

	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	// CInputKeyboardインスタンスの取得
	CInputJoypad *pJoypad = GET_JOYPAD;

	// ゲームパッド情報の取得
	DIJOYSTATE2 Controller = pJoypad->GetControllerPless(0);

	if (pKey->GetPress(KEY_MOVE_UP) == true)
	{
		if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
		{// モデル斜め右上移動
			m_move.z -= MOVE_PLAYER * sinf(rotCamera.y - PI_QUARTER);
			m_move.x += MOVE_PLAYER * cosf(rotCamera.y - PI_QUARTER);
			rotDest.y = rotCamera.y - PI_QUARTER * 3.0f;
		}
		else if (pKey->GetPress(KEY_MOVE_LEFT) == true)
		{// モデル斜め右上移動
			m_move.z += MOVE_PLAYER * sinf(rotCamera.y + PI_QUARTER);
			m_move.x -= MOVE_PLAYER * cosf(rotCamera.y + PI_QUARTER);
			rotDest.y = rotCamera.y + PI_QUARTER * 3.0f;
		}
		else
		{// モデル上移動
			m_move.z += MOVE_PLAYER * cosf(rotCamera.y);
			m_move.x += MOVE_PLAYER * sinf(rotCamera.y);
			rotDest.y = rotCamera.y - D3DX_PI;
		}
		// 移動しているとき移動モーションに設定
		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else if (pKey->GetPress(KEY_MOVE_DOWN) == true)
	{
		if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
		{// モデル斜め右上移動
			m_move.z -= MOVE_PLAYER * sinf(rotCamera.y + PI_QUARTER);
			m_move.x += MOVE_PLAYER * cosf(rotCamera.y + PI_QUARTER);
			rotDest.y = rotCamera.y - PI_QUARTER;
		}
		else if (pKey->GetPress(KEY_MOVE_LEFT) == true)
		{// モデル斜め右上移動
			m_move.z += MOVE_PLAYER * sinf(rotCamera.y - PI_QUARTER);
			m_move.x -= MOVE_PLAYER * cosf(rotCamera.y - PI_QUARTER);
			rotDest.y = rotCamera.y + PI_QUARTER;
		}
		else
		{// モデル下移動
			m_move.z -= MOVE_PLAYER * cosf(rotCamera.y);
			m_move.x -= MOVE_PLAYER * sinf(rotCamera.y);
			rotDest.y = rotCamera.y;
		}
		// 移動しているとき移動モーションに設定
		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
	{// モデル右移動
		m_move.z -= MOVE_PLAYER * sinf(rotCamera.y);
		m_move.x += MOVE_PLAYER * cosf(rotCamera.y);
		rotDest.y = rotCamera.y - D3DX_PI / 2.0f;

		// 移動しているとき移動モーションに設定
		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else if (pKey->GetPress(KEY_MOVE_LEFT) == true)
	{// モデル左移動
		m_move.z += MOVE_PLAYER * sinf(rotCamera.y);
		m_move.x -= MOVE_PLAYER * cosf(rotCamera.y);
		// 移動しているとき移動モーションに設定
		rotDest.y = rotCamera.y + D3DX_PI / 2.0f;

		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else
	{
		SetMotion(CPlayer::MOTIONTYPE_NEUTORAL);
	}

	// ラクガキ消す処理
	if (pKey->GetPress(KEY_DELETE))
	{
		// ラクガキとの当たり判定
		CollisionGraffiti();

		// 移動量を制限する
		m_move *= 0.4f;

		// 消してる状態にする
		m_bCleaner = true;

		// モーションをアクションモーションにする
		SetMotion(CPlayer::MOTIONTYPE_DELETE);

		// スキルゲージ用カウント
		m_nCntSkillGauge += 2;

		if (m_nCntSkillGauge >= DELETE_GARAFFITI_NUMBER_MAX)
		{
			m_nCntSkillGauge = DELETE_GARAFFITI_NUMBER_MAX;
		}
	}
	else
	{
		m_bCleaner = false;
	}

	// 消しゴム切り替え処理
	if (pKey->GetTrigger(KEY_SWITCH_ERASER))
	{
		m_pEraser->SwicthType();
	}

	// スキル発動
	if (m_bMaxSkillGauge == true)
	{
		if (pKey->GetTrigger(KEY_SKILL))
		{
			// スキルを使用したことを伝える
			m_bSkill = true;

			// モーションをスキルにする
			SetMotion(CPlayer::MOTIONTYPE_SKILL);

			// 値を初期化
			m_nCntSkillGauge = 0;
			m_bMaxSkillGauge = false;
		}
	}

	// 目的の向きの設定
	SetRotDest(rotDest);
}

//=============================================================================
// モーションの設定
//=============================================================================
bool CPlayer_Game::SetMotion(MotionType motionType)
{
	// モーションの種類によって処理を変える
	switch (motionType)
	{
		// ニュートラル
	case MOTIONTYPE_NEUTORAL:
		if (GetMotionType() == MOTIONTYPE_DELETE && m_bCleaner == true)
		{
			// 現在のモーションがジャンプなら返す
			return false;
		}
		break;

		// 移動
	case MOTIONTYPE_MOVE:
		if (GetMotionType() == MOTIONTYPE_DELETE && m_bCleaner == true)
		{
			// 現在のモーションがジャンプなら返す
			return false;
		}
		break;

		// アクション
	case MOTIONTYPE_DELETE:
		break;

		// スキル
	case MOTIONTYPE_SKILL:
		// モーションが開始したことを伝える
		SetFinishMotion(false);
		break;
	}

	if (GetMotionType() != motionType)
	{
		// モーションの種類設定
		SetMotionType(motionType);

		// 現在のモーション情報の取得
		CMotion::MOTION motion = m_pMotion->GetMotion();

		// モーションカウンタを初期化
		motion.nCounterMotion = 0;

		// 現在のキーを初期化
		motion.nKey = 0;

		// 現在のモーション情報を設定
		m_pMotion->SetMotion(motion);

		// 消しゴムのオフセット設定
		m_pEraser->SetOffset((int)motionType);

		// モーション
		Motion();

		return true;
	}
	return false;
}

//=============================================================================
// ステージとの当たり判定
//=============================================================================
bool CPlayer_Game::CollisionStage(void)
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
				D3DXVECTOR3 pos = GetPos();

				// 当たり判定
				if (pStage->Collision(&pos, GetSize()))
				{
					// 移動量の初期化
					if (m_move.y < 0.0f)
					{
						m_move.y = 0.0f;

						if (m_bColSkill == true)
						{
							m_bColSkill = false;
						}
					}
					// 位置の設定
					SetPos(pos);

					return true;
				}

				// 位置の設定
				SetPos(pos);
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
	return false;
}

//=============================================================================
// ラクガキとの当たり判定
//=============================================================================
void CPlayer_Game::CollisionGraffiti(void)
{
	// 床情報の取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_GRAFFITI);

	// NULLチェック
	if (pScene != NULL)
	{
		while (pScene)
		{
			// 次の情報を保存
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_GRAFFITI)
			{
				// オブジェクトの種類がラクガキだったらラクガキにキャスト
				CGraffiti *pGraffiti = (CGraffiti*)pScene;

				// 消しゴムタイプの取得
				CEraser::EraserType eraserType = m_pEraser->GetType();

				// 消しゴムの位置取得
				D3DXVECTOR3 posEraser = m_pEraser->GetPosition();
				D3DXVECTOR3 sizeEraser = m_pEraser->GetSize();

				// あたり判定
				if (pGraffiti->Collision(posEraser, sizeEraser, eraserType))
				{
					// 消した数をカウント
					m_nCntDeleteGraffiti++;

					// スキルゲージ用カウント
					m_nCntSkillGauge += 5;

					if (m_nCntSkillGauge >= DELETE_GARAFFITI_NUMBER_MAX)
					{
						m_nCntSkillGauge = DELETE_GARAFFITI_NUMBER_MAX;
					}
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 消しカスとの当たり判定
//=============================================================================
void CPlayer_Game::CollisionEraserDust(void)
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

			if (pScene->GetObjType() == CScene::OBJTYPE_ERASERDUST)
			{
				// オブジェクトの種類が	消しカスだったらキャスト
				CEraserDust *pEraserDust = (CEraserDust*)pScene;

				// あたり判定
 				pEraserDust->Collision(GetPos(), GetRot());
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// スキルとの当たり判定
//=============================================================================
void CPlayer_Game::CollisionSkill(void)
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

				// 位置とサイズの取得
				D3DXVECTOR3 pos = GetPos();
				D3DXVECTOR3 size = GetSize();

				// 当たり判定
				if (pSkill->Collision(pos, size, &m_move, D3DXVECTOR3(15.0f, 10.0f, 15.0f)))
				{
					m_bColSkill = true;

					float fRata = (float)m_nCntDeleteGraffiti * 0.5f;

					m_nCntDeleteGraffiti -= (int)fRata;

					if (m_nCntDeleteGraffiti < 0)
					{
						m_nCntDeleteGraffiti = 0;
					}
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// スキルとの当たり判定
//=============================================================================
void CPlayer_Game::CollisionPlayer(void)
{
	// 床情報の取得
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_PLAYER);

	// NULLチェック
	if (pScene != NULL)
	{
		while (pScene)
		{
			// 次の情報を保存
			CScene *pSceneNext = pScene->GetNext();

			// オブジェクトの種類が床だったら床にキャスト
			if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
			{
				CPlayer_Game *pPlayer = (CPlayer_Game*)pScene;

				// プレイヤーの番号が自分以外の時処理
				if (pPlayer->GetNumber() != GetNumber())
				{
					// 位置とサイズの取得
					D3DXVECTOR3 pos = GetPos();
					D3DXVECTOR3 size = GetSize();

					// 対象の位置とサイズの取得
					D3DXVECTOR3 posPlayer = pPlayer->GetPos();
					D3DXVECTOR3 sizePlayer = pPlayer->GetSize();

					// 距離を求める
					D3DXVECTOR3 vec = posPlayer - pos;
					float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

					// ある距離よ短いとき当たり判定を取る
					if (fLength < 25.0f)
					{
						// 移動量を求める
						float fRate = 1.0f - (fLength / 25.0f);
						pos.x -= vec.x * fRate;
						pos.z -= vec.z * fRate;
						posPlayer.x += vec.x * fRate;
						posPlayer.z += vec.z * fRate;
						SetPos(pos);
						pPlayer->SetPos(posPlayer);
					}
				}
			}

			// 次の情報にする
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// 消しゴムのオフセット情報の読み込み
//=============================================================================
void CPlayer_Game::LoadEraser(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/eraser.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// 変数宣言
		char aLine[128];
		char aText[128];
		bool bScript = false;
		bool bPartsSet = false;
		bool bTypeSet = false;
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntType = 0;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;

		// SCRIPTを探す
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// 毎行空白を見つけるまで読み込む
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPTを見つけたらそれ以降を読み込む許可を与える
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				bScript = true;
			}
			if (bScript == true)
			{
				// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
				if (strcmp(&aText[0], "TYPESET") == 0)
				{
					bTypeSet = true;
				}
				// 各パーツの情報を格納する許可があるとき処理
				if (bTypeSet == true)
				{
					// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
					if (strcmp(&aText[0], "OFFSET") == 0)
					{
						bPartsSet = true;
					}
					// 各パーツの情報を格納する許可があるとき処理
					if (bPartsSet == true)
					{
						// POSを見つけたら座標情報を格納
						if (strcmp(&aText[0], "POS") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&pos.x,
								&pos.y,
								&pos.z);
						}
						// ROTを見つけたら角度情報を格納
						if (strcmp(&aText[0], "ROT") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&rot.x,
								&rot.y,
								&rot.z);
						}
						// END_PARTSSETを見つけたら各パーツの情報を格納を終える
						if (strcmp(&aText[0], "END_OFFSET") == 0)
						{
							bPartsSet = false;
							m_pEraser->InitOffset(nCntParts, nCntType, pos, rot);

							// パーツの総数だけ格納するのでカウントを増やす
							nCntParts++;
						}
					}
					// END_PARTSSETを見つけたら各パーツの情報を格納を終える
					if (strcmp(&aText[0], "END_TYPESET") == 0)
					{
						bTypeSet = false;

						// パーツの総数だけ格納するのでカウントを増やす
						nCntType++;
						nCntParts = 0;
					}
				}
				// END_SCRIPTを見つけたら読み込みを終える
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					break;
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
	}
}

//=============================================================================
// プレイヤーマーカー情報の設定
//=============================================================================
void CPlayer_Game::SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera)
{
	m_pPlayerMarker = pPlayerMarker;

	m_pPlayerMarker->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPlayerMarker->SetTex(D3DXVECTOR2((float)GetNumber(), 1));

	D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
	m_pPlayerMarker->SetPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));
}

//=============================================================================
// 消しゴム情報の設定
//=============================================================================
void CPlayer_Game::SetEraser(CEraser *pEraser)
{
	m_pEraser = pEraser;

	m_pEraser->GetModel()->SetParent(GetModel(3));
	LoadEraser();
	m_pEraser->SetOffset((int)GetMotionType());
}
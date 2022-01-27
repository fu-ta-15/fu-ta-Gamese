//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "player.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "floor.h"
#include "model.h"
#include "motion.h"
#include "camera.h"
#include "game.h"
#include "score.h"
#include "eraser.h"
#include "graffiti.h"
#include "stage.h"
#include "shadowS.h"
#include "pause.h"
#include "countdown.h"
#include "eraserdust.h"
#include "player_marker.h"
#include "rank.h"

#include "keep_object.h"
#include "load_player_data.h"
#include "texture.h"
#include "result.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_PLAYER		(1.8f)		// プレイヤーの移動量
#define JUMP_PLAYER		(15.0f)		// プレイヤーのジャンプ量
#define GRAVITY_PLAYER	(1.0f)		// プレイヤーの重力
#define PI_QUARTER		(D3DX_PI / 4.0f)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// CPlayerのコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumber = 0;
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_motionType = MOTIONTYPE_MOVE;
}

//=============================================================================
// CPlayerのデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nNumber)
{
	// ポインタ変数の生成
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// NULLチェック
	if (pPlayer != NULL)
	{
		// 何番目か保存
		pPlayer->SetNumber(nNumber);

		// 初期化処理
		pPlayer->Init(pos, size);

		// オブジェクトタイプの設定
		pPlayer->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 変数の初期化
	m_pos = pos;
	m_size = D3DXVECTOR3(size.x, size.y, size.x);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_motionType = MOTIONTYPE_NEUTORAL;
	m_bFinishMotion = true;

	// モーション情報の生成
	m_pMotion = CMotion::Create();

	// プレイヤーデータ情報の取得
	CLoadPlayerData::Player_Data player_Data = CManager::GetKeepObject()->GetLoadPlayerData()->GetPlayerData(m_nNumber);
	m_nNumModel = player_Data.nNumModel;

	// モデルの読み込み
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// データの保存
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(player_Data.aOffset[nCntModel][0], player_Data.aOffset[nCntModel][1], player_Data.aOffset[nCntModel][2]),
			D3DXVECTOR3(player_Data.aOffset[nCntModel][3], player_Data.aOffset[nCntModel][4], player_Data.aOffset[nCntModel][5]),
			&player_Data.apModelFileName[nCntModel][0]);

		// 親モデルの設定
		if (player_Data.nIndexParent[nCntModel] >= 0)
		{
			m_apModel[nCntModel]->SetParent(m_apModel[player_Data.nIndexParent[nCntModel]]);
		}
		else
		{
			m_apModel[nCntModel]->SetParent(NULL);
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// メモリの開放
	if (m_pMotion != NULL)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// 乗っているかどうか
	bool bLandModel = false;

	// 座標の保存
	m_posOld = m_pos;

	// 現在の回転角を制限
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		m_rotDest.y -= D3DX_PI * 2.0f;
	}
	// 向きの更新
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.2f;

	// 現在の回転角を制限
	if (m_rot.y < -D3DX_PI)
	{// -3.14fより小さくなったとき値に3.14fにする
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{// 3.14fより大きくなったとき値を-3.14fにする
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// モーション管理
	Motion();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// 変数の受け渡し
	CRenderer *pRender = CManager::GetRenderer();
	pDevice = pRender->GetDevice();

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

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// パーツの描画
		m_apModel[nCnt]->Draw();
	}
}

//=============================================================================
// モーションの管理
//=============================================================================
void CPlayer::Motion(void)
{
	// 変数宣言
	CMotion::MOTION_INFO *pMotionInfo = CMotion::GetMotionInfo(0);		// モーション情報の取得
	CMotion::MOTION motion = m_pMotion->GetMotion();					// 現在のモーション情報の取得
	CMotion::KEY *pKey;													// キー情報
	CMotion::KEY *pKeyNext;												// 次のキー情報
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (motion.nKey == pMotionInfo[m_motionType].nNumKey - 1 && pMotionInfo[m_motionType].bLoop == false)
		{
			// 終わったことを伝える
			if (m_bFinishMotion == false)
			{
				m_bFinishMotion = true;
			}
			break;
		}

		// キーの確定
		pKey = &pMotionInfo[m_motionType].aKeyInfo[motion.nKey].aKey[nCntModel];
		pKeyNext = &pMotionInfo[m_motionType].aKeyInfo[(motion.nKey + 1) % pMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

		// 差分の確定
		float fDiffMoitonPosX = pKeyNext->fPosX - pKey->fPosX;
		float fDiffMoitonPosY = pKeyNext->fPosY - pKey->fPosY;
		float fDiffMoitonPosZ = pKeyNext->fPosZ - pKey->fPosZ;
		float fDiffMoitonRotX = pKeyNext->fRotX - pKey->fRotX;
		float fDiffMoitonRotY = pKeyNext->fRotY - pKey->fRotY;
		float fDiffMoitonRotZ = pKeyNext->fRotZ - pKey->fRotZ;

		// 相対値の確定
		float fRateMotion = (float)motion.nCounterMotion / (float)pMotionInfo[m_motionType].aKeyInfo[motion.nKey].nFrame;

		// モーションの位置と向きの確定
		fPosX = pKey->fPosX + (fDiffMoitonPosX * fRateMotion);
		fPosY = pKey->fPosY + (fDiffMoitonPosY * fRateMotion);
		fPosZ = pKey->fPosZ + (fDiffMoitonPosZ * fRateMotion);
		fRotX = pKey->fRotX + (fDiffMoitonRotX * fRateMotion);
		fRotY = pKey->fRotY + (fDiffMoitonRotY * fRateMotion);
		fRotZ = pKey->fRotZ + (fDiffMoitonRotZ * fRateMotion);

		// 現在の回転角を制限
		if (fRotX > D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			fRotX -= D3DX_PI * 2.0f;
		}
		else if (fRotX < -D3DX_PI)
		{// -3.14fより小さくなったとき値に3.14fにする
			fRotX += D3DX_PI * 2.0f;
		}
		// 現在の回転角を制限
		if (fRotY > D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			fRotY -= D3DX_PI * 2.0f;
		}
		else if (fRotY < -D3DX_PI)
		{// -3.14fより小さくなったとき値に3.14fにする
			fRotY += D3DX_PI * 2.0f;
		}
		// 現在の回転角を制限
		if (fRotZ > D3DX_PI)
		{// 3.14fより大きくなったとき値を-3.14fにする
			fRotZ -= D3DX_PI * 2.0f;
		}
		else if (fRotZ < -D3DX_PI)
		{// -3.14fより小さくなったとき値に3.14fにする
			fRotZ += D3DX_PI * 2.0f;
		}

		// データ保存用クラスの取得
		CKeepObject *pKeepObject = CManager::GetKeepObject();

		if (pKeepObject != NULL)
		{
			// プレイヤーデータ情報の取得
			CLoadPlayerData::Player_Data player_Data = pKeepObject->GetLoadPlayerData()->GetPlayerData(m_nNumber);

			// 位置と向きの設定
			m_apModel[nCntModel]->SetPosition(D3DXVECTOR3(fPosX + player_Data.aOffset[nCntModel][0], fPosY + player_Data.aOffset[nCntModel][1], fPosZ + player_Data.aOffset[nCntModel][2]));
			m_apModel[nCntModel]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
		}
	}

	// モーションカウンタが各キーの最終フレームに到達したとき
	if (motion.nCounterMotion >= pMotionInfo[m_motionType].aKeyInfo[motion.nKey].nFrame)
	{
		if (pMotionInfo[m_motionType].bLoop == false && motion.nKey == pMotionInfo[m_motionType].nNumKey - 1)
		{
			// モーションカウンタの更新
			motion.nCounterMotion = pMotionInfo[m_motionType].aKeyInfo[motion.nKey].nFrame;
		}
		else
		{
			// モーションカウンタの初期化
			motion.nCounterMotion = 0;

			// キーの更新
			motion.nKey++;

			// キーの確定
			motion.nKey = motion.nKey % pMotionInfo[m_motionType].nNumKey;
		}
	}
	else
	{
		// モーションカウンタの更新
		motion.nCounterMotion++;
	}

	// モーション情報の設定
	m_pMotion->SetMotion(motion);
}
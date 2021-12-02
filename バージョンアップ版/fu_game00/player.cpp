//*****************************************************************************
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "player.h"
#include "keyinput.h"
#include "manager.h"
#include "collision.h"
#include "mesh.h"
#include "game.h"
#include "tutorial.h"
#include "move.h"
#include "bulletmesh.h"
#include "particle.h"
#include "time.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PLAYER_MOVE					(5.0f)
#define PLAYER_JUNP					(20.0f)

#define BULLET_SIZE					(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define BULLET_MOVE_RIGHT			(D3DXVECTOR3(10.0f, 0.0f, 0.0f))
#define BULLET_MOVE_LEFT			(D3DXVECTOR3(-10.0f, 0.0f, 0.0f))

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene2D(OBJ_PLAYER)
{
	this->m_col = WhiteColor;				// 色情報
	this->m_posOld = ZeroVector3;			// プレイヤーの過去の位置
	this->m_fLife = PLAYER_LIFE;			// 体力
	this->m_state = STATE_NONE;				// 状態
	this->m_bUse = true;					// 使用中
	this->m_nBullet = 10;					// 弾のストック
	this->m_pShield = NULL;					// シールド
	this->m_nBulletTime = 0;				// 弾の復活時間のカウント
	this->m_bAlive = true;					// 生存中
	this->m_bFall = false;					// 落下したか
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
}

//=============================================================================
// プレイヤーの生成
//=============================================================================
CPlayer * CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// プレイヤーのポインタ変数生成
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULLチェック
		pPlayer = new CPlayer;			// インスタンス生成
		pPlayer->m_pos = pos;			// 位置の設定
		pPlayer->m_size = size;			// サイズの設定
		pPlayer->Init();				// 初期化処理
	}
	// 情報を返す
	return pPlayer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// メンバ変数の設定
	m_tex = D3DXVECTOR2(2, 0);
	m_number = D3DXVECTOR2(1, 0);
	m_nAnimeCnt = 1;
	m_nBulletCharge = 0;

	// ポリゴンの生成・テクスチャの設定
	CScene2D::Init(m_pos, m_size);
	CScene2D::CreateTexture("data/TEXTURE/player0.png");
	CScene2D::SetTex(m_tex, m_number);

	// ライフの設定
	for (int nCntLife = 0; nCntLife < PLAYER_LIFE_STOCK; nCntLife++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(50.0f, 190.0f - (10 * nCntLife), 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f,5.0f, 0.0f);
		m_pLife[nCntLife] = CScene2D::Create(pos, size);
		m_pLife[nCntLife]->CreateTexture("data/TEXTURE/lifeBlock.png");
	}

	// ウェポンの設定
	for (int nCntWeapon = 0; nCntWeapon < PLAYER_BULLET_STOCK; nCntWeapon++)
	{
		D3DXVECTOR3 pos = D3DXVECTOR3(80.0f, 100.0f + (10 * nCntWeapon), 0.0f);
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 5.0f, 0.0f);
		m_pWeapon[nCntWeapon] = CScene2D::Create(pos, size);
		m_pWeapon[nCntWeapon]->CreateTexture("data/TEXTURE/BulletBlock.png");
	}

	// シールドの設定
	m_pShield = CEffect::Create(m_pos, m_size * 2);
	m_pShield->CreateTexture("data/TEXTURE/Shockwave.png");
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// ポリゴンの終了処理
	CScene2D::Uninit();

	// シールドの開放
	if (m_pShield != NULL)
	{
		m_pShield->Uninit();
		m_pShield = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// キーボード情報取得
	CKey *pKey = CManager::GetKey();

	// 何もない状態の時
	if (m_state == STATE_NONE)
	{
		// アクション
		PlayerAction();	
	}

	// ライフの管理
	PlayerLife();

	// 移動量の加算
	m_move.y += 1.0f;

	// 移動量の加算
	m_pos += m_move;

	// 移動量の制御
	PlayerMoveControl();

	// 位置の制御
	PosControl();	

	// アニメーション
	PlayerAnime();

	// 状態管理
	PlayerState();

	CScene2D::SetUse(m_bUse);	// 存在している
	CScene2D::SetPos(m_pos);	// 位置の設定（更新）
	CScene2D::SetCol(m_col);	// 色の設定（更新）
	m_pShield->SetPos(m_pos);	// シールドの位置更新
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// ポリゴンの描画
	CScene2D::Draw();
}

//=============================================================================
// プレイヤーの位置制御
//=============================================================================
void CPlayer::PosControl(void)
{
	// 地面の当たり判定
	FieldControl();

	// 左画面端上限
	if (m_pos.x - m_size.x < 0)
	{
		// 元の位置の戻す
		m_pos.x = 0 + m_size.x;				
	}

	// 右画面端上限
	if (m_pos.x + m_size.x > SCREEN_WIDTH)
	{
		// 元の位置の戻す
		m_pos.x = SCREEN_WIDTH - m_size.x;	
	}
}

//=============================================================================
// プレイヤーのアクション
//=============================================================================
void CPlayer::PlayerAction(void)
{
	// キーボード情報取得
	CKey *pKey = CManager::GetKey();

	// 弾復活用のタイム
	m_nBulletTime++;

	// 毎35フレーム
	if ((m_nBulletTime % 35) == 0)
	{
		// ポリゴンを一つずつ確認
		for (int nCntWeapon = PLAYER_BULLET_STOCK; nCntWeapon > 0; nCntWeapon--)
		{
			// そのポリゴンが使われていなかったら（可視化してなかったら
			if (m_pWeapon[nCntWeapon - 1]->GetUse() != true)
			{
				// バレットストックの加算
				m_nBullet += 1;

				// ポリゴンを可視化する
				m_pWeapon[nCntWeapon - 1]->SetUse(true);
				break;
			}
		}
	}

	// 移動
	PlayerMove();

	// 弾の発射
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_NUMPAD6) == true)
	{
		m_nBulletCharge++;
		if ((m_nBulletCharge % 20) == 0)
		{
			CBulletMesh::Create(m_pos, D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(10.0f, 0.0f, 0.0f), true, OBJ_BULLET2);	// バレットの生成
			m_nBulletCharge = 0;																												// プレイヤーの弾消費
		}
	}	// トリガー・NUM6 が押されたとき
	else if (pKey->GetState(CKey::STATE_RELEASE, DIK_NUMPAD6) == true)
	{
		CBulletMesh::Create(m_pos, D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(10.0f, 0.0f, 0.0f), false, OBJ_BULLET1);	// バレットの生成
		m_nBulletCharge = 0;
		m_bPresse = false;
	}

}

//=============================================================================
// プレイヤーの移動
//=============================================================================
void CPlayer::PlayerMove(void)
{
	// キーボード情報取得
	CKey *pKey = CManager::GetKey();

	// プレス・Dが押されたとき
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D) == true)	
	{
		// アニメーションカウントアップ
		m_nAnimeCnt++;

		// 移動量の更新
		m_move.x += PLAYER_MOVE;	
	}

	// プレス・Aが押されたとき
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A) == true)	
	{
		// アニメーションカウントアップ
		m_nAnimeCnt++;

		// 移動量の更新
		m_move.x -= PLAYER_MOVE;	
	}

	// トリガー・SPACEが押されたとき
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	
	{
		// ジャンプ可能ならば
		if (m_bJunp == false)
		{
			// 移動量の更新
			m_move.y -= PLAYER_JUNP;

			// ジャンプ中
			m_bJunp = true;			
		}
	}

	// ジャンプ中の時
	if (m_bJunp == true)
	{
		// プレス・SPACEが押されたとき
		if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	
		{
			// 重力の減速
			m_move.y -= 0.6f;	
		}
	}
}

//=============================================================================
// プレイヤーの移動量のコントロール
//=============================================================================
void CPlayer::PlayerMoveControl(void)
{
	// 移動・回転量の減衰
	m_move = Move::MoveControl(m_move, D3DXVECTOR3(1.0f, 0.02f, 0.0f));
}

//=============================================================================
// フィールド上のプレイヤー制御
//=============================================================================
void CPlayer::FieldControl(void)
{
	// Meshのポインタ
	CMesh* pMesh = NULL;

	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
		// ゲーム
	case CManager::MODE_GAME:
		pMesh = CGame::GetMesh();
		break;
	default:
		break;
	}

	// 頂点情報の取得
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// 底辺の中心座標設定
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, m_pos.z);

	// 外積当たり判定
	bool bOutPro = false;

	// メッシュポリゴン上辺の位置のみ算出
	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{
		// 二つの頂点と点の外積判定
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x && m_bFall == false)
		{
			bOutPro = Collision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}

		// 点が二点より下にいたら
		if (bOutPro)
		{
			// 戻す分を算出
			m_posOld = Collision::WaveCollision(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);	

			// 重力ゼロ
			m_move.y = 0.0f;					

			// 画面内まで移動させる
			m_pos.y = m_posOld.y - m_size.y;	
			if (m_bJunp == true)
			{
				// ジャンプ可能
				m_bJunp = false;	

				// パーティクル生成
				Particle::SetParticle(D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f), m_size*0.4f, 50, Particle::TYPE_DOWN_FAN, "data/TEXTURE/Fog2001.png");
			}
			break;
		}
	}

	// 落下判定
	if (m_pos.y + m_size.y > pVtx[0].pos.y)
	{
		// 地面より下にいたら
		if (m_pos.y - m_size.y > pVtx[0].pos.y)
		{
			m_bFall = true;
		}

		// 地面より左に行こうとしたら
		if (m_pos.x - m_size.x < WIDTH_HALF + 100)
		{
			// めり込まないようにする
			m_pos.x = WIDTH_HALF + 100 + m_size.x;
		}
	}
}

//=============================================================================
// プレイヤーのアニメージョン
//=============================================================================
void CPlayer::PlayerAnime(void)
{
	// カウントが１０あまり１の時
	if ((m_nAnimeCnt % 5) == 0)
	{
		// テクスチャ座標加算
		m_number.x += 1;	

		// カウント初期化
		m_nAnimeCnt = 0;

		// テクスチャ座標が過ぎていたら
		if (((int)m_number.x % 2) == 0)
		{
			// 初期値に戻す
			m_number.x = 0.0f;	
		}

		// テクスチャ座標の反映
		CScene2D::SetTex(m_tex, m_number);
	}
}

//=============================================================================
// プレイヤーの状態
//=============================================================================
void CPlayer::PlayerState(void)
{
	// 通常状態の場合
	if (m_state == STATE_NONE)
	{
		// 色を戻す
		m_col = WhiteColor;	

		// ダメージ判定OFF
		m_bDamage = false;	
	}

	// シールドがある場合
	if (m_pShield->GetUse() == true && m_bCollEnemy == true)
	{
		// シールド削除
		m_pShield->SetUse(false);	

		// 当たり判定をもとに戻す
		m_bCollEnemy = false;		
	}

	// シールドがなかったら＆＆敵に当たったら
	if (m_pShield->GetUse() == false && m_bCollEnemy == true)
	{
		// ノックアップ状態
		m_state = STATE_KNOCKUP;					

		// ダメージ判定ON
		m_bDamage = true;							

		// ノックアップの位置
		m_KnockUpPos.x = 0.0f;						

		// ノックアップの位置
		m_KnockUpPos.y = m_pos.y + 40.0f;			

		// ダメージの色に変更
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	

		// 当たり判定をもとに戻す
		m_bCollEnemy = false;						
	}

	// ノックアップ状態の時
	if (m_state == STATE_KNOCKUP)
	{
		// ノックアップ状態の処理
		DamagePlayer();	
	}
}

//=============================================================================
// プレイヤー自身にダメージを受けた処理
//=============================================================================
void CPlayer::DamagePlayer(void)
{
	// カウントアップ
	m_nDamageCnt++;	
	
	// 目的の場所へノックアップ
	m_pos.x = Move::TargetPosMove(D3DXVECTOR3(m_KnockUpPos.x, 0.0f, 0.0f), m_pos, 0.035f).x;
	m_pos.y = Move::TargetPosMove(D3DXVECTOR3(0.0f, m_KnockUpPos.y, 0.0f), m_pos, 0.015f).y;

	// カウントが一定まで来たら
	if ((m_nDamageCnt % 15) == 0)
	{
		m_fLife -= (m_fLife * 0.056f + 1.2f);	// ライフの減少
		m_state = STATE_NONE;					// 状態を戻す
		m_nDamageCnt = 0;						// カウントを初期化する
	}
}

//=============================================================================
// ライフ管理
//=============================================================================
void CPlayer::PlayerLife(void)
{
	for (int nCntLife = 0; nCntLife < PLAYER_LIFE_STOCK; nCntLife++)
	{
		// ライフが一定の値を超えたら
		if (m_fLife < nCntLife * 10)
		{
			// ライフを削除
			if (m_pLife[nCntLife] != NULL)
			{
				m_pLife[nCntLife]->Release();
				m_pLife[nCntLife] = NULL;
				break;
			}
		}
	}

	// ライフがゼロになったら
	if (m_fLife < 0)
	{
		m_bAlive = false;
		m_bUse = false;
	}
}

//=============================================================================
// バレットの管理
//=============================================================================
void CPlayer::PlayerBullet(int nBullet)
{
	for (int nCnt = 0; nCnt < nBullet; nCnt++)
	{
		for (int nCntWeapon = 0; nCntWeapon < PLAYER_BULLET_STOCK; nCntWeapon++)
		{
			// 使用した分のポリゴンが残っていたら
			if (m_pWeapon[nCntWeapon]->GetUse() != false)
			{
				m_nBullet -= 1;							// ストックの減らす
				m_pWeapon[nCntWeapon]->SetUse(false);	// そのポリゴンを使用していない状態にする
				break;
			}
		}
	}
}

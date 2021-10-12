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
#include "bullet.h"
#include "collision.h"
#include "mesh.h"
#include "game.h"
#include "tutorial.h"
#include "move.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene2D(OBJ_PLAYER)
{
	this->m_col = WhiteColor;				// 色情報
	this->m_fLife = PLAYER_LIFE;			// 体力
	this->m_state = STATE_NONE;				// 状態
	this->m_bUse = true;					// 使用中
	this->m_nBullet = PLAYER_BULLET_STOCK;	// 弾のストック
	this->m_pShield = NULL;					// シールド
	this->m_nBulletTime = 0;
	this->m_bAlive = true;					// 生存中
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
	m_tex = D3DXVECTOR2(2.0f, 0.0f);
	m_number = D3DXVECTOR2(0.0f, 0.0f);
	m_nAnimeCnt = 0;

	// プレイヤー表示設定
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
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// キーボード情報取得
	CKey *pKey = CManager::GetKey();

	if (m_state == STATE_NONE)
	{// 何もない状態の時
		PlayerAction();	// アクション
	}

	// ライフの管理
	PlayerLife();

	// 移動量の加算
	m_move.y += GRAVITY;

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

	if (m_pos.x - m_size.x < 0)
	{// 左画面端上限
		m_pos.x = 0 + m_size.x;				// 元の位置の戻す
	}
	if (m_pos.x + m_size.x > SCREEN_WIDTH)
	{// 右画面端上限
		m_pos.x = SCREEN_WIDTH - m_size.x;	// 元の位置の戻す
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

	// 弾の復活
	if ((m_nBulletTime % 35) == 0)
	{
		for (int nCntWeapon = PLAYER_BULLET_STOCK; nCntWeapon > 0; nCntWeapon--)
		{
			if (m_pWeapon[nCntWeapon - 1]->GetUse() != true)
			{
				m_nBullet += 1;
				m_pWeapon[nCntWeapon - 1]->SetUse(true);
				break;
			}
		}
	}

	// 移動
	PlayerMove();

	// 弾の発射
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6) == true && m_nBullet > 0)	
	{// トリガー・NUM6 が押されたとき
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_RIGHT);	// バレットの生成
		PlayerBullet();											// プレイヤーの弾消費
	}
}

//=============================================================================
// プレイヤーの移動
//=============================================================================
void CPlayer::PlayerMove(void)
{
	// キーボード情報取得
	CKey *pKey = CManager::GetKey();

	/* プレイヤーの移動 */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D) == true)	
	{// プレス・Dが押されたとき
		m_nAnimeCnt++;
		m_move.x += PLAYER_MOVE;	// 移動量の更新
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A) == true)	
	{// プレス・Aが押されたとき
		m_nAnimeCnt++;
		m_move.x -= PLAYER_MOVE;	// 移動量の更新
	}

	/* プレイヤーのジャンプ */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	
	{// トリガー・SPACEが押されたとき
		if (m_bJunp == false)
		{// ジャンプ可能ならば
			m_move.y -= PLAYER_JUNP;// 移動量の更新
			m_bJunp = true;			// ジャンプ中
		}
	}
	/* プレイヤーの落下減速 */
	if (m_bJunp == true)
	{// ジャンプ中かつ、重力がプラスの時
		if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	
		{// プレス・SPACEが押されたとき
			m_move.y -= 0.6f;	// 重力の減速
		}
	}
}

//=============================================================================
// プレイヤーの移動量のコントロール
//=============================================================================
void CPlayer::PlayerMoveControl(void)
{
	// 移動・回転量の減衰
	m_move = CMove::MoveControl(m_move, MOVE_DECELERATION);
}

//=============================================================================
// フィールド上のプレイヤー制御
//=============================================================================
void CPlayer::FieldControl(void)
{
	CMesh* pMesh = NULL;

	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
		// チュートリアル
	case CManager::MODE_TUTORIAL:
		pMesh = MESH_TUTORIAL;
		break;
		// ゲーム
	case CManager::MODE_GAME:
		pMesh = MESH_GAME;
		break;
	default:
		break;
	}

	// 頂点情報の取得
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// 底辺の中心座標設定
	D3DXVECTOR3 posA = PLAYER_COLL_POS;

	// 外積当たり判定
	bool bOutPro = false;

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{// メッシュポリゴン上辺のみ算出
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// 二つの頂点と点の外積判定
			bOutPro = CCollision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}
		if (bOutPro == true)
		{// 点が二点より下にいたら
			if (pVtx[nCnt].pos.y > SCREEN_HEIGHT &&  pVtx[nCnt + 1].pos.y > SCREEN_HEIGHT)
			{
				m_bFall = true;
			}
			else
			{
				m_bFall = false;
			}
			if (m_bFall == false)
			{
				D3DXVECTOR3 m_posOld = CCollision::WaveCollision(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA, CCollision::TYPE_COL_Y);	// 戻す分を算出
				m_move.y = 0.0f;					// 重力ゼロ
				m_bJunp = false;					// ジャンプ可能
				m_pos.y = m_posOld.y - m_size.y;	// 画面内まで移動させる
			}
			break;
		}
	}
}

//=============================================================================
// プレイヤーのアニメージョン
//=============================================================================
void CPlayer::PlayerAnime(void)
{
	if ((m_nAnimeCnt % 10) == 0)
	{// カウントが１０あまり１の時
		m_number.x += 1;	// テクスチャ座標加算
		m_nAnimeCnt = 0;
		if (((int)m_number.x % 2) == 0)
		{// テクスチャ座標が過ぎていたら
			m_number.x = 0.0f;	// 初期値に戻す
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
	if (m_state == STATE_NONE)
	{// 通常状態の場合
		m_col = WhiteColor;	// 色を戻す
		m_bDamage = false;	// ダメージ判定OFF
	}

	if (m_pShield->GetUse() == true && m_bCollEnemy == true)
	{// シールドがある場合
		m_pShield->SetUse(false);	// シールド削除
		m_bCollEnemy = false;		// 当たり判定をもとに戻す
	}

	if (m_pShield->GetUse() == false && m_bCollEnemy == true)
	{// シールドがなかったら
		m_state = STATE_KNOCKUP;					// ノックアップ状態
		m_bDamage = true;							// ダメージ判定ON
		m_KnockUpPos.x = 0.0f;						// ノックアップの位置
		m_KnockUpPos.y = m_pos.y + 40.0f;			// ノックアップの位置
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// ダメージの色に変更
		m_bCollEnemy = false;						// 当たり判定をもとに戻す
	}

	if (m_state == STATE_KNOCKUP)
	{// ノックアップ状態の時
		DamagePlayer();	// ノックアップ状態の処理
	}
}

//=============================================================================
// プレイヤー自身にダメージを受けた処理
//=============================================================================
void CPlayer::DamagePlayer(void)
{
	m_nDamageCnt++;	// カウントアップ
	
	// 目的の場所へノックアップ
	m_pos.x = CMove::TargetPosMove(D3DXVECTOR3(m_KnockUpPos.x, 0.0f, 0.0f), m_pos, 0.035f).x;
	m_pos.y = CMove::TargetPosMove(D3DXVECTOR3(0.0f, m_KnockUpPos.y, 0.0f), m_pos, 0.015f).y;

	if ((m_nDamageCnt % 15) == 0)
	{// カウントが一定まで来たら
		m_fLife -= (m_fLife * 0.056f + 1.2f);
		m_state = STATE_NONE;	 // 状態を戻す
		m_nDamageCnt = 0.0f;	 // カウントを初期化する
	}
}

void CPlayer::PlayerLife(void)
{
	for (int nCntLife = 0; nCntLife < PLAYER_LIFE_STOCK; nCntLife++)
	{
		if (m_fLife < nCntLife * 10)
		{// ライフが一定の値を超えたら
			if (m_pLife[nCntLife] != NULL)
			{// ライフを削除
				m_pLife[nCntLife]->Release();
				m_pLife[nCntLife] = NULL;
				break;
			}
		}
	}
	if (m_fLife < 0)
	{// ライフがゼロになったら
		m_bAlive = false;
		m_bUse = false;
	}
}

void CPlayer::PlayerBullet(void)
{
	for (int nCntWeapon = 0; nCntWeapon < PLAYER_BULLET_STOCK; nCntWeapon++)
	{
		if (m_pWeapon[nCntWeapon]->GetUse() != false)
		{// 使用した分のポリゴンが残っていたら
			m_nBullet -= 1;							// ストックの減らす
			m_pWeapon[nCntWeapon]->SetUse(false);	// そのポリゴンを使用していない状態にする
			break;
		}
	}
}

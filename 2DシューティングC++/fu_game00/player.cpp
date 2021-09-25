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

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PLAYER_MOVE					(5.0f)
#define PLAYER_JUNP					(20.0f)
#define PLAYER_BULLET				(CBullet::BULLET_PLAYER)
#define PLAYER_RETURN_FLOOR			(SCREEN_HEIGHT - m_size.y)
#define MOVE_DECELERATION			(D3DXVECTOR3(1.0f, 0.02f, 0.0f))

#define BULLET_SIZE					(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define BULLET_MOVE					(10.0f)
#define BULLET_MOVE_RIGHT			(D3DXVECTOR3(BULLET_MOVE, 0.0f, 0.0f))
#define BULLET_MOVE_LEFT			(D3DXVECTOR3(-BULLET_MOVE, 0.0f, 0.0f))

#define GRAVITY						(1.0f)
#define PLAYER_COLL_POS				(D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, m_pos.z))

#define MESH_GAME					(CGame::GetMesh())
#define MESH_TUTORIAL				(CTutorial::GetMesh())

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene2D(OBJ_PLAYER)
{
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
		pPlayer->m_fStayTime = 0.0f;	// とどまっている時間
		pPlayer->m_col = WhiteColor;	// 白カラー
		pPlayer->m_bUse = true;			// 使用中
		pPlayer->m_pShield = NULL;		// シールド
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

	// ポリゴンの生成
	CScene2D::Init(m_pos, m_size);
	CScene2D::CreateTexture("data/TEXTURE/player0.png");
	CScene2D::SetTex(m_tex, m_number);
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

	// アクション
	PlayerAction();	

	// 移動
	PlayerMove();

	// 移動量の加算
	m_move.y += GRAVITY;
	m_pos += m_move;

	// 移動量の制御
	PlayerMoveControl();

	// 位置の制御
	PosControl();	

	// アニメーション
	PlayerAnime();

	CScene2D::SetUse(m_bUse);	// 存在している
	CScene2D::SetPos(m_pos);	// 位置の設定（更新）
	CScene2D::SetCol(m_col);	// 色の設定（更新）
	m_pShield->SetPos(m_pos);
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

	/* 弾の移動 */
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD6) == true)	// トリガー・Kが押されたとき
	{
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_RIGHT, PLAYER_BULLET);	// バレットの生成
	}
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_NUMPAD4) == true)	// トリガー・Kが押されたとき
	{
		CBullet::Create(m_pos, BULLET_SIZE, BULLET_MOVE_LEFT, PLAYER_BULLET);	// バレットの生成
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
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_D) == true)	// プレス・Dが押されたとき
	{
		m_nAnimeCnt++;
		m_move.x += PLAYER_MOVE;	// 移動量の更新
	}
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_A) == true)	// プレス・Aが押されたとき
	{
		m_nAnimeCnt++;
		m_move.x -= PLAYER_MOVE;	// 移動量の更新
	}

	/* プレイヤーのジャンプ */
	if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	// トリガー・SPACEが押されたとき
	{
		if (m_bJunp == false)
		{// ジャンプ可能ならば
			m_move.y -= PLAYER_JUNP;// 移動量の更新
			m_bJunp = true;			// ジャンプ中
		}
	}
	/* プレイヤーの落下減速 */
	if (m_bJunp == true)
	{// ジャンプ中かつ、重力がプラスの時
		if (pKey->GetState(CKey::STATE_PRESSE, DIK_W) == true)	// トリガー・SPACEが押されたとき
		{
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

	if (m_move.x == 0.0f && m_bJunp != true)
	{// 移動していなかったら
		m_fStayTime++;	// カウントUP
	}
	if (m_move.x != 0.0f || m_bJunp != false)
	{// 移動していたら
		m_fStayTime = 0.0f;
	}
	if (m_fStayTime >= 120.0f)
	{// 時間が来たら
		m_bStay = true;
	}
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
			D3DXVECTOR3 m_posOld = CCollision::WaveCollision(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA, CCollision::TYPE_COL_Y);	// 戻す分を算出
			m_move.y = 0.0f;					// 重力ゼロ
			m_bJunp = false;					// ジャンプ可能
			m_pos.y = m_posOld.y - m_size.y;	// 画面内まで移動させる
			break;
		}
	}
}

//=============================================================================
// プレイヤーのアニメージョン
//=============================================================================
void CPlayer::PlayerAnime(void)
{
	if ((m_nAnimeCnt % 10) == 1)
	{
		m_number.x++;
		if (((int)m_number.x + 1 % 2) == 0)
		{
			m_number.x = 0.0f;
		}
		CScene2D::SetTex(m_tex, m_number);
	}
}

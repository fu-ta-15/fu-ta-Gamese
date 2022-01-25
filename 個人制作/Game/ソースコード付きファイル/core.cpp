//*****************************************************************************
//
// コア処理 [core.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "core.h"
#include "move.h"
#include "game.h"
#include "boss.h"
#include "manager.h"
#include "tutorial.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCore::CCore(Priority nPriority) : CScene2D(nPriority)
{
	m_bUse = true;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCore::~CCore()
{
}

//=============================================================================
// コアの生成
//=============================================================================
CCore * CCore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,int nTime, Priority nPriority)
{
	CCore *pCore = NULL;

	if (pCore == NULL)
	{// NULLチェック
		pCore = new CCore(nPriority);
		pCore->m_nMoveCnt = nTime;
		pCore->Init();
		pCore->SetPos(pos);
		pCore->SetSize(size);
	}

	return pCore;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CCore::Init(void)
{
	CScene2D::Init();
	CScene2D::SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CScene2D::CreateTexture("data/TEXTURE/Particle04_bokashi_hard.png");
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCore::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCore::Update(void)
{
	CBoss *pBoss = NULL;
	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
	case CManager::MODE_GAME:
		// ボスの情報
		pBoss = CGame::GetBoss();
		break;

	default:
		break;
	}

	D3DXVECTOR3 pos = this->GetPos();
	D3DXVECTOR3 size = this->GetSize();
	
	m_nMoveCnt++;

	pos.x = Move::SinWave(pBoss->GetPos().x, pBoss->GetSize().x*1.5f, 150.0f, (float)m_nMoveCnt);
	pos.y = Move::CosWave(pBoss->GetPos().y, pBoss->GetSize().y*1.5f, 150.0f, (float)m_nMoveCnt);

	CScene2D::Update();
	CScene2D::SetPos(pos);
	CScene2D::SetSize(size);
}

//=============================================================================
// 描画処理
//=============================================================================
void CCore::Draw(void)
{
	if (m_bUse == true)
	{
		CScene2D::Draw();
	}
}


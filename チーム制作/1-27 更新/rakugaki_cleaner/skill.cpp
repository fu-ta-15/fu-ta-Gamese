//=============================================================================
//
// スキル処理 [skill.cpp]
// Author : 
//
//=============================================================================
#include "skill.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "eraserdust.h"
#include "eraser.h"
#include "base.h"
#include "game.h"
#include "graffiti_manager.h"

#include "debugproc.h"

//=============================================================================
// CSkillのコンストラクタ
//=============================================================================
CSkill::CSkill(int nPriority) : CScene3D(nPriority)
{
	// 変数の初期化
}

//=============================================================================
// CSkillのデストラクタ
//=============================================================================
CSkill::~CSkill()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CSkill *CSkill::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CSkill *pSkill;
	pSkill = new CSkill(CScene::PRIORITY_UI);

	// NULLチェック
	if (pSkill != NULL)
	{
		// 初期化処理
		pSkill->Init(pos, size);

		// オブジェクトタイプの設定
		pSkill->SetObjType(OBJTYPE_SKILL);
	}

	return pSkill;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSkill::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_nCnt = 0;

	// CScene2Dの初期化処理
	CScene3D::Init(pos, size);

	// 色の指定
	SetCol(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f));

	// テクスチャ管理情報の取得
	CTexture *pTexture = CManager::GetTexture();

	if (pTexture != NULL)
	{
		// テクスチャの割り当て
		BindTexture(pTexture->GetAddress(pTexture->SetTexture("data/TEXTURE/effect_explosion.png")));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSkill::Uninit(void)
{
	// CScene2Dの終了処理
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSkill::Update(void)
{
	// CScene2Dの更新処理
	CScene3D::Update();

	// カウント更新
	m_nCnt++;

	// 10フレーム後削除
	if (m_nCnt == 10)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CSkill::Draw(void)
{
	// CScene2Dの描画処理
	CScene3D::Draw();
}

//=============================================================================
// スキルの当たり判定処理
//=============================================================================
bool CSkill::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size,D3DXVECTOR3 *pMove, D3DXVECTOR3 moveBase)
{
	// 位置とサイズの取得
	D3DXVECTOR3 posSkill = GetPosition();
	D3DXVECTOR2 sizeSkill = GetSize();

	// オブジェクトとスキル範囲の中心との距離を算出
	D3DXVECTOR3 vec = pos - posSkill;
	float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

	// 範囲内なら飛ばす
	if (fLength < sizeSkill.x / 2.0f)
	{
		// 角度を求める
		float fAngle = atan2f(vec.x, vec.z);

		// 割合を求める
		D3DXVECTOR2 ratio;
		float fRatio = (sizeSkill.x - fLength) / sizeSkill.x;
		ratio.x = fLength / (sizeSkill.x / 2.0f);
		ratio.y = (sizeSkill.x - fLength) / (sizeSkill.x);

		// 移動量を求める
		pMove->x = moveBase.x * sinf(fAngle) * fRatio;
		pMove->y = moveBase.y * fRatio;
		pMove->z = moveBase.z * cosf(fAngle) * fRatio;

		return true;
	}

	return false;
}
//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "result_ui.h"
#include "manager.h"
#include "result.h"
#include "player_result.h"
#include "keep_object.h"
#include "player_marker.h"
#include "texture.h"
#include "score.h"
#include "number.h"
#include "texture.h"

#include "debugproc.h"

//=============================================================================
// CResult_UI�̃R���X�g���N�^
//=============================================================================
CResult_UI::CResult_UI(int nPriority) : CScene(nPriority)
{

}

//=============================================================================
// CResult_UI�̃f�X�g���N�^
//=============================================================================
CResult_UI::~CResult_UI()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult_UI::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �f�[�^�ۑ��p�I�u�W�F�N�g�̎擾
	CKeepObject *pKeepObject = CManager::GetKeepObject();
	int nNumPlayer = pKeepObject->GetNumPlayer();

	CPlayer_Result **pPlayer_Game = CResult::GetPlayer();

	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		// �e�N�X�`���Ǘ����̎擾
		CTexture *pTex = CManager::GetTexture();

		if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_COOPERATION)
		{
			InitDisplayAchivementPlayer(pPlayer_Game[nCnt]->GetPlayerMarker()->GetPosition(), nCnt);
		}
	}

	if (InitDisplayAchivement())
	{
		m_bAchivement = true;
	}
	else
	{
		m_bAchivement = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult_UI::Uninit(void)
{
	// �j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult_UI::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult_UI::Draw(void)
{
	// �f�[�^�ۑ��p�I�u�W�F�N�g�̎擾
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_COOPERATION)
	{
		m_pNumberDecimal->Draw();

		int nNumPlayer = pKeepObject->GetNumPlayer();

		for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
		{
			m_pNumberDecimalPlayer[nCnt]->Draw();
		}
	}
}

//=============================================================================
// �B��������������
//=============================================================================
bool CResult_UI::InitDisplayAchivement(void)
{
	// �f�[�^�ۑ��p�I�u�W�F�N�g�̎擾
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	// �e�N�X�`���Ǘ����擾
	CTexture *pTex = CManager::GetTexture();

	// ���N�K�L�Q�[�W�̐��������̐���
	m_pScoreInteger = CScore::Create(D3DXVECTOR3(640.0f, 150.0f, 0.0f),
		D3DXVECTOR2(10.0f, 20.0f),
		14.5f,
		CScore::SCOREPLACEMENT_CENTERED,
		pTex->SetTexture("data/TEXTURE/SCORE_LOGO04.png"));

	// ���N�K�L�Q�[�W�̏��������̐���
	m_pNumberDecimal = CNumber::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(10.0f, 20.0f));
	m_pNumberDecimal->SetTexNum(pTex->SetTexture("data/TEXTURE/SCORE_LOGO04.png"));

	// ���S���̐���
	m_pLogoComma = CScene2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/SCORE_LOGO06.png"));
	m_pLogoComma->SetTexSheet(D3DXVECTOR2(2, 1));
	m_pLogoComma->SetTex(D3DXVECTOR2(0, 0));

	m_pLogoPercent = CScene2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/SCORE_LOGO06.png"));
	m_pLogoPercent->SetTexSheet(D3DXVECTOR2(2, 1));
	m_pLogoPercent->SetTex(D3DXVECTOR2(1, 0));

	// �B�����̎擾
	float fRatio = pKeepObject->GetAchivement();
	m_pScoreInteger->SetScore((int)(fRatio * 100.0f));
	m_pScoreInteger->SetPosition(D3DXVECTOR3(640.0f, 100.0f, 0.0f));

	m_pNumberDecimal->SetNumber((int)((fRatio * 10000.0f) / powf(10.0f, 1.0f)));
	D3DXVECTOR3 pos = m_pScoreInteger->GetPosition(m_pScoreInteger->GetDigit() - 1);
	m_pNumberDecimal->SetPosition(D3DXVECTOR3(pos.x + 20.5f, pos.y, pos.z));

	D3DXVECTOR3 posNum = m_pNumberDecimal->GetPosition();
	m_pLogoComma->SetPosition(D3DXVECTOR3(posNum.x - 11.5f, posNum.y + 6.0f, posNum.z));
	m_pLogoPercent->SetPosition(D3DXVECTOR3(posNum.x + 15.5f, posNum.y + 6.0f, posNum.z));

	if (fRatio < 1.0f)
	{
		return false;
	}

	return true;
}

//=============================================================================
// �v���C���[�B��������������
//=============================================================================
void CResult_UI::InitDisplayAchivementPlayer(D3DXVECTOR3 pos, int nCnt)
{
	// �f�[�^�ۑ��p�I�u�W�F�N�g�̎擾
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	// �e�N�X�`���Ǘ����擾
	CTexture *pTex = CManager::GetTexture();

	// ���N�K�L�Q�[�W�̐��������̐���
	m_pScoreIntegerPlayer[nCnt] = CScore::Create(D3DXVECTOR3(640.0f, 150.0f, 0.0f),
		D3DXVECTOR2(10.0f, 20.0f),
		14.5f,
		CScore::SCOREPLACEMENT_CENTERED,
		pTex->SetTexture("data/TEXTURE/SCORE_LOGO04.png"));

	// ���N�K�L�Q�[�W�̏��������̐���
	m_pNumberDecimalPlayer[nCnt] = CNumber::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(10.0f, 20.0f));
	m_pNumberDecimalPlayer[nCnt]->SetTexNum(pTex->SetTexture("data/TEXTURE/SCORE_LOGO04.png"));

	// ���S���̐���
	m_pLogoCommaPlayer[nCnt] = CScene2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/SCORE_LOGO06.png"));
	m_pLogoCommaPlayer[nCnt]->SetTexSheet(D3DXVECTOR2(2, 1));
	m_pLogoCommaPlayer[nCnt]->SetTex(D3DXVECTOR2(0, 0));

	m_pLogoPercentPlayer[nCnt] = CScene2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture("data/TEXTURE/SCORE_LOGO06.png"));
	m_pLogoPercentPlayer[nCnt]->SetTexSheet(D3DXVECTOR2(2, 1));
	m_pLogoPercentPlayer[nCnt]->SetTex(D3DXVECTOR2(1, 0));

	// �B�����̎擾
	float fRatio = (float)pKeepObject->GetDeleteGraffiti(nCnt) / (float)pKeepObject->GetTotalGraffiti();
	m_pScoreIntegerPlayer[nCnt]->SetScore((int)(fRatio * 100.0f));
	m_pScoreIntegerPlayer[nCnt]->SetPosition(D3DXVECTOR3(pos.x - 10.0f, pos.y - 400.0f, pos.z));

	m_pNumberDecimalPlayer[nCnt]->SetNumber((int)((fRatio * 10000.0f) / powf(10.0f, 1.0f)));
	D3DXVECTOR3 posScore = m_pScoreIntegerPlayer[nCnt]->GetPosition(m_pScoreIntegerPlayer[nCnt]->GetDigit() - 1);
	m_pNumberDecimalPlayer[nCnt]->SetPosition(D3DXVECTOR3(posScore.x + 20.5f, posScore.y, posScore.z));

	D3DXVECTOR3 posNum = m_pNumberDecimalPlayer[nCnt]->GetPosition();
	m_pLogoCommaPlayer[nCnt]->SetPosition(D3DXVECTOR3(posNum.x - 11.5f, posNum.y + 6.0f, posNum.z));
	m_pLogoPercentPlayer[nCnt]->SetPosition(D3DXVECTOR3(posNum.x + 15.5f, posNum.y + 6.0f, posNum.z));
}
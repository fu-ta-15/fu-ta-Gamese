//=============================================================================
//
// �Q�[�W�Ǘ����� [gauge_manager.cpp]
// Author : 
//
//=============================================================================
#include "gauge_manager.h"

#include "manager.h"
#include "base.h"
#include "game.h"
#include "player_game.h"
#include "gauge.h"
#include "scoregauge.h"
#include "skillgauge.h"
#include "graffiti_gauge.h"
#include "graffiti_manager.h"
#include "keep_object.h"
#include "ranking_namespace.h"

#include "player_marker.h"
#include "crown.h"
#include "texture.h"
#include "renderer.h"

//=============================================================================
// CGaugeManager�̃R���X�g���N�^
//=============================================================================
CGaugeManager::CGaugeManager()
{
	// �����o�ϐ��̏�����
	m_nNumPlayer = 0;
}

//=============================================================================
// CGaugeManager�̃f�X�g���N�^
//=============================================================================
CGaugeManager::~CGaugeManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGaugeManager::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_nCntRank_One = 0;
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// �v���C���[���̕ۑ�
		m_nNumPlayer = pGame->GetNumPlayer();
	}

	// �����̏�����
	CTexture *pTex = CManager::GetTexture();

	m_pCrown = new CCrown*[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_pCrown[nCnt] = CCrown::Create({ 0.0f,0.0f,0.0f }, { 50.0f,50.0f }, pTex->SetTexture("data/TEXTURE/6912.png"));
	}

	// �v���C���[�}�[�J�[�̏�����
	m_pPlayerMarker = new CPlayerMarker*[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_pPlayerMarker[nCnt] = CPlayerMarker::Create({ 0.0f,0.0f,0.0f }, { 20.0f,20.0f }, {4,1},pTex->SetTexture("data/TEXTURE/player_Logo00.png"));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGaugeManager::Uninit(void)
{
	// �Q�[�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		// �ۑ�
		CGame *pGame = (CGame*)pBase;
		CKeepObject *pKeepObject = CManager::GetKeepObject();

		// ���N�K�L�Ǘ������擾
		CGraffitiManager *pGraffiti = pGame->GetGraffitiManager();

		// �B�����̕ۑ�
		int nNumTotalGraf = pGraffiti->GetTotalGraffiti();
		int nNumCurGraf = pGraffiti->GetCurGraffiti();
		pKeepObject->SetAchivement((float)(nNumTotalGraf - nNumCurGraf) / (float)nNumTotalGraf);
		pKeepObject->SetTotalGraffiti(nNumTotalGraf);
	}

	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGaugeManager::Update(void)
{
	// ������
	m_nCntRank_One = 0;

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_pCrown[nCnt]->SetDisplay(false);
	}

	// �X�R�A�Q�[�W����
	ScoreGauge();

	// �X�L���Q�[�W����
	SkillGauge();

	// ���N�K�L�Q�[�W����
	GraffitiGauge();

	// �Q�[�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// �v���C���[�����擾
		CPlayer_Game **pPlayer = pGame->GetPlayer();

		// �Q�[�W�𑝂₷
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// �X�R�A�Q�[�W�����擾
				CScoreGauge *pScoreGauge = pPlayer[nCnt]->GetScoreGauge();

				// �ʒu�ƃT�C�Y���̎擾
				D3DXVECTOR3 posScore = pScoreGauge->GetPosDef();
				D3DXVECTOR2 sizeScore = pScoreGauge->GetSizeDef();

				// �ʒu���T�C�Y������
				posScore.x -= sizeScore.x / 2.0f;

				// �ʒu�̕ύX
				m_pPlayerMarker[nCnt]->SetPosition(D3DXVECTOR3(posScore.x + 35.0f, posScore.y - 20.0f, 0.0f));
				m_pPlayerMarker[nCnt]->SetTex({ (float)nCnt,0 });
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGaugeManager::Draw(void)
{	

}

//=============================================================================
// �X�R�A�Q�[�W����
//=============================================================================
void CGaugeManager::ScoreGauge(void)
{
	// �ϐ��錾
	int nNumDeleteGraffiti[4] = {};
	int nNumMax = 0;					// ��ԑ傫���l�̃v���C���[�ԍ��i�[�p

	// �Q�[�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// �v���C���[�����擾
		CPlayer_Game **pPlayer = pGame->GetPlayer();

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// ���������N�K�L�����擾
				nNumDeleteGraffiti[nCnt] = pPlayer[nCnt]->GetDeleteGraffiti();
			}
		}

		// ���ʊm��
		int *nNumberRank = new int[m_nNumPlayer];

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			nNumberRank[nCnt] = 0;
		}
		RankingSpace::SetRanking(&nNumberRank[0], &nNumDeleteGraffiti[0], m_nNumPlayer);

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			pPlayer[nCnt]->SetRank(nNumberRank[nCnt]);
		}

		// �l���r
		for (int nCnt = 0; nCnt < m_nNumPlayer - 1; nCnt++)
		{
			for (int nCntSub = nCnt + 1; nCntSub < m_nNumPlayer; nCntSub++)
			{
				if (nNumDeleteGraffiti[nCnt] < nNumDeleteGraffiti[nCntSub])
				{
					int nSub = nNumDeleteGraffiti[nCnt];
					nNumDeleteGraffiti[nCnt] = nNumDeleteGraffiti[nCntSub];
					nNumDeleteGraffiti[nCntSub] = nSub;
				}
			}
		}

		// �Q�[�W�𑝂₷
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// �X�R�A�Q�[�W�����擾
				CScoreGauge *pScoreGauge = pPlayer[nCnt]->GetScoreGauge();

				// �ʒu�ƃT�C�Y���̎擾
				D3DXVECTOR3 posScore = pScoreGauge->GetPosDef();
				D3DXVECTOR2 sizeScore = pScoreGauge->GetSizeDef();

				// �ʒu���T�C�Y������
				posScore.x -= sizeScore.x / 2.0f;

				// ���������N�K�L����0����Ȃ��Ȃ珈��
				if (pPlayer[nCnt]->GetDeleteGraffiti() != 0)
				{
					if (nNumDeleteGraffiti[0] == pPlayer[nCnt]->GetDeleteGraffiti())
					{
						// ��ԏ����Ă�ꍇ
						pScoreGauge->AddPosTex(D3DXVECTOR3(posScore.x + sizeScore.x, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), 2, 3);
						pScoreGauge->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

						// �ʒu�̕ύX
						m_pCrown[m_nCntRank_One]->SetPosition(D3DXVECTOR3(posScore.x + 150.0f, posScore.y - 30.0f,0.0f));
						m_pCrown[m_nCntRank_One]->SetDisplay(true);

						// �J�E���g
						m_nCntRank_One++;
					}
					else
					{
						// ���������߂�
						float fRatio = (float)pPlayer[nCnt]->GetDeleteGraffiti() / (float)nNumDeleteGraffiti[0];

						// ��ʈȉ��̏ꍇ
						pScoreGauge->AddPosTex(D3DXVECTOR3(posScore.x + sizeScore.x * fRatio, 0.0f, 0.0f), D3DXVECTOR2(fRatio, 0.0f), 2, 3);
						pScoreGauge->SetCol(D3DXCOLOR(1.0f, 0.55f, 0.15f, 1.0f));
					}
				}
			}
		}
	}
}

//=============================================================================
// �X�L���Q�[�W����
//=============================================================================
void CGaugeManager::SkillGauge(void)
{
	// �ϐ��錾
	int nNumDeleteGraffiti[4] = {};

	// �Q�[�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// �v���C���[�����擾
		CPlayer_Game **pPlayer = pGame->GetPlayer();

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// ���������N�K�L�����擾
				nNumDeleteGraffiti[nCnt] = pPlayer[nCnt]->GetDeleteGraffiti();
			}
		}

		// �Q�[�W�𑝂₷
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (pPlayer[nCnt] != NULL)
			{
				// �X�L���Q�[�W
				CSkillGauge *pSkillGauge = pPlayer[nCnt]->GetSkillGauge();

				// �ʒu�ƃT�C�Y���̎擾
				D3DXVECTOR3 posSkill = pSkillGauge->GetPosDef();
				D3DXVECTOR2 sizeSkill = pSkillGauge->GetSizeDef();

				// �ʒu���T�C�Y������
				posSkill.x -= sizeSkill.x / 2.0f;

				// ���������N�K�L����0�Ȃ珈��
				if (pPlayer[nCnt]->GetCntSkillGauge() == 0)
				{
					// �Q�[�W��L�΂�
					pSkillGauge->AddPosTex(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 2, 3);

					// �F�̕ύX
					pSkillGauge->SetCol(D3DXCOLOR(1.0f, 0.55f, 0.15f, 1.0f));
				}
				else if (pPlayer[nCnt]->GetMaxSkillGauge() == false)
				{
					// ���������߂�
					float fRatio = (float)pPlayer[nCnt]->GetCntSkillGauge() / (float)DELETE_GARAFFITI_NUMBER_MAX;

					// �Q�[�W��L�΂�
					pSkillGauge->AddPosTex(D3DXVECTOR3(posSkill.x + sizeSkill.x * fRatio, 0.0f, 0.0f), D3DXVECTOR2(fRatio, 0.0f), 2, 3);

					if (pPlayer[nCnt]->GetCntSkillGauge() == DELETE_GARAFFITI_NUMBER_MAX)
					{
						// �F�̕ύX
						pSkillGauge->SetCol(D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f));

						// Max�ɂȂ������Ƃ�`����
						pPlayer[nCnt]->SetMaxSkillGauge(true);
					}
				}
			}
		}
	}
}

//=============================================================================
// ���N�K�L�Q�[�W����
//=============================================================================
void CGaugeManager::GraffitiGauge(void)
{
	// �Q�[�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;

		// ���N�K�L�Ǘ������擾
		CGraffitiManager *pGraffiti = pGame->GetGraffitiManager();

		if (pGraffiti != NULL)
		{
			// �����ƌ��݂̐����擾
			int nNumTotalGraf = pGraffiti->GetTotalGraffiti();
			int nNumCurGraf = pGraffiti->GetCurGraffiti();

			// �����ƌ��݂̐����Q�Ƃ����������߂�
			float fRatio = 0.0f;

			if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_COOPERATION)
			{
				fRatio = (float)(nNumTotalGraf - nNumCurGraf) / (float)nNumTotalGraf;
			}
			else if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_BATTLE)
			{
				fRatio = (float)(nNumCurGraf) / (float)nNumTotalGraf;
			}

			// �ʒu�ƃT�C�Y���̎擾
			D3DXVECTOR3 posGraf = m_pGraffitiGauge->GetPosDef();
			D3DXVECTOR2 sizeGraf = m_pGraffitiGauge->GetSizeDef();

			// �ʒu�ɃT�C�Y�����Z
			posGraf.x -= sizeGraf.x / 2.0f;

			// �E�[��X���W
			float fPosX = posGraf.x + sizeGraf.x * fRatio;

			// �Q�[�W�����炷
			m_pGraffitiGauge->AddPosTex(D3DXVECTOR3(fPosX,0.0f,0.0f), D3DXVECTOR2(0,0), 2, 3);

			// �����ƈʒu��ۑ�
			m_pGraffitiGauge->SetRatio(fRatio);
			m_pGraffitiGauge->SetPosXRightSide(fPosX);
		}
	}
}
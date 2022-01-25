//=============================================================================
//
// �f�[�^�ۑ��p���� [keeobject.cpp]
// Author : 
//
//=============================================================================
#include "keep_object.h"
#include "manager.h"
#include "renderer.h"

#include "load_player_data.h"
#include "motion.h"
#include "ranking_namespace.h"
#include "debugproc.h"

//=============================================================================
// CKeepObject�̃R���X�g���N�^
//=============================================================================
CKeepObject::CKeepObject()
{

}

//=============================================================================
// CDebugProc�̃f�X�g���N�^
//=============================================================================
CKeepObject::~CKeepObject()
{

}

//=============================================================================
// ����������
//=============================================================================
void CKeepObject::Init(void)
{
	// �v���C���[�f�[�^�ǂݍ��ݏ��̏�����
	m_pLoadPlayerData = new CLoadPlayerData;

	if (m_pLoadPlayerData != NULL)
	{
		m_pLoadPlayerData->Init();
	}

	m_gamemode = GAMEMODE_BATTLE;
}

//=============================================================================
// �I������
//=============================================================================
void CKeepObject::Uninit(void)
{
	// �������̔j��
	DeleteMem();

	// �v���C���[�f�[�^�ǂݍ��ݏ��̔j��
	if (m_pLoadPlayerData != NULL)
	{
		m_pLoadPlayerData->Uninit();
		delete m_pLoadPlayerData;
		m_pLoadPlayerData = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CKeepObject::Update(void)
{

}

//=============================================================================
// �v���C�l����ݒ�
//=============================================================================
void CKeepObject::SetNumPlayer(int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;

	// �������̊m��
	m_nNumDeleteGraffiti = new int[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_nNumDeleteGraffiti[nCnt] = 0;
	}
}

//=============================================================================
// �������̔j��
//=============================================================================
void CKeepObject::DeleteMem(void)
{
	// ���������N�K�L����ۑ�����|�C���^�̔j��
	if (m_nNumDeleteGraffiti != NULL)
	{
		delete[] m_nNumDeleteGraffiti;
		m_nNumDeleteGraffiti = NULL;
	}

	// ���ʂ�ۑ�����|�C���^�̔j��
	if (m_nRank != NULL)
	{
		delete[] m_nRank;
		m_nRank = NULL;
	}

	// ���ʂ�ۑ�����|�C���^�̔j��
	if (m_nNumberRank != NULL)
	{
		delete[] m_nNumberRank;
		m_nNumberRank = NULL;
	}
}

//=============================================================================
// �����L���O�̊m��
//=============================================================================
void CKeepObject::Rank(void)
{
	// �������̊m��
	m_nRank = new int[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_nRank[nCnt] = 0;
	}

	m_nNumberRank = new int[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_nNumberRank[nCnt] = 0;
	}

	// ���ʕt��
	RankingSpace::SetRanking(&m_nRank[0], &m_nNumDeleteGraffiti[0], m_nNumPlayer);
	RankingSpace::SetSameRate(&m_nNumberRank[0], &m_nNumDeleteGraffiti[0], m_nNumPlayer);
}
//*****************************************************************************
//
// �`���[�g���A������ [tutorial.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "tutorial.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "mesh3D.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
CMesh3D*	CTutorial::m_pTutorialMesh3D[3] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial() : CScene(OBJ_NONE)
{
	// �����o�ϐ��̐ݒ�
	m_nCntCycleLimit = 180;
	m_bNextPage = false;
	m_nPageID = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// �`���[�g���A���̐���
//=============================================================================
CTutorial * CTutorial::Create(void)
{
	// �������m��
	CTutorial* pTutorial = new CTutorial();

	// ����������
	pTutorial->Init();

	return pTutorial;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// �`���[�g���A���̉��
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pTutorialMesh3D[nCnt] = CMesh3D::Create(40, 40, D3DXVECTOR3(10.0f, (-nCnt*0.1f) , -5.0f), D3DXVECTOR3(150.0f, 0.0f, 150.0f));
	}
	m_pTutorialMesh3D[0]->CreateTexture("data/TEXTURE/tutorial.png");
	m_pTutorialMesh3D[1]->CreateTexture("data/TEXTURE/tutorial2.png");
	m_pTutorialMesh3D[2]->CreateTexture("data/TEXTURE/tutorial3.png");

	// �T�E���h�̊J�n
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_BGM001);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �T�E���h�̒�~
	CSound *pSound = CManager::GetSound();
	pSound->StopSound();

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	// �L�[���
	CKey *pKey = CManager::GetKey();	   

	// �t�F�[�h���
	CFade::FADE Fade = CFade::GetFade();   

	// SPACE�L�[�������ꂽ��
	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE) && Fade == CFade::FADE_NONE)
	{
		// GAME�EMODE��
		CManager::GetFade()->SetFade(CManager::MODE_GAME);	
	}

	// RETURN�L�[�������ꂽ��
	if (pKey->GetState(CKey::STATE_RELEASE, DIK_RETURN) && Fade == CFade::FADE_NONE)
	{
		// ���̃y�[�W�ɍs�����}
		m_bNextPage = true;
	}

	// ���̃y�[�W��
	if (m_bNextPage)
	{
		// ���b�V���̉�]
		m_pTutorialMesh3D[m_nPageID]->MeshCycleMove(m_nCntCycleLimit);

		// �E�ɉ�]�����邽�߂Ƀ}�C�i�X�̉��Z
		m_nCntCycleLimit--;
	}
	if (m_nCntCycleLimit < 0)
	{
		m_bNextPage = false;
		m_nCntCycleLimit = 180;
		m_nPageID++;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}

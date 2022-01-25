//=============================================================================
//
// �e�N�X�`���Ǘ����� [texture.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "stdio.h"

#include "debugproc.h"

//=============================================================================
// CTexture�̃R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{
	// �����o�ϐ��̏�����
	m_pTexture = NULL;
}

//=============================================================================
// CTexture�̃f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// �ǂݍ��ݏ���
//=============================================================================
HRESULT CTexture::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �������̊m��
	m_pTexture = (LPDIRECT3DTEXTURE9) new LPDIRECT3DTEXTURE9[128];
	m_apTexFileName = new char*[128];

	for (int nCnt = 0; nCnt < 128; nCnt++)
	{
		m_apTexFileName[nCnt] = new char[128];
	}

	m_nNumtextureInfo = -1;
	m_nNumFileName = 0;

	return S_OK;
}

//=============================================================================
// �j������
//=============================================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumtextureInfo; nCntTex++)
	{
		if (*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex] != NULL)
		{
			(*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex])->Release();
			*(LPDIRECT3DTEXTURE9*)&m_pTexture[nCntTex] = NULL;
		}
	}
	delete[] m_pTexture;
	m_pTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void CTexture::Update(void)
{
	CDebugProc::Print("\n--- �e�N�X�`����� ---\n");
	CDebugProc::Print("�e�N�X�`���� : %d\n",m_nNumtextureInfo + 1);
}

//=============================================================================
// �e�N�X�`�����擾
//=============================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return *(LPDIRECT3DTEXTURE9*)&m_pTexture[nIdx];
}

//=============================================================================
// �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
int CTexture::SetTexture(char *pTexFileName)
{
	// �������O���Ȃ����T��
	for (int nCnt = 0 ; nCnt < m_nNumFileName; nCnt++)
	{
  		if (strcmp(&m_apTexFileName[nCnt][0],&pTexFileName[0]) == 0)
		{
			return nCnt;
		}
	}

	// �e�N�X�`���t�@�C���l�[���̕ۑ�
	strcpy(&m_apTexFileName[m_nNumFileName][0], pTexFileName);
	m_nNumFileName++;

	// �e�N�X�`�����̃J�E���g
	m_nNumtextureInfo++;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
 	D3DXCreateTextureFromFile(pDevice, pTexFileName, (LPDIRECT3DTEXTURE9*)&m_pTexture[m_nNumtextureInfo]);

	return m_nNumtextureInfo;
}

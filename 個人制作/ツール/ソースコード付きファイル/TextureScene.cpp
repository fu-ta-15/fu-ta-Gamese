//*****************************************************************************
//
// �e�N�X�`������ [TextureScene.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "TextureScene.h"
#include "manager.h"
#include "renderer.h"
#include "keyinput.h"

//-----------------------------------------------------------------------------
// �ÓI�����o�ϐ�
//-----------------------------------------------------------------------------
list<CTextureScene*> CTextureScene::m_TextureScene;
int CTextureScene::m_nListSize = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTextureScene::CTextureScene()
{
	m_sLink = NULL;
	m_pTexture = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTextureScene::~CTextureScene()
{

}

//=============================================================================
// �e�N�X�`���̃��[�h
//=============================================================================
void CTextureScene::LoadTexture(void)
{

}

//=============================================================================
// �e�N�X�`���̃A�����[�h
//=============================================================================
void CTextureScene::UnLoadTexture(void)
{

}

//=============================================================================
// �h���b�v���ꂽ�e�N�X�`���t�@�C���̐���
//=============================================================================
void CTextureScene::FileDrop(const char * sTexName)
{
	// ���^�[���p�̃N���X�ϐ�
	CTextureScene *pTexScene = new CTextureScene;

	// ���������m�F��char�^�̕ϐ��𓮓I�Ɋm��
	size_t Length = strlen(sTexName);
	pTexScene->m_sLink = new char[Length];

	// ���O�̃R�s�[
	strcpy(pTexScene->m_sLink, sTexName);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, pTexScene->m_sLink, &pTexScene->m_pTexture);

	// NULL�`�F�b�N
	if (pTexScene->m_pTexture)
	{
		// �m�F�p
		printf("\n��������");

		// ���X�g�փv�b�V���o�b�N����
		m_TextureScene.push_back(pTexScene);
	}

	// ���g�̃T�C�Y���m�F
	m_nListSize = m_TextureScene.size();
}

//=============================================================================
// �h���b�v���ꂽ�e�N�X�`���t�@�C���̐���
//=============================================================================
CTextureScene * CTextureScene::GetTexScene(int nID)
{
	// �擪�̗v�f�̃C�e���[�^�[�̎擾
	list<CTextureScene*>::iterator it = m_TextureScene.begin();

	// �w�肳�ꂽ�Ԓn�̗v�f�܂Ői�߂�
	for (int nCnt = 0; nCnt < nID; nCnt++)
	{
		// �i�߂�
		it++;
	}

	// �w�肳�ꂽ�v�f��Ԃ�
	return *it;
}

//=============================================================================
// �e�N�X�`���̐���
//=============================================================================
CTextureScene *CTextureScene::TextureListIn(const char *sTexName)
{
	// ���^�[���p�̃N���X�ϐ�
	CTextureScene *pTexScene = new CTextureScene;

	// ���O�̃R�s�[
	pTexScene->m_sLink = (char*)sTexName;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, pTexScene->m_sLink, &pTexScene->m_pTexture);

	return pTexScene;
}

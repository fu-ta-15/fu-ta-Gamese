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
// �e�N�X�`���̐���
//=============================================================================
void CTextureScene::ListInTexture(const char * sTexName)
{
	// ���������m�F��char�^�̕ϐ��𓮓I�Ɋm��
	size_t Length = strlen("TEXTURE/") + strlen(sTexName);
	char *sLink = new char[Length];

	// ������̍���
	strcpy(sLink, "TEXTURE/");
	strcat(sLink, sTexName);

	// ���X�g�փv�b�V���o�b�N���邽�߂̕ϐ��ɏ����
	CTextureScene *pTexScene = TextureListIn(sLink);

	if (pTexScene->m_pTexture)
	{
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
void CTextureScene::FileDrop(const char * sTexName)
{
	// ���^�[���p�̃N���X�ϐ�
	CTextureScene *pTexScene = new CTextureScene;

	// ���O�̃R�s�[
	pTexScene->m_sLink = (char*)sTexName;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, pTexScene->m_sLink, &pTexScene->m_pTexture);

	if (pTexScene->m_pTexture)
	{
		printf("\n��������");
		// ���X�g�փv�b�V���o�b�N����
		m_TextureScene.push_back(pTexScene);
	}

	// ���g�̃T�C�Y���m�F
	m_nListSize = m_TextureScene.size();
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

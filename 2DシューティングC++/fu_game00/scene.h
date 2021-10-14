//*****************************************************************************
//
// �V�[������ [scene.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define MAX_OBJECT		(128)

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CScene
{
public:
	// �I�u�W�F�N�g�^�C�v
	typedef enum Priority	
	{
		OBJ_NONE = 0,	// �V���v���I�u�W�F�N�g
		OBJ_NONE2,	// �V���v���I�u�W�F�N�g
		OBJ_BULLET,		// �o���b�g�I�u�W�F�N�g
		OBJ_PLAYER,		// �v���C���[�I�u�W�F�N�g
		OBJ_BOSS,		// �{�X�I�u�W�F�N�g
		OBJ_ENEMY,		// �G�I�u�W�F�N�g
		OBJ_EFFECT,
		OBJ_MAX
	}ObjectType;	
	
	enum PauseType
	{
		PAUSE_BG = 0,
		PAUSE_BUTTON1,
		PAUSE_BUTTON2,
		PAUSE_BUTTON3,
		PAUSE_MAX
	};

	CScene(Priority type);					
	CScene(PauseType type);					
	CScene(bool bpause);					

	// ���z�����֐�
	virtual ~CScene();						
	virtual HRESULT Init(void)	= 0;		
	virtual void Uninit(void)	= 0;		
	virtual void Update(void)	= 0;		
	virtual void Draw(void)		= 0;		

	static void ReleaseAll(void);			
	static void UpdateAll(void);			
	static void DrawAll(void);				

	void Release(void);						
	void DeathRelease(void);				
	static void PauseRelease(void);			


	/* Set�֐� */
	void SetPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		// �ʒu�̐ݒ�
	void SetSize(D3DXVECTOR3 size)	{ m_size = size; }		// �T�C�Y�̐ݒ�
	void SetCol(D3DXCOLOR col)		{ m_col = col; }		// �F�̐ݒ�
	void SetBool(bool bflag)		{ m_bBool = bflag; }	// �����̃t���O

	/* Get�֐� */
	D3DXVECTOR3 GetPos(void)	{ return m_pos; }			// ����̃I�u�W�F�N�g�̈ʒu�擾
	D3DXVECTOR3 GetSize(void)	{ return m_size; }			// ����̃I�u�W�F�N�g�̃T�C�Y�擾
	D3DXCOLOR GetCol(void)		{ return m_col; }			// �F�̎擾
	bool GetBool(void)			{ return m_bBool; }			// �t���O�̎擾
	CScene *GetSceneNext(void)	{ return this->m_pNext; }	// ���̃V�[���I�u�W�F�N�g�̎擾
	static CScene *GetScene(Priority type) { 				// �V�[���I�u�W�F�N�g�̐擪�̎擾
		return m_pTop[type]; }

protected:

private:
	static CScene		*m_pTop[OBJ_MAX];			// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene		*m_pCur[OBJ_MAX];			// ���݁i�Ō���j�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene		*m_pPauseScene;				// �V�[�����~�߂�ÓI�ϐ�
	static CScene		*m_pPauseObj[PAUSE_MAX];
	CScene				*m_pPrev;					// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene				*m_pNext;					// ���̃I�u�W�F�N�g�ւ̃|�C���^
	Priority			 m_type;					// �I�u�W�F�N�^�C�v
	D3DXVECTOR3			 m_pos;						// �ʒu
	D3DXVECTOR3			 m_size;					// �T�C�Y
	D3DXCOLOR			 m_col;						// �F
	bool				 m_bBool;
	bool				 m_bDeath;					// ���S�t���O
};


#endif
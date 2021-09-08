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
	typedef enum ObjType	
	{
		OBJ_NONE = 0,	// �V���v���I�u�W�F�N�g
		OBJ_BULLET,		// �o���b�g�I�u�W�F�N�g
		OBJ_PLAYER,		// �v���C���[�I�u�W�F�N�g
		OBJ_ENEMY,		// �G�I�u�W�F�N�g
		OBJ_MAX
	}ObjectType;	
	
	enum PauseType
	{
		PAUSE_BG = 0,
		PAUSE_BUTTON1,
		PAUSE_BUTTON2,
		PAUSE_MAX
	};

	CScene();								// �R���X�g���N�^
	CScene(ObjType type);					// �I�[�o���[�h���ꂽ�R���X�g���N�^
	CScene(PauseType type);					// �I�[�o���[�h���ꂽ�R���X�g���N�^
	CScene(bool bpause);					// �I�[�o���[�h���ꂽ�R���X�g���N�^

	// ���z�����֐�
	virtual ~CScene();						// �f�X�g���N�^
	virtual HRESULT Init(void)	= 0;		// ����������
	virtual void Uninit(void)	= 0;		// �I������
	virtual void Update(void)	= 0;		// �X�V����
	virtual void Draw(void)		= 0;		// �`�揈��

	static void ReleaseAll(void);			// ���ׂẴI�u�W�F�N�g�폜
	static void UpdateAll(void);			// ���ׂẴI�u�W�F�N�g�̍X�V
	static void DrawAll(void);				// ���ׂẴI�u�W�F�N�g�̍X�V

	/* Set�֐� */
	void SetPos(D3DXVECTOR3 pos);			// �ʒu�̐ݒ�
	void SetSize(D3DXVECTOR3 size);			// �T�C�Y�̐ݒ�
	void SetCol(D3DXCOLOR col);				// �F�̐ݒ�

	/* Get�֐� */
	static int GetObjeNum(ObjType type);						// ����̃I�u�W�F�N�g��
	static D3DXVECTOR3 GetPos(ObjType type, int nID);			// ����̃I�u�W�F�N�g�̈ʒu�擾
	static D3DXVECTOR3 GetSize(ObjType type, int nID);			// ����̃I�u�W�F�N�g�̃T�C�Y�擾
	static D3DXCOLOR GetCol(ObjType type, int nID);

	void Release(void);									// �X�̃I�u�W�F�N�g�̍폜
	static void ObjRelease(ObjType type, int nID);				// ����̃I�u�W�F�N�g�̍폜
	static void PauseRelease(void);

protected:

private:
	static CScene		*m_apScene[OBJ_MAX][MAX_OBJECT];   // �V�[���̐ÓI�ϐ�
	static CScene		*m_pPauseScene;					   // �V�[�����~�߂�ÓI�ϐ�
	static CScene		*m_pPauseObj[PAUSE_MAX];
	static int			 m_nNumAll;						   // ���ׂẴI�u�W�F�N�g��
	static bool			 m_bPause;
	ObjType				 m_type;						   // �I�u�W�F�N�^�C�v
	D3DXVECTOR3			 m_pos;							   // �ʒu
	D3DXVECTOR3			 m_size;						   // �T�C�Y
	D3DXCOLOR			 m_col;							   // �F
	bool				 m_bBool;
	int					 m_nID;							   // ����̃i���o�[
};


#endif
//=============================================================================
//
// �����S������ [eraser.h]
// Author : 
//
//=============================================================================
#ifndef _ERASER_H_
#define _ERASER_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;		// ���f���N���X
class CPlayer;

//-----------------------------------------------------------------------------
// �����S���N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CEraser : public CScene
{
public:
	// �����S���̎��
	typedef enum 
	{
		ERASERTYPE_NORMAL = 0,	// �ʏ�̏����S��
		ERASERTYPE_SAND,		// ������
		ERASERTYPE_MAX,	
	}EraserType;

	CEraser(int nPriority);
	~CEraser();

	static CEraser *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel *GetModel(void) { return m_pModel; }
	EraserType GetType(void) { return m_eraserType; }
	void InitOffset(int nType, int nCntPattern, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetOffset(int nNum);
	void SetOffset(void);
	int SwicthType(void);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetColStage(bool bCol) { m_bColStage = bCol; }

	bool CollisionStage(int nRank);				// �X�e�[�W�Ƃ̓����蔻��

private:
	CModel *m_pModel;						// ���f���ւ̃|�C���^
	float **m_pOffset;						// �e���f������̃I�t�Z�b�g
	D3DXVECTOR3 **m_pOffsetPos;				// �e���f������̈ʒu�I�t�Z�b�g
	D3DXVECTOR3 **m_pOffsetRot;				// �e���f������̌����I�t�Z�b�g
	char m_apModelFileName[128];			// ���f���t�@�C���l�[��
	int m_nNumModel;						// ���f����
	int m_nNumOffset;						// ���Ԗڂ̃I�t�Z�b�g��

	D3DXMATRIX m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;						// ���݂̈ʒu
	D3DXVECTOR3 m_posOld;					// �O��̈ʒu
	D3DXVECTOR3 m_size;						// �T�C�Y
	D3DXVECTOR3 m_rot;						// ����
	D3DXVECTOR3 m_move;						// �ړ���

	EraserType m_eraserType;				// �����S���̃^�C�v
	bool m_bColStage;						// �X�e�[�W�ɓ����������ǂ���
};
#endif

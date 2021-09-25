//#############################################################################
//
// �ړ��w�b�_�t�@�C�� [move.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _MOVE_H_
#define _MOVE_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CMove
{
public:
	//-----------------------------------------------------------------------------
	// �����o�֐�
	//-----------------------------------------------------------------------------
	CMove() {}
	~CMove() {}

	static D3DXVECTOR3 MoveControl(D3DXVECTOR3 move, D3DXVECTOR3 deceleration);
	static float MoveSnake(float pos, float move, float Uplimit, float Downlimit, float Speed);
	static D3DXVECTOR3 TargetPosMove(D3DXVECTOR3 tagpos, D3DXVECTOR3 mypos, float Decele);
	static float SinWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime);
	static float CosWave(const float CenterPos, const float nHeight, const float nCycle, const float nTime);


	typedef struct
	{
		float fCenterpos;
		float fHeight;
		float fCycle;
	}WaveInfo;

private:

};


#endif // !_MOVE_H_

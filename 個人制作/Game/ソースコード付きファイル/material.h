//*****************************************************************************
//
// 素材管理 [material.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "manager.h"

class CMaterial
{
public:
	typedef enum TITLE
	{
		BG_TITLE,

		TITLE_LOGO,
		TITLE_BUTTON,


	};

	typedef enum TUTORIAL
	{
		BG_TUTORIAL,

		TUTORIAL_BUTTON,

	};

	typedef enum GAME
	{
		BG_GAME = 0,

		ENEMY_00,
		ENEMY_01,
		ENEMY_03,

		PLAYER_00,

		BULLET_00,
		BULLET_01,
		BULLET_03,

		LIFE_00,
		LIFE_01,

		TIME_00,

		EFFECT_00,
		EFFECT_01,
		EFFECT_03,

	};

	typedef enum RESULT
	{
		BG_RESULT,

	};

	CMaterial();
	~CMaterial();



private:

};



#endif // !_MATERIAL_H_

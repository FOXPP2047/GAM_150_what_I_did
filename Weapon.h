/*******************************************************************************
filename     Weapon.h
author       Minjeong.Kim, yongmin.cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description: This file is prototype of Weapon.c.

*******************************************************************************/
#ifndef WEAPON_H
#define WEAPON_H
#include "WarpEngine.h"
#include "WeaponData.h"
#include "Poison.h"
#include "Sieze.h"
#include "IronArrow.h"

//for using MJGun
typedef struct MJGun GUN;
//for using MJLaser
typedef struct MJLaser LASER;
//for using MJFreezing
typedef struct MJFreezing Freezing;
//for using MapInfo
typedef struct MapInfo MapInfo;

typedef struct WeaponManager
{
  // Gun head.
	GUN* pGunHead;
	// Laser head.
	LASER* pLaserHead;
	// Freeze head.
	Freezing* pFreezingHead;
	// Poison head.
	struct YMPoison* pPoisonHead;
	// Sieze head.
	struct YMSieze* pSiezeHead;
	// Iron head.
	struct YMIron* pIronHead;
}WeaponManager;

//Set the Weapon target.
void MJWeaponManagerSetTarget(WeaponManager* pWmag, WEVec2* pTargetCheck);
//Delete the Weapon planet.
void MJWeaponManagerDeleteByOwner(WeaponManager* pWmag, WEVec2* pOwner);
//Initialize the head to zero.
void MJWeaponManagerInit(WeaponManager* pWmag);
//Add the Weapon planet.
Weapon* MJWeaponManagerAdd(WeaponManager* pWmag, WEVec2* pOwner, float* pRot, int* pActive, E_Weapon ID, int textureId);
//Update the Weapon planet.
void MJWeaponManagerUpdate(WeaponManager* pWmag, MapInfo* pCurMap, float dt);
//Draw the Weapon planet.
void MJWeaponManagerDraw(WeaponManager* pWmag);
//Delete the Weapon planet.
void MJWeaponManagerDestroy(WeaponManager* pWmag);

#endif/*WEAPON_H*/
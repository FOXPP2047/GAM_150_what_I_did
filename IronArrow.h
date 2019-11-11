/*******************************************************************************
filename     IronArrow.h
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This is a prtotype of IronArrow.c.

*******************************************************************************/
#ifndef IRONARROW_H
#define IRONARROW_H
#include "GameObject.h"
#include "WeaponData.h"
#include "MapInfo.h" // for knowing entire map size.
//for using makeBullet.
typedef struct MJBullet IronArrow;
//for using size of entire map.
typedef struct MapInfo MapInfo;

typedef struct YMIron
{
	//for texture
	int textureID;
	//for initailize the weapon struct.
	Weapon base;
	//pos and scale.
	Transform trs;
	//for make bullet.
	struct MJBullet* pIronArrowHead;
	//Linkedlist.
	struct YMIron* pNext;
}Iron;

//Initialize the bullet and planet, texture of bullet.
void YMInitilaizeIron(Iron* pIron, int texture);
//Add and Intilaize the bullet, every adding time.
void YMAddIronArrow(Iron* pIron, WEVec2 direction);
//Update the planet and Bullet of planet.
void YMUpdateIronShot(Iron* pIron, MapInfo* pCurMap, float dt);
//for put off the bullet->onFire.
void YMOutofMap(Iron* pIron);
//Draw the Iron Bullet.
void YMDrawIronArrow(Iron* pIron);
#endif
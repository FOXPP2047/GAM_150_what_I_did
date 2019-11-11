/*******************************************************************************
filename     Sieze.h
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This is a prtotype of Sieze.c.

*******************************************************************************/
#ifndef SIEZE_H
#define SIEZE_H
#include "WeaponData.h" // Weapon
#include "GameObject.h" // Transform
#include "ParticleSystem.h" // MJPSAddEmitter

//for using makeBullet.
typedef struct MJBullet SlugBullet;

typedef struct YMSieze
{
  //for texutre.
	int textureID;
	//for initailize the weapon struct.
	Weapon base;
	//pos and scale.
	Transform trs;
	//for makeBullet.
	struct MJBullet* pSlugBulletHead;
	//Linkedlist.
	struct YMSieze* pNext;
}Sieze;

//Initialize the time texture, and head.
void YMInitializeSieze(Sieze* pSieze, int texture);
//Add and Intilaize the bullet, every adding time.
void YMAddSlugBullet(Sieze* pSieze, WEVec2 direction, WEVec2 destination);
//By condition, change the planet state, and update the position of Slug bullet.
void YMUpdateSieze(Sieze* pSieze, float dt);
//Bomb the particle.
void YMBombSieze(SlugBullet* pSlugBullet);
//Draw the SlugBullet.
void YMDrawSlugBullet(Sieze* pSieze);
#endif
/*******************************************************************************
filename     Poison.h
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This is a prtotype of Poison.c.

*******************************************************************************/
#ifndef POISON_H
#define POISON_H
#include "WeaponData.h" //weapon

typedef struct YMPoison
{
	//poison damage.
	float power;
	//for checking state.
	float Sparayed;
	//for checking state. 
	float SprayTime;
	//for initailize the weapon struct.
	Weapon base;
	//Linkedlist.
	struct YMPoison* pNext;
}POISON;

//Initailize the time texture and head.
void YMInitPoison(POISON* pPoison);
//Spray the particle.
void YMSprayPoison(POISON* pPoison, WEVec2 direction);
//Change the state of the POISON planet.
void YMUpdatePoison(POISON* pPoison, float dt);
#endif
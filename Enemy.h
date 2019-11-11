/*******************************************************************************
filename     Enemy.h
author       yongmin.cho
email				 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This is prototype of Enemy.c.

*******************************************************************************/
#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h" //Transform

//EnemyState enum
typedef enum {
	Dead,
	Alive
}EnemyState;
//EnemyMovementPattern enum
typedef enum EnemyMovementPattern{
	Normal,
	Curve,
}EnemyPattern;
//EnemyProperty enum
typedef enum{
	Sturdy,
	FastWeak,
	Alert,
	Big,
	Small,
}EnemyProperty;

typedef struct EnemyBase{
  //for pos and scale
	Transform physical;
	//Enemy State
	EnemyState state;
	//Enemy Pattern
	EnemyPattern pattern;
	//EnemyID
	EnemyProperty ID;
	//Enemy destination
	WEVec2 destination;
	//for Curve pattern
	float timer;
	//for Curve pattern
	WEVec2 acc;
	//Enemy life
	float life;
	//Enemt angle to core
	float rot;
	//each speed
	float speed;
	//when enemy died, user can earn this ammount.
	int cost;
	//if slow active enemy speed slow down
	int slow;
	//if bombed active, enemy damaged and changed color.
	int bombed;
	//if infected active, enemy damaged and changed color.
	float infected;
	//Enemt velocity including direction and size.
	WEVec2 vel;
	//Linkedlist
	struct EnemyBase* pNext;
}Enemy;

//Initialize when enemy have curved pattern
void YM_InitCurveEnemy(Enemy* pEnemy);
//Update for enemy what having Normal pattern
void YM_NormalUpdateEnemy(Enemy* pEnemy, float dt);
//Update for enemy what having Curve pattern
void YM_CurveUpdateEnemy(Enemy* pEnemy, float dt);

#endif
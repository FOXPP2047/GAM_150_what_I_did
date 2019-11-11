/*******************************************************************************
filename     Generator.h
author       yongmin.cho
email        qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:
This is a prtotype of Generator.c.

*******************************************************************************/
#ifndef GENERATOR_H
#define GENERATOR_H
#include "Enemy.h" //Enemy
// for infected enemy.
typedef struct WeaponManager WeaponManager;
//Enemy texture struct
typedef struct
{
	int sturdy;
	int fastWeak;
	int alert;
}EnemyTextures;

//MakeEnemy function
static Enemy* MakeEnemy(void);
//Intialize Sturdy property.
static void SturdyInit(Enemy* pEnemy);
//Intialize FastWeak property.
static void FastWeakInit(Enemy* pEnemy);
//Intialize Alert property.
static void AlertInit(Enemy* pEnemy);
//Intialize Big property.
static void BigInit(Enemy* pEnemy);
//Intialize Small property.
static void SmallInit(Enemy* pEnemy);

//Add how many dead enemy
void AddNumDeadEnemy(int a);
//return the numDeadenemy
int GetNumDeadEnemy(void);
//Set the numDeadenemy
void SetNumDeadEnemy(int a);

//for create Enemy
void EnemyManagerCreateEnemies(Enemy** ppEnemyHead, WEVec2 whereToCreate, EnemyProperty enemyProperty, EnemyPattern pattern, /*int numEnemis,*/WEVec2 destination);
//when big dead, small enemy appear.
void BigToSmallUpdate(Enemy** ppEnemyHead, Enemy* pCur);
//Enemy Update function
void EnemyManagerUpdateEnemies(Enemy** ppEnemyHead, WeaponManager* pWmag, float dt);
//Delete the Enemy
void EnemyManagerDeleteEnemies(Enemy** ppEnemyHead);
//Draw the Enemy
void EnemyManagerDrawEnemies(Enemy* pEnemyHead, int textures[]);
//Delete when program end.
void EnemyManagerDestroyAllEnemy(Enemy* pEnemyHead);
#endif
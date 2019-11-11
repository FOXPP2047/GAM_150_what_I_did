/*******************************************************************************
filename     Collisions.h
author       Minjeong.Kim, Yongmin.Cho
email        lovebkdn@gmail.com (Minjeong.kim)
						 qlxhdl55@gmail.com (yongmin.cho)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description:

function prototypes for collisions

Collision check and then give them<enemies> damage and effects.

Weapons and Enemies
Core and Enemies
Spike planet and Enemies

-MJCREnemyWeapon
-MJCREnemyCore
-MJCREnemyPlanet

*******************************************************************************/
#ifndef COLLISION_H
#define COLLISION_H

typedef struct EnemyBase Enemy;
typedef struct WeaponManager WeaponManager;
typedef struct Core Core;
typedef struct planetStruct YH_DEFENSEPLANET;

void MJCREnemyWeapon(Enemy* pEnemyHead, WeaponManager* pWmag, float dt);
void MJCREnemyCore(Enemy* pEnemyHead, Core* pCore);
void MJCREnemyPlanet(Enemy* pEnemyHead, YH_DEFENSEPLANET* pHead, float dt);

#endif /* COLLISION_H */
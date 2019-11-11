/*******************************************************************************
filename     WaveSystem.h
author       yongmin.cho Minjeong.Kim
email        qlxhdl55@gmail.com(yongmin.cho) lovebkdn@gmail.com (Minjeong.kim)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description: All function about Wave system, the wave numbers and
enemy types per wave and regions.

-YM_InitWaveSystem
-YM_AddRegion
-YM_AddInfoEnemyWave
-YM_AddEnemyInRegion
-YM_UpdateWave
-YM_DestroyWaveSystem

*******************************************************************************/
#ifndef WAVESYSTEM_H
#define WAVESYSTEM_H
#include "WarpEngine.h"

typedef struct Warning Warning;
typedef struct EnemyBase Enemy;

typedef struct
{
	int ID;
	WEVec2 pos;
	int active;
	float spawnTimer;
	int numEnemy;
}Region;

typedef struct WaveSystem_
{
	int WaveNumber;
	int numberOfplaceIDs;/*the sum of values of regionsPerWave*/
	int numberOfRegion;
	int* pRegionsPerWave;
	int* pPlaceIDs;
	Region* pRegions;
	float waveDuration;
	float spawnDuration;
	float waveTimer;
	int currentWave;
	int currentPlaceID;
	int* pNumEnemy;
	int* pEnemyProperty;
	int* pEnemyPattern;
	Enemy* pEnemyHead;
	Enemy* lastEnemy;
}WaveSystem;

void YM_InitWaveSystem(WaveSystem** ppWave, int regionnumber, int wavenumber, float duration, int* a_regionsPerWave);
void YM_AddRegion(WaveSystem* pWave, WEVec2 pos);
void YM_AddInfoEnemyWave(WaveSystem* pWave, int* pProperty, int* pPattern, int* pNum, int* a_placeIDs);
void YM_AddEnemyInRegion(WaveSystem* pWave, Warning* pWarn);
int YM_UpdateWave(WaveSystem* pWave, WEVec2 chase, float dt, Warning* pWarn);
void YM_DestroyWaveSystem(WaveSystem** pWave);
#endif
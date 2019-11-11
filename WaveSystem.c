/*******************************************************************************
filename     WaveSystem.c
author       Minjeong.Kim
email        lovebkdn@gmail.com (Minjeong.kim)
project name Meteorcre Defense
course       GAME150
Update       13/6/2015
copyright    All content (c) 2015 DigiPen (USA) Corporation, all rights
reserved.

Brief Description: All function about Wave system, the wave numbers and
enemy types per wave and regions.


*******************************************************************************/
#include "WaveSystem.h"
#include "WarningSystem.h" /* warning active management */
#include "Generator.h"	   /* enemy management			*/
#include <stdlib.h>		   /* malloc free				*/
#include "Sound.h"

static const float delayTime = 2.f; // delay time to exchange the mode.
static float delayTimer = 0.f;      // delay timer
/******************************************************************************/
/*!
Init Wave system

\param ppWave
a pointer to a pointer to wave system object.
\param regionnumber
the number of regions create enemies
\param wavenumber 
the number of waves in current state
\param duration
the wave duration
\param a_regionsPerWave
a pointer (array) to region IDs.
*/
/******************************************************************************/
void YM_InitWaveSystem(WaveSystem** ppWave, int regionnumber, int wavenumber, float duration, int* a_regionsPerWave)
{
	int i;
	WaveSystem* pWave;

	//init data

	SetNumDeadEnemy(0);
	delayTimer = 0;
	*ppWave = malloc(sizeof(WaveSystem));
	pWave = *ppWave;
	pWave->pRegions = malloc(sizeof(Region)*regionnumber);
	pWave->numberOfRegion = regionnumber;
	pWave->spawnDuration = 0.4f;
	pWave->waveDuration = duration;
	pWave->waveTimer = duration/2;
	pWave->WaveNumber = wavenumber;
	pWave->pRegionsPerWave = malloc(sizeof(int)*wavenumber);
	pWave->currentWave = 0;
	pWave->currentPlaceID = 0;
	pWave->numberOfplaceIDs = 0;
	pWave->pEnemyHead = 0;
	pWave->lastEnemy = 0;

	//init wave information
	for (i = 0; i < wavenumber; ++i)
	{
		pWave->pRegionsPerWave[i] = a_regionsPerWave[i];
		pWave->numberOfplaceIDs += a_regionsPerWave[i];
	}

	//make rooms for storage.
	pWave->pPlaceIDs = malloc(sizeof(int)*pWave->numberOfplaceIDs);
	pWave->pEnemyProperty = malloc(sizeof(int)*pWave->numberOfplaceIDs);
	pWave->pEnemyPattern = malloc(sizeof(int)*pWave->numberOfplaceIDs);
	pWave->pNumEnemy = malloc(sizeof(int)*pWave->numberOfplaceIDs);

	//init regions
	for (i = 0; i < regionnumber; ++i)
	{
		pWave->pRegions[i].active = 0;
		pWave->pRegions[i].ID = -1;
		pWave->pRegions[i].spawnTimer = 0.f;
		pWave->pRegions[i].numEnemy = 0;
	}
}
/******************************************************************************/
/*!
Destroy Wave system

it hold
free enemies which created in the head that wave system have
free regionPerWave
free pPlaceIDs
free enemyProperty
free enemyPattern
free NumEnemy
free region
free wave system

\param ppWave
*/
/******************************************************************************/
void YM_DestroyWaveSystem(WaveSystem** ppWave)
{
	WaveSystem* pWave;
	pWave = *ppWave;
	EnemyManagerDestroyAllEnemy(pWave->pEnemyHead);
	free(pWave->pRegionsPerWave);
	free(pWave->pPlaceIDs);
	free(pWave->pEnemyProperty);
	free(pWave->pEnemyPattern);
	free(pWave->pNumEnemy);
	free(pWave->pRegions);
	free(*ppWave);
}
/******************************************************************************/
/*!
Add Informations about enemies per Wave

\param pWave
a pointer to wave system
\param pProperty
the pointer (array)to properties of enemies
\param pPattern
the pointer (array)to pattern 
\param pNum
the pointer (array)to number of ememies per wave

\param a_placesIDs
array of the place id per wave

*/
/******************************************************************************/
void YM_AddInfoEnemyWave(WaveSystem* pWave, int* pProperty, int* pPattern, int* pNum, int* a_placesIDs)
{
	int i;
	for (i = 0; i < pWave->numberOfplaceIDs; ++i)
	{
		pWave->pPlaceIDs[i] = a_placesIDs[i];
		pWave->pEnemyProperty[i] = pProperty[i];
		pWave->pEnemyPattern[i] = pPattern[i];
		pWave->pNumEnemy[i] = pNum[i];
	}
}
/******************************************************************************/
/*!
Add Region information

\param pWave
a pointer to wave system
\param pos
a position of region

*/
/******************************************************************************/
void YM_AddRegion(WaveSystem* pWave, WEVec2 pos)
{
	int i;
	for (i = 0; i < pWave->numberOfRegion; ++i)
	{
		if (pWave->pRegions[i].ID != -1)
			continue;
		pWave->pRegions[i].ID = i;
		pWave->pRegions[i].pos = pos;
		break;
	}
}
/******************************************************************************/
/*!
Add Enemies in the Region

\param pWave
a pointer to wave system
\param pWarn
a pointer to warning object to activate

*/
/******************************************************************************/
void YM_AddEnemyInRegion(WaveSystem* pWave, Warning* pWarn)
{
	int i;
	int currentWave;
	int currentPlace;

	currentWave = pWave->currentWave;
	//update the data, currentPlaceID and currentWave
	for (i = 0; i < pWave->pRegionsPerWave[currentWave]; ++i)
	{
		currentPlace = pWave->pPlaceIDs[pWave->currentPlaceID + i];
		
		pWave->pRegions[currentPlace].active = 1;
		pWave->pRegions[currentPlace].spawnTimer = 0;
		pWave->waveTimer = 0.f;
		pWave->pRegions[currentPlace].numEnemy += pWave->pNumEnemy[pWave->currentPlaceID + i];
		WarningActive(pWarn, pWave->pRegions[currentPlace].pos);
	}
}
/******************************************************************************/
/*!
Update wave

\param pWave
a pointer to wave system
\param chase
a position of the chase object of enemy
\param dt
the usage of the time
\param pWarn
a warning object to use when move next wave (alarm)

\return
	0 if current stage is not done, otherwise 1.
*/
/******************************************************************************/
int YM_UpdateWave(WaveSystem* pWave, WEVec2 chase, float dt, Warning* pWarn)
{
	int i;
	pWave->waveTimer += dt;
	
	//if the final wave finished, delay some time and return 1 to say current stage done.
	if (pWave->WaveNumber <= pWave->currentWave && GetNumDeadEnemy() == 0)
	{
		delayTimer += dt;
		if (delayTime <= delayTimer)
			return 1;
	}
	
	//if it is time to start sending enemies or trigger,
	//and currentWave is not final wave
	//or triggerd
	if (pWave->WaveNumber > pWave->currentWave && (pWave->waveTimer >= pWave->waveDuration))
	{
		//and check the current regions are not activated, if it is activated, do not add enemies.
		for (i = 0; pWave->currentWave != 0 && i < pWave->pRegionsPerWave[pWave->currentWave - 1]; ++i)
		{
			if (pWave->pRegions[(pWave->pPlaceIDs[pWave->currentPlaceID + i])].active)
				break;
		}
		if (pWave->currentWave == 0 || i == pWave->pRegionsPerWave[pWave->currentWave - 1])
		{
			if (pWave->currentWave != 0)
				pWave->currentPlaceID += pWave->pRegionsPerWave[pWave->currentWave - 1];

			YM_AddEnemyInRegion(pWave, pWarn);
			//if it is final wave, send sound to  infomation
			if (pWave->currentWave == pWave->WaveNumber - 1)
				Sound_Play(SOUND_FINALWAVE);
			else //else, other info sound
				Sound_Play(SOUND_ENEMYAPPROACHING);
			++pWave->currentWave;
		}
	}

	//if regions are added enemies
	if (pWave->currentWave > 0)
		for (i = 0; i < pWave->pRegionsPerWave[pWave->currentWave - 1]; ++i)
		{
			int here = (pWave->pPlaceIDs[pWave->currentPlaceID + i]);
			// here region send enemies
			if (pWave->pRegions[here].active == 1)
			{
				pWave->pRegions[here].spawnTimer += dt;
				if (pWave->pRegions[here].spawnTimer >= pWave->spawnDuration)
				{
					EnemyManagerCreateEnemies(&pWave->pEnemyHead, pWave->pRegions[here].pos, pWave->pEnemyProperty[pWave->currentPlaceID + i], pWave->pEnemyPattern[pWave->currentPlaceID + i], chase);
					--pWave->pRegions[here].numEnemy;
					pWave->pRegions[here].spawnTimer = 0;
				}
				//deactivate when region has no enemy
				if (pWave->pRegions[here].numEnemy <= 0)
					pWave->pRegions[here].active = 0;
			}
		}

	return 0;
}
